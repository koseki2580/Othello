#ifndef OTHELLO_HPP
#define OTHELLO_HPP

#include <vector>
#include <string>
#include <utility> // pairを使用するためのヘッダーファイル
#include <sstream> // stringstreamを使用するためのヘッダーファイル
#include <numeric> // accumulate関数を使用するためのヘッダーファイル
#include "othello.hpp"

class Othello
{
public:
    static const int mapSize = 8;
    inline static const int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    Othello();
    bool isDone();
    std::vector<std::pair<int, int>> legalActions(int player);
    bool isValidAction(int player, int y, int x);
    std::string getWinner();
    void put(int player, std::pair<int, int> pos);
    Othello copyBoard();
    std::pair<int, int> getCount();
    std::string toString();
    std::vector<std::vector<int>> getBoard();
    // コピー代入演算子
    Othello &operator=(const Othello &other);

private:
    std::vector<std::vector<int>> board;
};

#endif // OTHELLO_HPP
