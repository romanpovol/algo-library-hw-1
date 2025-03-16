module;

#include <vector>
#include <algorithm>

export module tree_algorithms:AATree;
import :BSTree;
import :TreeNode;
import :AATreeNode;

namespace trees {

    using namespace nodes;

    export template<typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T>>
    class AATree final : public BSTree<T, Comp, Alloc> {
        // Используем rebinding для управления памятью узлов типа RBTreeNode<T>
        using NodeAllocatorType = typename std::allocator_traits<Alloc>::template rebind_alloc<AATreeNode<T>>;

    public:
        explicit AATree(Comp comparator = Comp{}, Alloc allocator = Alloc{})
                : BSTree<T, Comp, Alloc>(comparator, allocator),
                  m_node_allocator(this->m_allocator)
        {}

        // Вставка элемента с последующей балансировкой по алгоритму RB-дерева
        void insert(const T& value) override {
            root = insertNode(root, value);
            ++this->m_size;
        }

        // Удаление элемента с последующей балансировкой
        bool remove(const T& value) override {
    
            auto new_root = removeNode(root, value);

            if (!deleted) {
                return false;
            }

            root = new_root;
            --this->m_size;

            return true;
        }

        ~RBTree() override {
            this->clear();
        }

    private:
        AATreeNode* insertNode(AATreeNode* node = this->m_root, const T& value) {
            if (!node) {
                return static_cast<AATreeNode<T>*>(create(value));;
            }
            if (this->m_comparator(value, node->value)) {
                node->left = insertNode(node->left, value);
            } else if (this->m_comparator(node->value, value)) {
                node->right = insertNode(node->right, value);
            } else {
                return node; 
            }
            node = node->right_rotate();
            node = node->left_rotate();
            return node;
        }

        void decreaseLevel(AATreeNode* node) {
            int shouldLevel = 1;
            if (node->left && node->right) {
                shouldLevel = std::min(node->left->level, node->right->level) + 1;
            } else if (node->left) {
                shouldLevel = node->left->level + 1;
            } else if (node->right) {
                shouldLevel = node->right->level + 1;
            }
            if (shouldLevel < node->level) {
                node->level = shouldLevel;
                if (node->right && shouldLevel < node->right->level) {
                    node->right->level = shouldLevel;
                }
            }
        }

        AATreeNode* removeNode(AATreeNode* node, int value) {
            if (!node) {
                return nullptr;
            }
    
            if (this->m_comparator(value, node->value)) {
                node->left = removeNode(node->left, value);
            } else if (this->m_comparator(value, node->value)) {
                node->right = removeNode(node->right, value);
            } else {
                if (!node->left || !node->right) {
                    AATreeNode* toDelete = node;
                    node = (node->left) ? node->left : node->right;
                    destroy(toDelete);
                } else {
                    AATreeNode* successor = minimum(node->right);
                    node->value = successor->value;
                    node->right = removeNode(node->right, successor->value);
                }
            }

            if (node) {
                decreaseLevel(node);
                node = node.right_rotate();
                if (node->right) {
                    node->right = node->right->right_rotate();
                }
                if (node->right && node->right->right) {
                    node->right->right = node->right->right_rotate();
                }
                node = split(node);
                if (node->right) {
                    node->right = node->right->left_rotate();
                }
            }
            return node;
        }

        AATreeNode<T>* minimum(AATreeNode<T>* node) {
            while (node->left != nullptr) {
                node = static_cast<AATreeNode<T>*>(node->left);
            }
            return node;
        }

        TreeNode<T>* create(const T& value) override {
            return this->BSTree<T, Comp, Alloc>::template create<AATreeNode<T>>(m_node_allocator, value);
        }

        void destroy(TreeNode<T>* node) override {
            this->BSTree<T, Comp, Alloc>::template destroy<AATreeNode<T>>(m_node_allocator, static_cast<AATreeNode<T>*>(node));
        }

        NodeAllocatorType m_node_allocator;
    };

} // namespace trees
