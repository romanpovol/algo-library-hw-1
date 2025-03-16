module;

#include <utility>

export module tree_algorithms:AATreeNode;
import :TreeNode;

namespace trees::nodes {
    template <class T>
    struct AATreeNode final : public TreeNode<T> {
        using TreeNode<T>::TreeNode;
        int level = 1;
        AATreeNode* rightRotate() {
            if (!this || !this->left) {
                return this;
            }
            if (this->left->level == this->level) {
                AATreeNode* L = this->left;
                this->left = L->right;
                L->right = this;
                return L;
            }
            return this;    
        }

        AATreeNode* leftRotate() {
            if (!this || !this->right || !this->right->right) {
                return this;
            }
            if (this->right->right->level == this->level) {
                AATreeNode* R = this->right;
                this->right = R->left;
                R->left = this;
                R->level++;
                return R;
            }
            return this;    
        }
    };

} // namespace trees::nodes
