module;

#include <vector>
#include <cmath>
#include <iostream>
#include <memory>

export module tree_algorithms:ScapeGoatTree;
import :BSTree;
import :TreeNode;
import :ScapeGoatTreeNode;

namespace trees {

using namespace nodes;

export template<typename T, typename Comp = std::less<T>, typename Alloc = std::allocator<T>>
class ScapeGoatTree final : public BSTree<T, Comp, Alloc> {
    using NodeAllocatorType = typename std::allocator_traits<Alloc>::template rebind_alloc<ScapeGoatTreeNode<T>>;
    
public:
    explicit ScapeGoatTree(Comp comparator = Comp{}, Alloc allocator = Alloc{})
            : BSTree<T, Comp, Alloc>(comparator, allocator),
                m_node_allocator(this->m_allocator), m_size(0), m_q(0)
    {}

    void insert(const T& value) override {
        auto node = static_cast<ScapeGoatTreeNode<T>*>(create(value));
        int depth = insert_with_depth(node);

        if (depth > calcHeight()) {
            while (2 * size(node->parent) >= 3 * size(node)) {
                node = node->parent;
            }
            node = node->parent;
            rebuild(node);
        }
    }

    bool remove(const T& value) override {
        bool deleted = remove(search(this->m_root, value));
		if (deleted){
			m_size--;
			if (m_size < 2.0 / 3 * m_q) {
				rebuild(static_cast<ScapeGoatTreeNode<T>*>(this->m_root));
				m_q = m_size;
			}
		}
		return deleted;
    }

    size_t count(const T& value) override {
        std::vector<TreeNode<T>*> stack;

        if (this->m_root != nullptr) {
            stack.push_back(this->m_root);
        }

        size_t answer = 0;

        while (!stack.empty()) {
            TreeNode<T> *node = stack.back();
            stack.pop_back();

            // equivalent to: node.value == value
            if (!this->m_comparator(node->value, value) && !this->m_comparator(value, node->value)) {
                answer += static_cast<ScapeGoatTreeNode<T>*>(node)->cnt;
            }

            if (node->left != nullptr) {
                stack.push_back(node->left);
            }
            if (node->right != nullptr) {
                stack.push_back(node->right);
            }
        }

        return answer;
    }

    ~ScapeGoatTree() override {
        this->clear();
    }

private:

    int insert_with_depth(ScapeGoatTreeNode<T>* node) {
        auto rt = static_cast<ScapeGoatTreeNode<T>*>(this->m_root); 
        if (!rt) {
            this->m_root = node;
            m_size++;
            m_q++;
            return 0;
        }

        bool inserted = false;
        int depth = 0;
        while (!inserted) {
            if (node->value > rt->value) {
                if (!rt->right) {
                    rt->right = node;
                    node->parent = rt;
                    inserted = true;
                } else {
                    rt = static_cast<ScapeGoatTreeNode<T>*>(rt->right);
                }
            } else if (node->value < rt->value) {
                if (!rt->left) {
                    rt->left = node;
                    node->parent = rt;
                    inserted = true;
                } else {
                    rt =  static_cast<ScapeGoatTreeNode<T>*>(rt->left);
                }
            } else {
                rt->cnt++;
                destroy(node);
                return -1;
            }
            depth++;
        }
        m_q++;
        m_size++;
        return depth;
    }

    bool remove(ScapeGoatTreeNode<T>* node) {
        if (!node) {
            return false;
        }

        if (node->cnt > 1) {
            node->cnt--;
            return true;
        }

        if (!node->left && !node->right) {
            if (node != this->m_root) {
                auto parent = static_cast<ScapeGoatTreeNode<T>*>(node->parent);
                if (parent->left == node) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }
            } else {
                this->m_root = nullptr;
            }
            destroy(node);
        } else if (node->left && node->right) {
            auto successor = get_successor(node->right);
            if (successor) {
                int saved_value = successor->value;
                int saved_cnt = successor->cnt;
                remove(successor);
                node->value = saved_value;
                node->cnt = saved_cnt;
            }
        } else {
            auto child = node->left ? 
            static_cast<ScapeGoatTreeNode<T>*>(node->left) : 
            static_cast<ScapeGoatTreeNode<T>*>(node->right);

            if (node != this->m_root) {
                auto parent = static_cast<ScapeGoatTreeNode<T>*>(node->parent);
                if (parent->left == node) {
                    parent->left = child;
                } else {
                    parent->right = child;
                }
                if (child) {
                    child->parent = parent;
                }
            } else {
                this->m_root = child;
                if (child) {
                    child->parent = nullptr;
                }
            }
            destroy(node);
        }
        return true;
    }

