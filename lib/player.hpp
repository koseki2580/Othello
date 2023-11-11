#ifndef PLAYER_H
#define PLAYER_H

#include "othello.hpp"
#include "evaluation.hpp"
#include "action/action.hpp"
#include "action/random_action.hpp"
#include "action/mini_max_action.hpp"
#include <memory>  // unique_ptrを使用するためのヘッダーファイル
#include <vector>  // vectorを使用するためのヘッダーファイル
#include <utility> // pairを使用するためのヘッダーファイル
#include <optional>

class Player
{

public:
    Player(int _id, Othello &_othello, const Strategy strategy, int depth = 2, Evaluation evaluation = Evaluation::MASS_COUNT);

    pair<int, int> put();
    Player &operator=(const Player &player);

private:
    int id;
    Othello &othello;
    unique_ptr<Action> strategy;

    int (Player::*action)(const vector<pair<int, int>> &);
};

#endif // PLAYER_H
