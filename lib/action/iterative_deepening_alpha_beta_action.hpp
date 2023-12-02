#ifndef ITERATIVE_DEEPENING_ALPHA_BETA_H
#define ITERATIVE_DEEPENING_ALPHA_BETA_H

#include <functional>
#include <vector>
#include <utility>
#include <chrono>
#include "../othello.hpp"
#include "../evaluation.hpp"
#include "action.hpp"

using namespace std;

class IterativeDeepeningAlphaBetaAction : public Action
{
public:
    IterativeDeepeningAlphaBetaAction(int id, int depth, int maxTimeInMilliseconds, const Evaluation evaluation);
    optional<pair<int, int>> action(Othello othello) override;

private:
    int maxTimeInMilliseconds;
    int maxDepth;
    std::function<int(Othello &, int)> evaluation;
    int maxNum = 10'000'000;

    chrono::high_resolution_clock::time_point endTime;

    pair<int, pair<int, int>> iterativeDeepeningAlphaBeta(int _id, Othello othello, int nowDepth, int alpha, int beta, int maxDepth);
};

#endif // ITERATIVE_DEEPENING_ALPHA_BETA_H
