#include "othello.hpp"
#include <vector>  // vectorを使用するためのヘッダーファイル
#include <utility> // pairを使用するためのヘッダーファイル
#include <map>
#include <string>

#include "evaluation.hpp"
using namespace std;

const map<Evaluation, string> evaluationNames = {
    {Evaluation::MASS_COUNT, "mass count"},
    {Evaluation::CUSTOM, "custom"},
};
// 静的メソッド：マスカウント評価
int EvaluationFunction::massCount(Othello &othello, int _id)
{
    // プレイヤーの ID に基づいてベース値を決定
    int _base = (_id == 0) ? 1 : -1;
    // 白と黒のカウントを取得
    pair<int, int> count = othello.getCount();
    // 評価を計算して返す
    return (count.first - count.second) * _base;
}

// 静的メソッド：カスタム評価
int EvaluationFunction::customEvaluation(Othello &othello, int _id)
{
    // ゲームボードの状態を取得
    const vector<vector<int>> &board = othello.getBoard();

    // 各要素の重み付け
    int cornerWeight = 10;
    int edgeWeight = 5;
    int mobilityWeight = 2;
    int parityWeight = 1;

    // 評価値の初期化
    int evaluation = 0;

    // コーナーの評価
    int cornerValue = 0;
    for (int i : {0, othello.mapSize - 1})
    {
        for (int j : {0, othello.mapSize - 1})
        {
            if (board[i][j] == _id)
            {
                cornerValue += 1;
            }
            else if (board[i][j] == (_id ^ 1))
            {
                cornerValue -= 1;
            }
        }
    }
    evaluation += cornerWeight * cornerValue;

    // 辺の評価
    int edgeValue = 0;
    for (int i = 0; i < othello.mapSize; i++)
    {
        for (int j : {0, othello.mapSize - 1})
        {
            // 縦の辺
            if (board[i][j] == _id)
            {
                edgeValue += 1;
            }
            else if (board[i][j] == (_id ^ 1))
            {
                edgeValue -= 1;
            }

            // 横の辺
            if (board[j][i] == _id)
            {
                edgeValue += 1;
            }
            else if (board[j][i] == (_id ^ 1))
            {
                edgeValue -= 1;
            }
        }
    }
    evaluation += edgeWeight * edgeValue;

    // モビリティの評価
    int mobilityValue = othello.legalActions(_id).size() - othello.legalActions(_id ^ 1).size();
    evaluation += mobilityWeight * mobilityValue;

    // パリティの評価
    // プレイヤーの石の数を取得
    pair<int, int> count = othello.getCount();
    int _base = (_id == 1) ? -1 : 1;
    int parityValue = 0;
    if (count.first > count.second)
    {
        parityValue = 1;
    }
    else if (count.first < count.second)
    {
        parityValue = -1;
    }
    evaluation += parityWeight * parityValue * _base;

    // 最終的な評価を返す
    return evaluation;
}
