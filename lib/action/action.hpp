#ifndef OTHELLO_STRATEGY_H
#define OTHELLO_STRATEGY_H

#include <utility>  // pairを使用するためのヘッダーファイル
#include <optional> // optionalを使用するためのヘッダーファイル
#include <map>

#include "../othello.hpp"

using namespace std;

enum class Strategy
{
    RANDOM,
    MINIMAX,
    ALPHABETA,
    ITERATIVE_DEEPENING_ALPHA_BETA,
    PRIMITIVE_MONTE_CARLO,
    MONTE_CARLO_TREE_SEARCH,
};
extern const map<Strategy, string> strategyNames;

class Action
{

public:
    Action(int value);
    // 純粋仮想関数を宣言
    virtual optional<pair<int, int>> action(Othello othello) = 0;
    virtual ~Action() = default;

protected:
    int id;
};
#endif // OTHELLO_STRATEGY_H
