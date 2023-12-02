#include <functional> // functionを使用するためのヘッダーファイル
#include <chrono>
#include <vector>  // vectorを使用するためのヘッダーファイル
#include <utility> // pairを使用するためのヘッダーファイル
#include "../othello.hpp"
#include "../evaluation.hpp"
#include "action.hpp"
#include "iterative_deepening_alpha_beta_action.hpp"
#include <unordered_map> // キャッシュに使用するためのヘッダーファイル

IterativeDeepeningAlphaBetaAction::IterativeDeepeningAlphaBetaAction(int id, int depth, int maxTimeInMilliseconds, const Evaluation evaluation)
    : Action(id), maxTimeInMilliseconds(maxTimeInMilliseconds), maxDepth(depth)
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

optional<pair<int, int>> IterativeDeepeningAlphaBetaAction::action(Othello othello)
{
    auto startTime = chrono::high_resolution_clock::now();
    endTime = startTime + chrono::milliseconds(maxTimeInMilliseconds);

    pair<int, pair<int, int>> bestMove = make_pair(-maxNum, make_pair(-1, -1));

    int depth = 1;

    while (chrono::high_resolution_clock::now() < endTime && depth <= maxDepth)
    {
        auto result = iterativeDeepeningAlphaBeta(this->id, othello, 0, -maxNum, maxNum, depth);
        if (result.second != make_pair(-1, -1))
        {
            bestMove = result;
        }
        depth++;
    }

    if (bestMove.second == make_pair(-1, -1))
    {
        return nullopt;
    }
    return bestMove.second;
}

pair<int, pair<int, int>> IterativeDeepeningAlphaBetaAction::iterativeDeepeningAlphaBeta(int _id, Othello othello, int nowDepth, int alpha, int beta, int maxDepth)
{
    if (nowDepth == maxDepth || chrono::high_resolution_clock::now() >= endTime)
    {
        return make_pair(evaluation(othello, this->id), make_pair(-1, -1));
    }

    vector<pair<int, int>> actions = othello.legalActions(_id);

    int _base = (nowDepth & 1) == 1 ? -1 : 1;
    pair<int, pair<int, int>> nextPut = make_pair(-maxNum, make_pair(-1, -1));

    if (actions.empty())
    {
        // スキップさせる手は最大の評価にする
        return nextPut;
    }

    for (const auto &action : actions)
    {
        Othello nextBoard = othello.copyBoard();
        nextBoard.put(_id, action);
        pair<int, pair<int, int>> value = iterativeDeepeningAlphaBeta(_id ^ 1, nextBoard, nowDepth + 1, alpha, beta, maxDepth);

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

        // Alpha-Beta pruning
        if (beta <= alpha)
        {
            break;
        }
    }

    return nextPut;
}
