#include <vector>
#include <string>
#include <utility> // pairを使用するためのヘッダーファイル
#include <sstream> // stringstreamを使用するためのヘッダーファイル
#include <numeric> // accumulate関数を使用するためのヘッダーファイル
#include "othello.hpp"
using namespace std;
Othello::Othello()
{
    board = vector<vector<int>>(mapSize, vector<int>(mapSize, -1));
    // 0がwhite
    // 1がblackとする
    int half = mapSize / 2;
    board[half][half] = 0;
    board[half - 1][half] = 1;
    board[half][half - 1] = 1;
    board[half - 1][half - 1] = 0;
}

bool Othello::isDone()
{
    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            if (board[i][j] == -1 && legalActions(0).size() + legalActions(1).size() != 0)
            {
                return false;
            }
        }
    }
    return true;
}

vector<pair<int, int>> Othello::legalActions(int player)
{
    vector<pair<int, int>> actions;
    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            if (isValidAction(player, i, j))
            {
                actions.push_back({i, j});
            }
        }
    }
    return actions;
}

bool Othello::isValidAction(int player, int y, int x)
{
    // すでに置かれている
    if (board[y][x] >= 0)
    {
        return false;
    }

    for (int dir = 0; dir < 8; ++dir)
    {
        int dy = directions[dir][0];
        int dx = directions[dir][1];
        int ny = y + dy;
        int nx = x + dx;
        // 盤面外
        if (!(ny >= 0 && ny < mapSize && nx >= 0 && nx < mapSize))
        {
            continue;
        }
        // 隣が相手のマスでない
        if ((player ^ board[ny][nx]) != 1)
        {
            continue;
        }
        // 延長線上に自分のマスが存在するか
        ny += dy;
        nx += dx;
        while (ny >= 0 && ny < mapSize && nx >= 0 && nx < mapSize)
        {
            // 延長線上に自分のマスが存在する
            if ((board[ny][nx] ^ player) == 0)
            {
                return true;
            }
            else if ((board[ny][nx] ^ player) == 1)
            {
                ny += dy;
                nx += dx;
                continue;
            }
            // 自分のマスが登場する前に何も置かれていないマスが登場するか
            break;
        }
    }

    return false;
}

string Othello::getWinner()
{
    if (isDone())
    {
        int black = 0;
        for (int i = 0; i < mapSize; ++i)
        {
            black += accumulate(board[i].begin(), board[i].end(), 0);
        }
        int white = mapSize * mapSize - black;
        if (black < white)
        {
            return "White Win";
        }
        else if (black == white)
        {
            return "Draw";
        }
        return "Black Win";
    }
    else
    {
        return "In Progress";
    }
}

void Othello::put(int player, pair<int, int> pos)
{
    board[pos.first][pos.second] = player;

    for (int dir = 0; dir < 8; ++dir)
    {
        int dy = directions[dir][0];
        int dx = directions[dir][1];
        int ny = pos.first + dy;
        int nx = pos.second + dx;
        vector<pair<int, int>> turn_over;

        while (ny >= 0 && ny < mapSize && nx >= 0 && nx < mapSize)
        {
            // ひっくり返すものを入れる
            if ((player ^ board[ny][nx]) == 1)
            {
                turn_over.push_back(make_pair(ny, nx));
                ny += dy;
                nx += dx;
                continue;
            }
            break;
        }
        // ひっくり返す対象の最後が自分のマスであるか
        if (!((ny >= 0 && ny < mapSize && nx >= 0 && nx < mapSize) && board[ny][nx] == player))
        {
            continue;
        }
        // ひっくり返す
        for (const auto &move : turn_over)
        {
            board[move.first][move.second] = player;
        }
    }
}

Othello Othello::copyBoard()
{
    // 現在の盤面をディープコピーして新しい盤面を作成
    Othello copy = *this;
    return copy;
}

pair<int, int> Othello::getCount()
{
    int white = 0;
    int black = 0;
    for (int i = 0; i < mapSize; ++i)
    {
        for (int j = 0; j < mapSize; ++j)
        {
            if (board[i][j] == 1)
            {
                black++;
            }
            else if (board[i][j] == 0)
            {
                white++;
            }
        }
    }
    return make_pair(white, black);
}

string Othello::toString()
{
    stringstream os("");
    os << "Player Information" << endl;

    pair<int, int> mass = getCount();
    int black = mass.second;
    int white = mass.first;

    os << "Black: " << black << endl;
    os << "White: " << white << endl;

    os << "----------------------------" << endl;
    os << "Board Information" << endl;

    auto addSidebar = []() -> string
    {
        stringstream bar("");
        for (int i = 0; i < mapSize; ++i)
        {
            bar << " ";
            bar << "ー";
        }
        return bar.str();
    };
    os << addSidebar() << endl;
    for (int i = 0; i < mapSize; ++i)
    {
        os << "|";
        for (int j = 0; j < mapSize; ++j)
        {
            if (board[i][j] == 1)
            {
                os << "⚫️";
            }
            else if (board[i][j] == 0)
            {
                os << "⚪️";
            }
            else
            {
                os << "　";
            }
            os << "|";
        }
        os << endl;
        os << addSidebar() << endl;
    }
    return os.str();
}

vector<vector<int>> Othello::getBoard()
{
    return board;
}

// コピー代入演算子
Othello &Othello::operator=(const Othello &other)
{
    if (this != &other)
    { // 自己代入をチェック
        // メンバ変数をディープコピーする
        this->board = other.board;
    }
    return *this;
}
