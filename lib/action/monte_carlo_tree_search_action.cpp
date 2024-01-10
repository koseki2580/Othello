#include "../othello.hpp" // othello.h のインクルードが必要
#include "action.hpp"     // action.h のインクルードが必要
#include "monte_carlo_tree_search_action.hpp"
#include <chrono>
#include <random>
#include <optional>
#include <vector> // vectorを使用するためのヘッダーファイル
#include <string>
#include <algorithm> // 必要なヘッダーファイル
#include <iostream>

using namespace std;

void Node::buildTree()
{
    // 木に子ノードを展開するためのメソッド
    for (const auto &action : othello.legalActions(id ^ 1))
    {
        Othello next_othello = othello.copyBoard();
        next_othello.put(id ^ 1, action);
        // 取れるアクション毎に子ノードを作成
        Node *child = new Node(next_othello, id ^ 1, action, this);
        children.push_back(child);
    }
}

MonteCarloTreeSearchAction::MonteCarloTreeSearchAction(int id, int threshold, int maxTimeInMilliseconds, double explorationWeight, int maxCount, int depth)
    : Action(id), maxTimeInMilliseconds(maxTimeInMilliseconds), explorationWeight(explorationWeight), maxDepth(depth), threshold(threshold), maxCount(maxCount)
{
}

optional<pair<int, int>> MonteCarloTreeSearchAction::action(Othello othello)
{
    if (othello.legalActions(id).empty())
    {
        return nullopt;
    }

    Node *root = new Node(othello, id ^ 1, make_pair(-1, -1), nullptr);
    root->buildTree();

    auto endTime = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(maxTimeInMilliseconds);
    int count = 0;

    while (std::chrono::high_resolution_clock::now() < endTime && count < maxCount)
    {
        Node *node = select(root, count);
        double result = simulate(node->othello, node->id);
        backPropagate(node, result);

        if (node->visits > threshold)
        {
            node->buildTree();
        }

        count++;
    }

    Node *bestChild = this->bestChild(root);
    return bestChild->action;
}

Node *MonteCarloTreeSearchAction::select(Node *node, int cumTime)
{
    while (!node->children.empty())
    {
        node = bestUct(node, cumTime);
    }
    return node;
}

Node *MonteCarloTreeSearchAction::bestUct(Node *node, int cumTime)
{
    return *max_element(
        node->children.begin(), node->children.end(),
        [this, cumTime](Node *child1, Node *child2)
        {
            return uctValue(child1, cumTime) < uctValue(child2, cumTime);
        });
}

Node *MonteCarloTreeSearchAction::bestChild(Node *node)
{
    return *max_element(
        node->children.begin(), node->children.end(),
        [](Node *child1, Node *child2)
        {
            return child1->visits < child2->visits;
        });
}

double MonteCarloTreeSearchAction::uctValue(Node *child, int cumTime)
{
    if (child->visits == 0)
    {
        return maxNum;
    }
    // UCT
    // c *sqrt(ln(N) / n) + v / n
    // c : ペナルティ項目
    // N : 累計試行回数
    // n : このNodeの試行回数
    // v : 価値
    return (child->value / child->visits) + explorationWeight * (sqrt(log(cumTime) / child->visits));
}

double MonteCarloTreeSearchAction::simulate(Othello othello, int _id)
{
    int play_id = _id ^ 1;
    int depth = 0;

    while (!othello.isDone() && depth <= maxDepth)
    {
        vector<std::pair<int, int>> actions = othello.legalActions(play_id);
        if (actions.empty())
        {
            play_id ^= 1;
            continue;
        }
        int random_index = randCalc(actions);
        othello.put(play_id, actions[random_index]);
        play_id ^= 1;
        depth++;
    }

    pair<int, int> result = othello.getCount();

    if (result.first < result.second)
    {
        return (id == 0) ? 0 : 1;
    }
    else if (result.first > result.second)
    {
        return (id == 0) ? 1 : 0;
    }
    return 0.5;
}

void MonteCarloTreeSearchAction::backPropagate(Node *node, double result)
{
    while (node != nullptr)
    {
        node->visits++;
        node->value += (node->action != make_pair(-1, -1)) ? result : 0;
        node = node->parent;
    }
}

int MonteCarloTreeSearchAction::randCalc(const vector<pair<int, int>> &actions)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, actions.size() - 1);
    return dist(gen);
}
