#ifndef EVALUATIONFUNCTION_H
#define EVALUATIONFUNCTION_H

#include "othello.hpp"
#include <vector>  // vectorを使用するためのヘッダーファイル
#include <utility> // pairを使用するためのヘッダーファイル
#include <map>
#include <string>
using namespace std;

enum class Evaluation
{
    MASS_COUNT,
    CUSTOM
};

extern const map<Evaluation, string> evaluationNames;

class EvaluationFunction
{
public:
    // 静的メソッド：マスカウント評価
    static int massCount(Othello &othello, int _id);

    // 静的メソッド：カスタム評価
    static int customEvaluation(Othello &othello, int _id);
};

#endif // EVALUATIONFUNCTION_H
