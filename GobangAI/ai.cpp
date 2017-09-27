#define DLLEXPORT extern "C" __declspec(dllexport)

#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <set>
#include <map>
#include <sstream>

using namespace std;

const int INFI = 9999999;
const int LINE_NUM = 15;
const int DIRECTIONS[8] = {-LINE_NUM, -LINE_NUM + 1, 1, LINE_NUM + 1, LINE_NUM, LINE_NUM - 1, -1, -LINE_NUM - 1};
const int SCORE_LIVE_TWO = 5;
const int SCORE_LIVE_THREE = 50;
const int SCORE_LIVE_FOUR = 1000;
const int SCORE_LIVE_FIVE = 10000;
const int SCORE_DEAD_TWO = 0;
const int SCORE_GAP_TWO = 2;
const int SCORE_DEAD_THREE = 3;
const int SCORE_GAP_THREE = 40;
const int SCORE_DEAD_FOUR = 50;
const int SCORE_GAP_FOUR = 50;
const int SCORE_GAP_FIVE = 50;

const int FILTER_SIZE = 10;
int MAX_EXTEND = 2;
int MAX_DEPTH = 8;

vector<vector<int>> rowArray(0), colArray(0), diagRightUpArray(0), diagRightDownArray(0);
map<string, int> substitutionMap;
int currentBoardScore = 0, lastRound = -1;
int positionValue[LINE_NUM * LINE_NUM] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
        0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0,
        0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0,
        0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0,
        0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0,
        0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0,
        0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0,
        0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0,
        0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0,
        0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0,
        0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

struct ScoreMap {
    int index;
    int score;

    ScoreMap() : index(-1), score(0) {};

    ScoreMap(int i, int s) : index(i), score(s) {};
};

bool sortScoreMapMin(ScoreMap scoreMap1, ScoreMap scoreMap2) {
    return scoreMap1.score < scoreMap2.score;
}

bool sortScoreMapMax(ScoreMap scoreMap1, ScoreMap scoreMap2) {
    return scoreMap1.score > scoreMap2.score;
}

string constructKey(const int boardStatus[], const int &depth, const int &color) {
    stringstream ss;
    ss << depth << color;
    for (int i = 0; i < LINE_NUM * LINE_NUM; ++i) {
        ss << boardStatus[i];
    }
    return ss.str();
}

bool judgeFrontierContinue(int &index, int &dir) {

    int row = index / LINE_NUM, col = index % LINE_NUM;
    return !(col == 0 && (dir + 2 * LINE_NUM) % LINE_NUM == LINE_NUM - 1 ||
             col == LINE_NUM - 1 && (dir + LINE_NUM) % LINE_NUM == 1 ||
             row == 0 && dir <= -LINE_NUM + 1 ||
             row == LINE_NUM - 1 && dir >= LINE_NUM - 1);
}

int judgeModel(const int &cnt, const int &wall, const int &gap, const int &continueCnt, const int &nearWallZero) {

    switch (cnt) {
        case 0:
        case 1:
            return 0;
        case 2:
            if (wall == 0) {
                if (gap == 0) {
                    if (nearWallZero < 2) {
                        return SCORE_LIVE_TWO;
                    } else {
                        return SCORE_DEAD_TWO;
                    }
                } else if (gap == 1) {
                    return SCORE_GAP_TWO;
                }
            } else if (wall == 1 && gap == 0 && nearWallZero == 0) {
                return SCORE_DEAD_TWO;
            }
            break;
        case 3:
            if (wall == 0) {
                if (gap == 0) {
                    if (nearWallZero < 2) {
                        return SCORE_LIVE_THREE;
                    } else {
                        return SCORE_DEAD_THREE;
                    }
                } else if (gap == 1) {
                    return SCORE_GAP_THREE;
                }
            } else if (wall == 1 && nearWallZero == 0) {
                return SCORE_DEAD_THREE;
            }
            break;
        case 4:
            if (wall == 0) {
                if (gap == 0) {
                    return SCORE_LIVE_FOUR;
                } else if (gap == 1) {
                    return SCORE_GAP_FOUR;
                }
            } else if (wall == 1 || wall == 2 && gap == 1) {
                return SCORE_DEAD_FOUR;
            } else if (wall == 2 && gap == 2) {
                return SCORE_DEAD_THREE;
            }
            break;
        default:
            if (gap == 0) {
                return SCORE_LIVE_FIVE;
            } else {
                if (continueCnt == 3) {
                    return SCORE_LIVE_THREE;
                } else if (continueCnt == 4) {
                    return SCORE_LIVE_FOUR;
                } else if (continueCnt >= 5) {
                    return SCORE_LIVE_FIVE;
                }
            }
            return SCORE_GAP_FIVE;
    }
    return 0;
}

