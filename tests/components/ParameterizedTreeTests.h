//
// Created by Vladislav.Artiukhov on 08.02.25.
//

#ifndef PARAMETERIZEDTREETESTS_H
#define PARAMETERIZEDTREETESTS_H

#include <gtest/gtest.h>

import tree_algorithms;


// Define the tree types to test (extend this list as needed)
// NOTE: extend these templates according to support testing for other tree types
template <typename T, typename Comp, typename Alloc> using AVLTreeType = trees::AVLTree<T, Comp, Alloc>;
template <typename T, typename Comp, typename Alloc> using SplayTreeType = trees::SplayTree<T, Comp, Alloc>;
template <typename T, typename Comp, typename Alloc> using TreapTreeType = trees::TreapTree<T, Comp, Alloc>;
template <typename T, typename Comp, typename Alloc> using RBTreeType = trees::RBTree<T, Comp, Alloc>;
template <typename T, typename Comp, typename Alloc> using ScapeGoatTreeType = trees::ScapeGoatTree<T, Comp, Alloc>;


template <typename TreeType>
class ParameterizedTreeTests : public ::testing::Test {};


template <template <typename, typename, typename> class TreeTemplate>
struct TreeTemplateWrapper {
    // Aliases the tree type with a specific type `T`, `Comp`, and `Alloc`
    template <typename T, typename Comp, typename Alloc> using Type = TreeTemplate<T, Comp, Alloc>;
};

/**
 * Insert new tree subclass here to run unit-tests on it.
 */
using TreeTypeTemplates = ::testing::Types<
    TreeTemplateWrapper<AVLTreeType>,
    TreeTemplateWrapper<SplayTreeType>,
    TreeTemplateWrapper<TreapTreeType>,
    TreeTemplateWrapper<RBTreeType>,
    TreeTemplateWrapper<ScapeGoatTreeType>
>;

template <typename TreeWrapper>
class ParameterizedTreeTester : public ::testing::Test {
protected:
    // Define the alias for the unwrapped tree type
    // Actual tree type for testing
    template <typename T, typename Comp, typename Alloc> using TreeType = typename TreeWrapper::template Type<T, Comp, Alloc>;
};


#endif //PARAMETERIZEDTREETESTS_H
