#include <iostream>
#include <map>

import tree_algorithms;

#include "scenarios/Scenario/Scenario.h"
#include "scenarios/Random/Random.h"
#include "scenarios/Sorted/Sorted.h"


namespace {

    template<class T>
    using tree_ptr = std::unique_ptr<trees::BSTree<T>>;

    using scenario_ptr = std::unique_ptr<playground::IScenario>;

    std::pair<std::string, tree_ptr<int>> make_tree_by_name(const std::string& name) {
        if (name == "AVLTree") {
            return { "AVLTree<int>", std::make_unique<trees::AVLTree<int>>() };
        }
        if (name == "RBTree") {
            return { "RBTree<int>", std::make_unique<trees::RBTree<int>>() };
        }
        if (name == "SplayTree") {
            return { "SplayTree<int>", std::make_unique<trees::SplayTree<int>>() };
        }
        if (name == "TreapTree") {
            return { "TreapTree<int>", std::make_unique<trees::TreapTree<int>>() };
        }
        if (name == "ScapeGoatTree") {
            return { "ScapeGoatTree<int>", std::make_unique<trees::ScapeGoatTree<int>>() };
        }
        throw std::runtime_error("Unknown tree type provided: " + name);
    }

    std::vector<scenario_ptr> select_scenarios(const std::string& name) {
        std::vector<scenario_ptr> scenarios;
        if (name == "all") {
            scenarios.emplace_back(std::make_unique<playground::Random>());
            scenarios.emplace_back(std::make_unique<playground::Sorted>());
        }
        else if (name == "random") {
            scenarios.emplace_back(std::make_unique<playground::Random>());
        }
        else if (name == "sorted") {
            scenarios.emplace_back(std::make_unique<playground::Sorted>());
        }

        return scenarios;
    }

}



/**
 * Expects two positional arguments:
 * 1. Tree name: the name of the tree to use for benchmarking. Available values:
 *  1. `AVLTree`: AVL tree
 *  2. `RBTree`: Red-Black tree
 *  3. `SplayTree`: Splay tree
 *  4. `TreapTree`: Cartesian tree (Treap)
 *  5. `ScapeGoatTree`: ScapeGoat tree 
 * 2. Benchmarking scenario: the scenario to use for benchmark. Available values:
 *  1. `all`: runs all available benchmarking scenarios
 *  2. `random`: see the description of the [Random::run].
 *  3.`sorted`: see the description of the [Sorted::run].
 *
 * @param argc number of arguments, additional arguments expected: a tree name and benchmark scenario
 * @param argv arguments
 * @return
 */
int main(const int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Two additional arguments expected!\nUsage: " << argv[0] << " <tree> <scenario>\n";
        return 1;
    }

    const std::string tree_name = argv[1];
    const std::string scenario_name = argv[2];

    auto [tree_tag, tree]  = make_tree_by_name(tree_name);
    const std::vector<scenario_ptr> scenarios = select_scenarios(scenario_name);

    // running benchmarking scenarios
    for (auto& scenario : scenarios) {
        const trees::testing::Report report = scenario->run(tree_tag, tree);
        std::cout << report.str() << std::endl;
        // report.dump("report.txt");
    }

    return 0;
}