int calculateScore(const int boardStatus[], const int &originColor, const vector<vector<int>> &dirArray) {

    int cnt, wall, gap, nearWallZero, tmpColor, conCnt, maxConCnt, score, total = 0;
    bool lastIsBlank, firstIndex;

    for (const vector<int> &line : dirArray) {
        tmpColor = nearWallZero = cnt = gap = wall = conCnt = maxConCnt = 0;
        lastIsBlank = false;
        firstIndex = true;
        for (int index : line) {
            if (index == -1) {
                if (!lastIsBlank) {
                    wall++;
                    lastIsBlank = true;
                } else {
                    nearWallZero++;
                }
                if (tmpColor != 0) {
                    score = judgeModel(cnt, wall, gap, maxConCnt, nearWallZero);
                    total += (tmpColor == originColor ? score : -score);
                }
            } else {
                if (boardStatus[index] == 0) {
                    if (lastIsBlank) {
                        if (tmpColor != 0) {
                            score = judgeModel(cnt, wall, gap, maxConCnt, nearWallZero);
                            total += (tmpColor == originColor ? score : -score);
                            gap = cnt = nearWallZero = tmpColor = conCnt = maxConCnt = 0;
                        } else if (firstIndex) {
                            nearWallZero++;
                        }
                        wall = 0;
                    } else {
                        lastIsBlank = true;
                        if (conCnt > maxConCnt) {
                            maxConCnt = conCnt;
                        }
                        conCnt = 0;
                    }
                } else if (boardStatus[index] == tmpColor) {
                    if (lastIsBlank) {
                        gap++;
                    }
                    cnt++;
                    conCnt++;
                    lastIsBlank = false;
                } else if (boardStatus[index] == -tmpColor) {
                    if (!lastIsBlank) {
                        wall++;
                    } else {
                        nearWallZero++;
                    }
                    if (conCnt > maxConCnt) {
                        maxConCnt = conCnt;
                    }
                    score = judgeModel(cnt, wall, gap, maxConCnt, nearWallZero);
                    total += (tmpColor == originColor ? score : -score);
                    if (!lastIsBlank) {
                        wall = 1;
                        nearWallZero = 0;
                    } else {
                        wall = 0;
                        nearWallZero = 1;
                    }
                    lastIsBlank = false;
                    gap = maxConCnt = 0;
                    cnt = conCnt = 1;
                    tmpColor = -tmpColor;
                } else {
                    tmpColor = boardStatus[index];
                    gap = maxConCnt = 0;
                    cnt = conCnt = 1;
                    lastIsBlank = false;
                }
                firstIndex = false;
            }
        }
    }

    return total;
}

int calculateScoreChange(int boardStatus[], const int &originColor, const int &index) {

    int indexColor = boardStatus[index], originScore, score;

    boardStatus[index] = 0;
    vector<vector<int>> dirArray(0);

    int row = index / LINE_NUM, col = index % LINE_NUM;
    dirArray.emplace_back(rowArray[row]);
    dirArray.emplace_back(colArray[col]);
    if (LINE_NUM - 1 - row + col > 3 && LINE_NUM - 1 - row + col < LINE_NUM * 2 - 5) {
        dirArray.emplace_back(diagRightDownArray[LINE_NUM - 1 - row + col]);
    }
    if (row + col > 3 && row + col < LINE_NUM * 2 - 5) {
        dirArray.emplace_back(diagRightUpArray[row + col]);
    }

    originScore = calculateScore(boardStatus, originColor, dirArray);
    boardStatus[index] = indexColor;
    score = calculateScore(boardStatus, originColor, dirArray);
//            + positionValue[index] * (indexColor == originColor ? 1 : -1);

    return score - originScore;
}

int judgeWin(const int boardStatus[]) {

    int index, status, candidateIndex, lastIndex, chessArray[LINE_NUM * LINE_NUM], chessArraySize = 0;

    for (int i = 0; i < LINE_NUM * LINE_NUM; ++i) {
        if (boardStatus[i] != 0) {
            chessArray[chessArraySize++] = i;
        }
    }

    for (int n = 0; n < chessArraySize; ++n) {
        index = chessArray[n];
        status = boardStatus[index];
        for (int dir : DIRECTIONS) {
            for (int i = 1; i <= 4; ++i) {
                candidateIndex = index + i * dir;
                lastIndex = candidateIndex - dir;
                if (!judgeFrontierContinue(lastIndex, dir)) {
                    break;
                }
                if (boardStatus[candidateIndex] == status) {
                    if (i == 4) {
                        return status;
                    }
                    continue;
                }
                break;
            }
        }
    }
    return 0;
}

