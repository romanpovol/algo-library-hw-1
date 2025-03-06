module;

#include <memory>

export module tree_algorithms:ScapeGoatTreeNode;
import :TreeNode;

namespace trees::nodes {

template <class T>
struct ScapeGoatTreeNode final : public TreeNode<T> {
    explicit ScapeGoatTreeNode(const T& value, ScapeGoatTreeNode* parent = nullptr)
        : TreeNode<T>(value), parent(parent), cnt(1) {}

	ScapeGoatTreeNode* parent; 
	int cnt;

    void set_left_child(ScapeGoatTreeNode* child) {
        this->left = child;
        if (child) child->parent = this;
    }

    void set_right_child(ScapeGoatTreeNode* child) {
        this->right = child;
        if (child) child->parent = this;
    }
};

} // namespace trees::nodes    
