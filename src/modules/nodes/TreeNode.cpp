module;

#include <memory>

export module tree_algorithms:TreeNode;



namespace trees::nodes {

template <class T>
struct TreeNode {
    explicit TreeNode(T value) : value(std::move(value)) {}
    virtual ~TreeNode() = default;

    T value;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;
};

}