int minMax(int boardStatus[], const int &originColor, const int depth, const int &upperScore,
           const int &lastIndex, const int &boardScore, const set<int> &nullIndexSet) {

    int tmp, lastTmp, score, color, index;

    if (depth % 2 == 1) {
        score = INFI;
        color = -originColor;
    } else {
        score = -INFI;
        color = originColor;
    }

    auto subIt = substitutionMap.find(constructKey(boardStatus, depth, color));
    if (subIt != substitutionMap.end()) {
        return subIt->second;
    }

    if (depth == MAX_DEPTH) {
        return boardScore;
    }

    set<int> newNullIndexSet(nullIndexSet);

    if (newNullIndexSet.find(lastIndex) != newNullIndexSet.end()) {
        newNullIndexSet.erase(lastIndex);
    }
    for (int dir : DIRECTIONS) {
        for (int j = 1; j <= MAX_EXTEND; ++j) {
            tmp = lastIndex + j * dir;
            lastTmp = tmp - dir;
            if (!judgeFrontierContinue(lastTmp, dir)) {
                break;
            } else if (boardStatus[tmp] == 0) {
                newNullIndexSet.insert(tmp);
            }
        }
    }

    int tmpStatus[LINE_NUM * LINE_NUM];
    memcpy(tmpStatus, boardStatus, LINE_NUM * LINE_NUM * sizeof(boardStatus[0]));

    vector<ScoreMap> scoreMapArray(0);
    for (int i : newNullIndexSet) {
        tmpStatus[i] = color;
        int winStatus = judgeWin(tmpStatus);
        if (winStatus != 0) {
            return (INFI - depth) * (winStatus == originColor ? 1 : -1);
        }
        int newBoardScore = boardScore + calculateScoreChange(tmpStatus, originColor, i);
        scoreMapArray.emplace_back(ScoreMap(i, newBoardScore));
        tmpStatus[i] = 0;
    }

    if (color == originColor) {
        sort(scoreMapArray.begin(), scoreMapArray.end(), sortScoreMapMax);
    } else {
        sort(scoreMapArray.begin(), scoreMapArray.end(), sortScoreMapMin);
    }

    for (int i = 0; i < min((int) scoreMapArray.size(), FILTER_SIZE); ++i) {
        index = scoreMapArray[i].index;
        tmpStatus[index] = color;
        int downScore = minMax(tmpStatus, originColor, depth + 1, score, index, scoreMapArray[i].score,
                               newNullIndexSet);
//        if (depth == 1) {
//            cout << index << " " << downScore << endl;
//        }
        if (depth % 2 == 1) {
            if (downScore < upperScore) {
                substitutionMap[constructKey(boardStatus, depth, color)] = downScore;
                return downScore;
            } else {
                if (downScore < score) {
                    score = downScore;
                }
            }
        } else {
            if (downScore > upperScore) {
                substitutionMap[constructKey(boardStatus, depth, color)] = downScore;
                return downScore;
            } else {
                if (downScore > score) {
                    score = downScore;
                }
            }
        }
        tmpStatus[index] = 0;
    }

    substitutionMap[constructKey(boardStatus, depth, color)] = score;
    return score;
}

void sliceBoard() {

    int index;

    // construct direction array
    for (int row = 0; row < LINE_NUM; ++row) {
        vector<int> tmpArray(1, -1);
        for (int col = 0; col < LINE_NUM; ++col) {
            index = row * LINE_NUM + col;
            tmpArray.emplace_back(index);
        }
        tmpArray.emplace_back(-1);
        rowArray.emplace_back(tmpArray);
    }

    for (int col = 0; col < LINE_NUM; ++col) {
        vector<int> tmpArray(1, -1);
        for (int row = 0; row < LINE_NUM; ++row) {
            index = row * LINE_NUM + col;
            tmpArray.emplace_back(index);
        }
        tmpArray.emplace_back(-1);
        colArray.emplace_back(tmpArray);
    }

    for (int row = LINE_NUM - 1; row >= 0; --row) {
        vector<int> tmpArray(1, -1);
        for (int offset = 0; offset < LINE_NUM - row; ++offset) {
            index = (row + offset) * LINE_NUM + offset;
            tmpArray.emplace_back(index);
        }
        tmpArray.emplace_back(-1);
        diagRightDownArray.emplace_back(tmpArray);
    }

    for (int row = 0; row < LINE_NUM; ++row) {
        vector<int> tmpArray(1, -1);
        for (int offset = 0; offset <= row; ++offset) {
            index = (row - offset) * LINE_NUM + offset;
            tmpArray.emplace_back(index);
        }
        tmpArray.emplace_back(-1);
        diagRightUpArray.emplace_back(tmpArray);
    }

    for (int col = 1; col < LINE_NUM; ++col) {
        vector<int> tmpDownArray(1, -1), tmpUpArray(1, -1);
        for (int offset = 0; offset < LINE_NUM - col; ++offset) {
            index = offset * LINE_NUM + (col + offset);
            tmpDownArray.emplace_back(index);
        }
        tmpDownArray.emplace_back(-1);
        diagRightDownArray.emplace_back(tmpDownArray);
        for (int offset = 0; offset < LINE_NUM - col; ++offset) {
            index = (LINE_NUM - offset - 1) * LINE_NUM + (col + offset);
            tmpUpArray.emplace_back(index);
        }
        tmpUpArray.emplace_back(-1);
        diagRightUpArray.emplace_back(tmpUpArray);
    }
}

