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

#include "lib/player.hpp"
using namespace std;

class PlayOthello
{
public:
    PlayOthello(bool isFirstPlayer, string strategy, int depth, string evaluation)
    {
        Strategy cpu_strategy;
        Evaluation cpu_evaluation;
        if (strategy == "random")
        {
            cpu_strategy = Strategy::RANDOM;
        }
        else if (strategy == "minimax")
        {
            cpu_strategy = Strategy::MINIMAX;
        }
        else if (strategy == "alphabeta")
        {
            cpu_strategy = Strategy::ALPHABETA;
        }
        else
        {
            cpu_strategy = Strategy::RANDOM;
        }
        if (evaluation == "mass_count")
        {
            cpu_evaluation = Evaluation::MASS_COUNT;
        }
        else if (evaluation == "custom")
        {
            cpu_evaluation = Evaluation::CUSTOM;
        }
        else
        {
            cpu_evaluation = Evaluation::MASS_COUNT;
        }
        cout << "Evaluation: " << evaluationNames.at(cpu_evaluation) << endl;
        cout << "Strategy: " << strategyNames.at(cpu_strategy) << endl;
        cout << "depth: " << depth << endl;

        // オセロボードを作成する
        othello = Othello();
        playerId = isFirstPlayer ? 0 : 1;
        this->cpu = Player(playerId ^ 1, othello, cpu_strategy, depth, cpu_evaluation);
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

private:
    Othello othello;
    Player cpu = Player(0, othello, Strategy::RANDOM, 2, Evaluation::CUSTOM);
    int playerId;
};

extern "C"
{
    struct PlayOthelloWrapper
    {
        PlayOthello *instance;
    };

    PlayOthelloWrapper *createPlayOthello(bool isFirstPlayer, const char *strategy, int depth, const char *evaluation)
    {
        PlayOthelloWrapper *wrapper = new PlayOthelloWrapper;
        wrapper->instance = new PlayOthello(isFirstPlayer, strategy, depth, evaluation);
        return wrapper;
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