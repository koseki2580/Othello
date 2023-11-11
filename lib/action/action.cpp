
#include <utility>  // pairを使用するためのヘッダーファイル
#include <optional> // optionalを使用するためのヘッダーファイル
#include <map>

#include "../othello.hpp"
#include "action.hpp"
using namespace std;

const map<Strategy, string> strategyNames = {
    {Strategy::RANDOM, "random"},
    {Strategy::MINIMAX, "minimax"},
};
Action::Action(int value) : id(value)
{
}