DLLEXPORT void __stdcall init() {
    rowArray.clear();
    colArray.clear();
    diagRightUpArray.clear();
    diagRightDownArray.clear();
}

DLLEXPORT int __stdcall call(int statuses[], int round, int lastIndex, int level) {

    switch (level) {
        case 1: MAX_DEPTH = 2;  MAX_EXTEND = 1;  break;
        case 2: MAX_DEPTH = 4;  MAX_EXTEND = 1;  break;
        case 3: MAX_DEPTH = 6;  MAX_EXTEND = 1;  break;
        case 4: MAX_DEPTH = 8; MAX_EXTEND = 2;  break;
        default:    MAX_DEPTH = 1;  break;
    }

    int boardStatus[LINE_NUM * LINE_NUM];
    memcpy(boardStatus, statuses, LINE_NUM * LINE_NUM * sizeof(statuses[0]));

    set<int> nullIndex;
    int tmp, lastTmp, index, score = -INFI, color = round % 2 == 1 ? 1 : -1;

    default_random_engine eng;
    eng.seed((unsigned) time(nullptr));
    substitutionMap.clear();

    if (rowArray.empty()) {
        sliceBoard();
    }

    if (round - lastRound != 1 && lastIndex >= 0) {
        currentBoardScore += calculateScoreChange(boardStatus, color, lastIndex);
    }

    for (int i = 0; i < LINE_NUM * LINE_NUM; ++i) {
        if (boardStatus[i] != 0) {
            for (int dir : DIRECTIONS) {
                for (int j = 1; j <= MAX_EXTEND; ++j) {
                    tmp = i + j * dir;
                    lastTmp = tmp - dir;
                    if (!judgeFrontierContinue(lastTmp, dir)) {
                        break;
                    } else if (boardStatus[tmp] == 0) {
                        nullIndex.insert(tmp);
                    }
                }
            }
        }
    }

    if (round == 1) {
        return LINE_NUM * LINE_NUM / 2;
    } else if (round == 2) {
        return lastIndex + DIRECTIONS[eng() % 8];
    }

    vector<ScoreMap> scoreMapArray(0), resultMapArray(0);
    vector<int> resultIndex(0);
    for (int i : nullIndex) {
        boardStatus[i] = color;
        int winStatus = judgeWin(boardStatus);
        if (winStatus == color) {
            return i;
        }
        int newBoardScore = currentBoardScore + calculateScoreChange(boardStatus, color, i);
        scoreMapArray.emplace_back(ScoreMap(i, newBoardScore));
        boardStatus[i] = 0;
    }

    sort(scoreMapArray.begin(), scoreMapArray.end(), sortScoreMapMax);

//    cout << endl;
//    for (auto map : scoreMapArray) {
//        cout << map.index << " " << map.score << endl;
//    }

    for (int i = 0; i < min((int) scoreMapArray.size(), FILTER_SIZE); ++i) {
        index = scoreMapArray[i].index;
        boardStatus[index] = color;
        int s = minMax(boardStatus, color, 1, score, index, scoreMapArray[i].score, nullIndex);
//        cout << endl << index << " " << s << endl << endl;
        if (s > score) {
            score = s;
            resultIndex.clear();
            resultIndex.emplace_back(index);
        } else if (s == score) {
            resultIndex.emplace_back(index);
        }
        if (round <= 4) {
            resultMapArray.emplace_back(ScoreMap(index, s));
        }
        boardStatus[index] = 0;
    }

    int result;
    if (round <= 4) {
        sort(resultMapArray.begin(), resultMapArray.end(), sortScoreMapMax);
        result = resultMapArray[eng() % 5].index;
    } else {
        result = resultIndex[eng() % resultIndex.size()];
    }

    boardStatus[result] = color;
    currentBoardScore += calculateScoreChange(boardStatus, color, result);
    lastRound = round;
    return result;
}
