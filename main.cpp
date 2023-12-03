// c++標準
#include <vector>
#include <random>
#include <string>
#include <utility> // pairを使用するためのヘッダーファイル
#include <sstream> // stringstreamを使用するためのヘッダーファイル
#include <optional>
#include <functional>
#include <memory>
#include <numeric> // accumulate関数を使用するためのヘッダーファイル
#include <iostream>

// emscripten用
#include <emscripten.h>
#include <emscripten/bind.h>
using namespace emscripten;

// 自分のコード
#include "lib/othello.hpp"
#include "lib/evaluation.hpp"
#include "lib/action/action.hpp"
#include "lib/action/random_action.hpp"
#include "lib/action/mini_max_action.hpp"
#include "lib/action/alpha_beta_action.hpp"
#include "lib/action/iterative_deepening_alpha_beta_action.hpp"
#include "lib/action/primitive_monte_carlo_action.hpp"
#include "lib/player.hpp"
using namespace std;

class PlayOthello
{
public:
    PlayOthello(bool isFirstPlayer)
    {
        // オセロボードを作成する
        othello = Othello();
        playerId = isFirstPlayer ? 0 : 1;
    }

    std::vector<pair<int, int>> getLegalActions()
    {
        return this->othello.legalActions(this->playerId);
    }

    std::vector<std::vector<int>> getBoardInfo()
    {
        return this->othello.getBoard();
    }

    void put(pair<int, int> action)
    {
        this->othello.put(playerId, action);
    }

    pair<int, int> cpuPut()
    {
        return this->cpu.put();
    }

    bool isDone()
    {
        return this->othello.isDone();
    }

    void setRandomAction()
    {
        strategy = Strategy::RANDOM;
    }

    void setMiniMaxAction(int depth, string evaluation)
    {
        setEvaluationFunction(evaluation);
        this->depth = depth;
        strategy = Strategy::MINIMAX;
    }

    void setAlphaBetaAction(int depth, string evaluation)
    {
        setEvaluationFunction(evaluation);
        this->depth = depth;
        strategy = Strategy::ALPHABETA;
    }

    void setIterativeDeepeningAlphaBetaAction(int depth, int maxTime, string evaluation)
    {
        setEvaluationFunction(evaluation);
        this->depth = depth;
        this->maxTime = maxTime;
        strategy = Strategy::ITERATIVE_DEEPENING_ALPHA_BETA;
    }

    void setPrimitiveMonteCarloAction(int maxCount, int maxTime, int depth)
    {
        this->maxCount = maxCount;
        this->maxTime = maxTime;
        this->depth = depth;
        strategy = Strategy::PRIMITIVE_MONTE_CARLO;
    }

    void decision()
    {
        cout << "Strategy: " << strategyNames.at(strategy) << endl;
        cout << "Evaluation: " << evaluationNames.at(this->evaluation) << endl;
        cout << "depth: " << depth << endl;
        cout << "maxTime: " << maxTime << endl;
        cout << "maxCount: " << maxCount << endl;
        this->cpu = Player(playerId ^ 1, othello, strategy, depth, maxTime, maxCount, evaluation);
    }

private:
    Othello othello;
    Strategy strategy = Strategy::RANDOM;
    int depth;
    int maxTime;
    int maxCount;
    Evaluation evaluation = Evaluation::MASS_COUNT;
    Player cpu = Player(0, othello, Strategy::RANDOM, 2, 1000, 100, Evaluation::CUSTOM);
    int playerId;

    void setEvaluationFunction(string _evaluation)
    {
        if (_evaluation == "mass_count")
        {
            this->evaluation = Evaluation::MASS_COUNT;
        }
        else if (_evaluation == "custom")
        {
            this->evaluation = Evaluation::CUSTOM;
        }
        else
        {
            this->evaluation = Evaluation::MASS_COUNT;
        }
    }
};

