#include "othello.hpp"
#include "evaluation.hpp"
#include "action/action.hpp"
#include "action/random_action.hpp"
#include "action/mini_max_action.hpp"
#include "action/alpha_beta_action.hpp"
#include "action/iterative_deepening_alpha_beta_action.hpp"
#include "action/primitive_monte_carlo_action.hpp"

#include <memory>  // unique_ptrを使用するためのヘッダーファイル
#include <vector>  // vectorを使用するためのヘッダーファイル
#include <utility> // pairを使用するためのヘッダーファイル
#include <iostream>
#include <typeinfo>

#include "player.hpp"
using namespace std;
Player::Player(int _id, Othello &_othello, const Strategy strategy, int depth, int maxTime, int maxCount, Evaluation evaluation) : id(_id), othello(_othello)
{
    id = _id;
    // cout << "Player Evaluation: " << evaluationNames.at(evaluation) << endl;
    // cout << "Player Strategy: " << strategyNames.at(strategy) << endl;

    switch (strategy)
    {
    case Strategy::RANDOM:
        this->strategy = make_unique<RandomAction>(_id);
        break;
    case Strategy::MINIMAX:
        this->strategy = make_unique<MiniMaxAction>(_id, depth, evaluation);
        break;
    case Strategy::ALPHABETA:
        this->strategy = make_unique<AlphaBetaAction>(_id, depth, evaluation);
        break;
    case Strategy::ITERATIVE_DEEPENING_ALPHA_BETA:
        this->strategy = make_unique<IterativeDeepeningAlphaBetaAction>(_id, depth, maxTime, evaluation);
        break;
    case Strategy::PRIMITIVE_MONTE_CARLO:
        this->strategy = make_unique<PrimitiveMonteCarloAction>(_id, maxCount, maxTime, depth);
        break;
    default:
        this->strategy = make_unique<RandomAction>(_id);
        break;
    }
}

pair<int, int> Player::put()
{
    optional<pair<int, int>> action = strategy->action(othello.copyBoard());
    if (!action.has_value())
    {
        return make_pair(-1, -1);
    }
    othello.put(id, action.value());
    return action.value();
}

Player &Player::operator=(const Player &other)
{
    if (this != &other)
    {
        // idをコピー
        this->id = other.id;

        // othelloのコピー
        this->othello = other.othello;

        // strategyのコピーまたはムーブ操作
        this->strategy = std::move(const_cast<Player &>(other).strategy);

        // action関数ポインターのコピー
        this->action = other.action;
    }
    return *this;
}
