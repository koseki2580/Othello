#ifndef RANDOMACTION_H
#define RANDOMACTION_H

#include "../othello.hpp" // othello.h のインクルードが必要
#include "action.hpp"     // action.h のインクルードが必要
#include <random>
#include <optional>

using namespace std;

class RandomAction : public Action
{
public:
    RandomAction(int value);

    optional<pair<int, int>> action(Othello othello) override;

private:
    int calc(const vector<pair<int, int>> &actions);
};

#endif // RANDOMACTION_H
