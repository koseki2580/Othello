#include "../othello.hpp" // othello.h のインクルードが必要
#include "action.hpp"     // action.h のインクルードが必要
#include "random_action.hpp"
#include <random>
#include <optional>

using namespace std;

RandomAction::RandomAction(int value) : Action(value)
{
}
optional<pair<int, int>> RandomAction::action(Othello othello)
{
    printf("random\n");
    vector<pair<int, int>> actions = othello.legalActions(this->id);
    if (actions.empty())
    {
        return nullopt;
    }
    int index = calc(actions);
    return actions[index];
}

int RandomAction::calc(const vector<pair<int, int>> &actions)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, actions.size() - 1);
    return dist(gen);
}