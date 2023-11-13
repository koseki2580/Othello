#ifndef ALPHABETA_ACTION_H
#define ALPHABETA_ACTION_H
#include <functional> // functionを使用するためのヘッダーファイル

#include <vector>  // vectorを使用するためのヘッダーファイル
#include <utility> // pairを使用するためのヘッダーファイル
#include "../othello.hpp"
#include "../evaluation.hpp"
#include "action.hpp"

using namespace std;
class AlphaBetaAction : public Action
{
public:
    int depth;
    std::function<int(Othello &, int)> evaluation;

    AlphaBetaAction(int id, int depth, const Evaluation evaluation);

    optional<pair<int, int>> action(Othello othello) override;

    pair<int, pair<int, int>> alphaBeta(int _id, Othello othello, int nowDepth, int alpha, int beta);

private:
    const int maxNum = 10'000'000;
};

#endif // ALPHABETA_ACTION_H
