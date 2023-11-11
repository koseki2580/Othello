
#include <vector>  // vectorを使用するためのヘッダーファイル
#include <utility> // pairを使用するためのヘッダーファイル
#include "../othello.hpp"
#include "../evaluation.hpp"
#include "action.hpp"
#include <functional> // functionを使用するためのヘッダーファイル
#include "mini_max_action.hpp"
using namespace std;

MiniMaxAction::MiniMaxAction(int id, int depth, const Evaluation evaluation) : Action(id), depth(depth)
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

optional<pair<int, int>> MiniMaxAction::action(Othello othello)
{
    auto result = miniMax(this->id, othello, 0);
    if (result.second == make_pair(-1, -1))
    {
        return nullopt;
    }
    return result.second;
}

pair<int, pair<int, int>> MiniMaxAction::miniMax(int _id, Othello othello, int nowDepth)
{
    if (nowDepth == depth)
    {
        // 自分から見た評価を計算する
        return make_pair(evaluation(othello, this->id), make_pair(-1, -1));
    }

    vector<pair<int, int>> actions = othello.legalActions(_id);

    int _base = (nowDepth & 1) == 1 ? -1 : 1;
    pair<int, pair<int, int>> nextPut = make_pair(10'000'000 * _base * -1, make_pair(-1, -1));

    if (actions.empty())
    {
        // スキップさせる手は最大の評価にする
        return nextPut;
    }

    for (const auto &action : actions)
    {
        Othello next_board = othello.copyBoard();
        next_board.put(_id, action);
        pair<int, pair<int, int>> value = miniMax(_id ^ 1, next_board, nowDepth + 1);

        if (_base * nextPut.first <= _base * value.first)
        {
            nextPut = make_pair(value.first, action);
        }
    }

    return nextPut;
}