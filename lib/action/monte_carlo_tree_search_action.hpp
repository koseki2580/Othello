// monte_carlo_tree_search_action.hpp

#ifndef MONTE_CARLO_TREE_SEARCH_ACTION_HPP
#define MONTE_CARLO_TREE_SEARCH_ACTION_HPP

#include "../othello.hpp"
#include "action.hpp"
#include <vector>
#include <utility> // pairを使用するためのヘッダーファイル
using namespace std;

class Node
{
public:
    Othello othello;         // このノードの盤面状態
    pair<int, int> action;   // このノードでの行動
    int id;                  // このノードでのプレイヤーID
    Node *parent;            // 親ノード
    vector<Node *> children; // 子ノード
    int visits;              // このノードが選択された回数
    double value;            // このノードの評価値

    Node(Othello oth, int _id, pair<int, int> act, Node *par)
        : othello(oth), id(_id), action(act), parent(par), visits(0), value(0) {}

    void buildTree(); // 木を構築するメソッド
};

class MonteCarloTreeSearchAction : public Action
{
public:
    int maxTimeInMilliseconds; // 最大計算時間（反復深化）
    double explorationWeight;  // ハイパーパラメータ c
    int maxDepth;              // 探索する深さ
    int threshold;             // 木を展開する閾値
    int maxCount;              // 最大試行回数

    MonteCarloTreeSearchAction(int id, int threshold = 10, int maxTimeInMilliseconds = 10000, double explorationWeight = 1.41421356237, int maxCount = 300, int depth = -1);

    optional<pair<int, int>> action(Othello othello) override;

private:
    Node *select(Node *node, int cum_time);        // ノードの選択メソッド
    Node *bestUct(Node *node, int cum_time);       // UCTに基づく最良のノードを選択するメソッド
    double uctValue(Node *child, int cum_time);    // UCT値を計算するメソッド
    double simulate(Othello othello, int _id);     // シミュレーションを行うメソッド
    void backPropagate(Node *node, double result); // 評価値を親ノードに伝播させるメソッド
    Node *bestChild(Node *node);                   // 最良の子ノードを選択するメソッド
    int randCalc(const vector<pair<int, int>> &actions);

    const int maxNum = 10'000'000;
};

#endif // MONTE_CARLO_TREE_SEARCH_ACTION_HPP
