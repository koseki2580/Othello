#include <functional> // functionを使用するためのヘッダーファイル

#include <vector>  // vectorを使用するためのヘッダーファイル
#include <utility> // pairを使用するためのヘッダーファイル
#include "../othello.hpp"
#include "../evaluation.hpp"
#include "action.hpp"
#include "alpha_beta_action.hpp"
using namespace std;

AlphaBetaAction::AlphaBetaAction(int id, int depth, const Evaluation evaluation) : Action(id), depth(depth)
{
    // 評価関数を選択
    switch (evaluation)
    {
    case Evaluation::MASS_COUNT:
        this->evaluation = [](Othello &othello, int id)
        {
            return EvaluationFunction::massCount(othello, id);
        };
        break;
    case Evaluation::CUSTOM:
        this->evaluation = [](Othello &othello, int id)
        {
            return EvaluationFunction::customEvaluation(othello, id);
        };
        break;

    default:
        this->evaluation = [](Othello &othello, int id)
        {
            return EvaluationFunction::massCount(othello, id);
        };
        break;
    }
}

optional<pair<int, int>> AlphaBetaAction::action(Othello othello)
{
    auto result = alphaBeta(this->id, othello, 0, maxNum * -1, maxNum);
    if (result.second == make_pair(-1, -1))
    {
        return nullopt;
    }
    return result.second;
}

pair<int, pair<int, int>> AlphaBetaAction::alphaBeta(int _id, Othello othello, int nowDepth, int alpha, int beta)
{
    if (nowDepth == depth)
    {
        // 自分から見た評価を計算する
        return make_pair(evaluation(othello, this->id), make_pair(-1, -1));
    }

    vector<pair<int, int>> actions = othello.legalActions(_id);

    int _base = (nowDepth & 1) == 1 ? -1 : 1;
    pair<int, pair<int, int>> nextPut = make_pair(maxNum * _base * -1, make_pair(-1, -1));

    if (actions.empty())
    {
        // スキップさせる手は最大の評価にする
        return nextPut;
    }

    for (const auto &action : actions)
    {
        Othello next_board = othello.copyBoard();
        next_board.put(_id, action);
        pair<int, pair<int, int>> value = alphaBeta(_id ^ 1, next_board, nowDepth + 1, alpha, beta);

        if (_base * nextPut.first <= _base * value.first)
        {
            nextPut = make_pair(value.first, action);
        }
        if (nowDepth & 1)
        {
            alpha = max(alpha, value.first);
        }
        else
        {
            beta = min(beta, value.first);
        }
        if (beta <= alpha)
        {
            // αカット
            // βカット
            break;
        }
        }

    return nextPut;
}