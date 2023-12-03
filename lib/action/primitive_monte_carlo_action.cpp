#include "../othello.hpp" // othello.h のインクルードが必要
#include "action.hpp"     // action.h のインクルードが必要
#include "primitive_monte_carlo_action.hpp"
#include <chrono>
#include <random>
#include <optional>
#include <vector> // vectorを使用するためのヘッダーファイル
#include <string>
#include <iostream>

using namespace std;

PrimitiveMonteCarloAction::PrimitiveMonteCarloAction(int id, int maxCount, int maxTimeInMilliseconds, int depth)
    : Action(id), maxCount(maxCount), maxTimeInMilliseconds(maxTimeInMilliseconds), maxDepth(depth)
{
}

optional<pair<int, int>> PrimitiveMonteCarloAction::action(Othello othello)
{
    pair<int, int> next_action = primitiveMonteCarlo(othello);
    if (next_action == make_pair(-1, -1))
    {
        return nullopt;
    }
    return next_action;
}

pair<int, int> PrimitiveMonteCarloAction::primitiveMonteCarlo(Othello othello)
{
    std::vector<std::pair<int, int>> actions = othello.legalActions(this->id);
    if (actions.empty())
    {
        return make_pair(-1, -1);
    }
    auto startTime = chrono::high_resolution_clock::now();
    auto endTime = startTime + chrono::milliseconds(maxTimeInMilliseconds);
    int count = 0;

    vector<pair<double, int>> actionResult(actions.size(), {0, 0});

    string win_id = this->id == 0 ? "White Win" : "Black Win";

    while (count < maxCount && chrono::high_resolution_clock::now() < endTime)
    {
        Othello next_board = othello;
        next_board.put(this->id, actions[count % actions.size()]);
        string result = simulate(next_board);
        if (result == win_id)
        {
            actionResult[count % actionResult.size()].first += 1;
        }
        else if (result == "Draw")
        {
            actionResult[count % actionResult.size()].first += 0.5;
        }

        actionResult[count % actionResult.size()].second += 1;
        count += 1;
    }

    int idx = 0;
    double max_ = actionResult[0].first / actionResult[0].second;

    for (int i = 1; i < actionResult.size(); ++i)
    {
        if (actionResult[i].second == 0)
        {
            continue;
        }

        double evl = actionResult[i].first / actionResult[i].second;

        if (max_ < evl)
        {
            idx = i;
            max_ = evl;
        }
    }
    return actions[idx];
}

string PrimitiveMonteCarloAction::simulate(Othello othello)
{
    int play_id = this->id ^ 1;

    if (maxDepth <= 0)
    {
        while (!othello.isDone())
        {
            vector<pair<int, int>> actions = othello.legalActions(play_id);

            if (actions.empty())
            {
                play_id ^= 1;
                continue;
            }

            int random_index = randCalc(actions);
            othello.put(play_id, actions[random_index]);
            play_id ^= 1;
        }

        return othello.getWinner();
    }
    else
    {
        int depth = 0;

        while (!othello.isDone() && depth < maxDepth)
        {
            std::vector<std::pair<int, int>> actions = othello.legalActions(play_id);

            if (actions.empty())
            {
                play_id ^= 1;
                continue;
            }

            int random_index = randCalc(actions);
            othello.put(play_id, actions[random_index]);
            play_id ^= 1;
            depth += 1;
        }

        std::pair<int, int> result = othello.getCount();

        if (result.first < result.second)
        {
            return "Black Win";
        }
        else if (result.first > result.second)
        {
            return "White Win";
        }

        return "Draw";
    }
}

int PrimitiveMonteCarloAction::randCalc(const vector<pair<int, int>> &actions)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, actions.size() - 1);
    return dist(gen);
}