extern "C"
{
    struct PlayOthelloWrapper
    {
        PlayOthello *instance;
    };

    PlayOthelloWrapper *createPlayOthello(bool isFirstPlayer)
    {
        PlayOthelloWrapper *wrapper = new PlayOthelloWrapper;
        wrapper->instance = new PlayOthello(isFirstPlayer);
        return wrapper;
    }

    void setRandomAction(PlayOthelloWrapper *wrapper)
    {
        PlayOthello *playOthello = wrapper->instance;
        playOthello->setRandomAction();
    }

    void setMiniMaxAction(PlayOthelloWrapper *wrapper, int depth, const char *evaluation)
    {
        PlayOthello *playOthello = wrapper->instance;
        playOthello->setMiniMaxAction(depth, evaluation);
    }

    void setAlphaBetaAction(PlayOthelloWrapper *wrapper, int depth, const char *evaluation)
    {
        PlayOthello *playOthello = wrapper->instance;
        playOthello->setAlphaBetaAction(depth, evaluation);
    }

    void setIterativeDeepeningAlphaBetaAction(PlayOthelloWrapper *wrapper, int depth, int maxTime, const char *evaluation)
    {
        PlayOthello *playOthello = wrapper->instance;
        playOthello->setIterativeDeepeningAlphaBetaAction(depth, maxTime, evaluation);
    }

    void setPrimitiveMonteCarloAction(PlayOthelloWrapper *wrapper, int maxCount, int maxTime, int depth)
    {
        PlayOthello *playOthello = wrapper->instance;
        playOthello->setPrimitiveMonteCarloAction(maxCount, maxTime, depth);
    }

    void decision(PlayOthelloWrapper *wrapper)
    {
        PlayOthello *playOthello = wrapper->instance;
        playOthello->decision();
    }

    void deletePlayOthello(PlayOthelloWrapper *wrapper)
    {
        delete wrapper->instance;
        delete wrapper;
    }

    void getLegalActions(PlayOthelloWrapper *wrapper, int **outArray, int *outSize)
    {
        PlayOthello *playOthello = wrapper->instance;
        std::vector<std::pair<int, int>> legalActions = playOthello->getLegalActions();
        // printf("%d\n", *outSize);
        *outSize = legalActions.size();
        // printf("%d\n", *outSize);

        *outArray = new int[2 * legalActions.size()];

        for (size_t i = 0; i < legalActions.size(); i++)
        {
            (*outArray)[2 * i] = legalActions[i].first;
            (*outArray)[2 * i + 1] = legalActions[i].second;
            // printf("outArray: %d %d\n", (*outArray)[2 * i], (*outArray)[2 * i + 1]);
        }
    }

    void getBoardInfo(PlayOthelloWrapper *wrapper, int ***outBoard, int *outRows, int *outCols)
    {
        PlayOthello *playOthello = wrapper->instance;
        std::vector<std::vector<int>> board = playOthello->getBoardInfo();
        *outRows = board.size();
        if (*outRows > 0)
        {
            *outCols = board[0].size();
        }
        else
        {
            *outCols = 0;
        }

        *outBoard = new int *[*outRows];
        for (int i = 0; i < *outRows; i++)
        {
            (*outBoard)[i] = new int[*outCols];
            for (int j = 0; j < *outCols; j++)
            {
                (*outBoard)[i][j] = board[i][j];
            }
        }
    }

    void put(PlayOthelloWrapper *wrapper, int y, int x)
    {
        PlayOthello *playOthello = wrapper->instance;
        playOthello->put(std::make_pair(y, x));
    }

    void cpuPut(PlayOthelloWrapper *wrapper, int **outArray)
    {
        PlayOthello *playOthello = wrapper->instance;
        pair<int, int> putPoint = playOthello->cpuPut();
        // printf("%d, %d", putPoint.first, putPoint.second);
        *outArray = new int[2];
        (*outArray)[0] = putPoint.first;
        (*outArray)[1] = putPoint.second;
    }

    void isDone(PlayOthelloWrapper *wrapper, bool *_isDone)
    {
        PlayOthello *playOthello = wrapper->instance;
        *_isDone = playOthello->isDone();
    }
}