    ScapeGoatTreeNode<T>* get_successor(TreeNode<T>* node) {
        auto p = static_cast<ScapeGoatTreeNode<T>*>(node);
        while (p && p->left) {
            p = static_cast<ScapeGoatTreeNode<T>*>(p->left);
        }
        return p;
	}

    int size(TreeNode<T>* node) const	{
		if (!node) {
			return 0;
        }
		return size(node->right) + size(node->left) + 1;
	}

    ScapeGoatTreeNode<T>* search(TreeNode<T>* node, T value)	{
		if (!node || node->value == value) {
			return static_cast<ScapeGoatTreeNode<T>*>(node);
        }
		if (node->value > value) {
			return search(node->left, value);
        }
		return search(node->right, value);
	}

    double calcHeight() {
        return 2.466 * log(m_q);
    }

    void rebuild(ScapeGoatTreeNode<T>* node) {
        auto up = (node == nullptr ? nullptr : static_cast<ScapeGoatTreeNode<T>*>(node->parent));
        std::vector <ScapeGoatTreeNode<T>*> v;
        put_in_array(v, node);
        if (up == nullptr) {
            auto root = static_cast<ScapeGoatTreeNode<T>*>(this->m_root);
            root = build_balanced(v);
            if (root) {
                root->parent = nullptr;
            }
        } else if (up->left == node) {
            auto left = static_cast<ScapeGoatTreeNode<T>*>(up->left);
            left = build_balanced(v);
            if (left) {
                left->parent = up;
            }
        } else {
            auto right = static_cast<ScapeGoatTreeNode<T>*>(up->right);
            right = build_balanced(v);
            if (right) {
			    right->parent = up;
            }
        }
    }

    ScapeGoatTreeNode<T>* build_balanced(std::vector<ScapeGoatTreeNode<T>*> &v) {
        if (v.empty()) {
			return nullptr;
        }
		int center = v.size() / 2;
		ScapeGoatTreeNode<T>* root = v[center];
		std::vector<ScapeGoatTreeNode<T>*> l(v.begin(), v.begin() + center);
        ScapeGoatTreeNode<T>* left = static_cast<ScapeGoatTreeNode<T>*>(root->left);
		left = build_balanced(l);
		if (left) {
			left->parent = root;
        }
		std::vector<ScapeGoatTreeNode<T>*> r(v.begin() + center + 1, v.end());
        ScapeGoatTreeNode<T>* right = static_cast<ScapeGoatTreeNode<T>*>(root->right);
		right = build_balanced(r);
		if (right != 0) {
			right->parent = root;
        }
        
		return root;
    }

    void put_in_array(std::vector<ScapeGoatTreeNode<T>*> &v, ScapeGoatTreeNode<T>* node) {
		if (!node) {
			return;
        }
        put_in_array(v, static_cast<ScapeGoatTreeNode<T>*>(node->left));
		v.push_back(node);
		put_in_array(v, static_cast<ScapeGoatTreeNode<T>*>(node->right));
	}

    TreeNode<T>* create(const T& value) override {
        return this->BSTree<T, Comp, Alloc>::template create<ScapeGoatTreeNode<T>>(m_node_allocator, value);
    }

    void destroy(TreeNode<T>* node) override {
        this->BSTree<T, Comp, Alloc>::template destroy<ScapeGoatTreeNode<T>>(m_node_allocator, static_cast<ScapeGoatTreeNode<T>*>(node));
    }

    NodeAllocatorType m_node_allocator;    
    int m_size;
    int m_q;
};

} // namespace trees