
#include <utility>  // pairを使用するためのヘッダーファイル
#include <optional> // optionalを使用するためのヘッダーファイル
#include <map>

#include "../othello.hpp"
#include "action.hpp"
using namespace std;

const map<Strategy, string> strategyNames = {
    {Strategy::RANDOM, "random"},
    {Strategy::MINIMAX, "minimax"},
    {Strategy::ALPHABETA, "alphabeta"},
    {Strategy::ITERATIVE_DEEPENING_ALPHA_BETA, "iterative_deepening_alpha_beta"},
    {Strategy::PRIMITIVE_MONTE_CARLO, "primitive_monte_carlo"},
    {Strategy::MONTE_CARLO_TREE_SEARCH, "monte_carlo_tree_search"},
};
Action::Action(int value) : id(value)
{
}
