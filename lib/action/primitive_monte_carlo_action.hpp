#ifndef PRIMITIVEMONTECARLOACTION_H
#define PRIMITIVEMONTECARLOACTION_H

#include <utility> // pairを使用するためのヘッダーファイル
#include "../othello.hpp"
#include "action.hpp"
#include <string>
using namespace std;

class PrimitiveMonteCarloAction : public Action
{
public:
    PrimitiveMonteCarloAction(int id, int maxCount, int maxTimeInMilliseconds, int depth = -1);

    optional<pair<int, int>> action(Othello othello);

private:
    int maxCount;
    int maxTimeInMilliseconds;
    int maxDepth;

    pair<int, int> primitiveMonteCarlo(Othello othello);
    string simulate(Othello othello);
    int randCalc(const vector<pair<int, int>> &actions);
};
#endif // PRIMITIVEMONTECARLOACTION_H
