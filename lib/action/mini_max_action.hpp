#ifndef MINIMAX_ACTION_H
#define MINIMAX_ACTION_H
#include <vector>  // vectorを使用するためのヘッダーファイル
#include <utility> // pairを使用するためのヘッダーファイル
#include "../othello.hpp"
#include "../evaluation.hpp"
#include "action.hpp"
#include <functional> // functionを使用するためのヘッダーファイル

using namespace std;
class MiniMaxAction : public Action
{
public:
    int depth;
    std::function<int(Othello &, int)> evaluation;

    MiniMaxAction(int id, int depth, const Evaluation evaluation);

    optional<pair<int, int>> action(Othello othello) override;

    pair<int, pair<int, int>> miniMax(int _id, Othello othello, int nowDepth);
};

#endif // MINIMAX_ACTION_H
