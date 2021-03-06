#define DLLEXPORT extern "C" __declspec(dllexport)

#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <set>
#include <map>
#include <cstring>
#include <sstream>
#include <thread>
#include <mutex>

using namespace std;

const double PI = 3.1415926;
const int INFI = 9999999;
const int LINE_NUM = 15;
const int DIRECTIONS[8] = {-LINE_NUM, -LINE_NUM + 1, 1, LINE_NUM + 1, LINE_NUM, LINE_NUM - 1, -1, -LINE_NUM - 1};
const int SCORE_LIVE_TWO = 5;
const int SCORE_LIVE_THREE = 50;
const int SCORE_LIVE_FOUR = 1000;
const int SCORE_LIVE_FIVE = 10000;
const int SCORE_DEAD_TWO = 0;
const int SCORE_GAP_TWO = 2;
const int SCORE_DEAD_THREE = 5;
const int SCORE_GAP_THREE = 40;
const int SCORE_DEAD_FOUR = 50;
const int SCORE_GAP_FOUR = 40;
const int SCORE_GAP_FIVE = 40;
const double SCORE_HISTORY_RATIO = 0.01;

const int THREAD_NUM = 3;
const int MAX_FILTER_SIZE = 15;
const int FILTER_SIZE = 10;
int MAX_EXTEND = 2;
int MAX_DEPTH = 8;
double WIN_RATE = 50;

struct ScoreMap {
    int index;
    int score;

    ScoreMap() : index(-1), score(0) {};

    ScoreMap(int i, int s) : index(i), score(s) {};
};

vector<vector<int>> rowArray(0), colArray(0), diagRightUpArray(0), diagRightDownArray(0),
        orderIndexs(LINE_NUM * LINE_NUM, vector<int>(0));
vector<ScoreMap> resultMapArray(0);
int currentBoardScore = 0, lastRound = -1, currentRound = 1;
mutex resultMutex;
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
           const int &lastIndex, const int &boardScore, const set<int> &nullIndexSet,
           map<string, int> &substitutionMap, map<string, vector<ScoreMap>> &substitutionScoreMap) {

    int tmp, lastTmp, score, color, index, filter = depth <= 1 ? MAX_FILTER_SIZE : FILTER_SIZE;

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

    string key = constructKey(boardStatus, 0, color);
    auto scoreIt = substitutionScoreMap.find(key);
    if (scoreIt != substitutionScoreMap.end()) {
        scoreMapArray = scoreIt->second;
    } else {
        for (int i : newNullIndexSet) {
            tmpStatus[i] = color;
            int winStatus = judgeWin(tmpStatus), newBoardScore;
            if (winStatus != 0) {
                return (INFI - depth) * (winStatus == originColor ? 1 : -1);
            }
            newBoardScore = boardScore + calculateScoreChange(tmpStatus, originColor, i) +
                            (int) (orderIndexs[currentRound + depth][i] * SCORE_HISTORY_RATIO);
            scoreMapArray.emplace_back(ScoreMap(i, newBoardScore));
            tmpStatus[i] = 0;
        }

        if (color == originColor) {
            sort(scoreMapArray.begin(), scoreMapArray.end(), sortScoreMapMax);
        } else {
            sort(scoreMapArray.begin(), scoreMapArray.end(), sortScoreMapMin);
        }

        substitutionScoreMap[key] = scoreMapArray;
    }

    for (int i = 0; i < min((int) scoreMapArray.size(), filter); ++i) {
        index = scoreMapArray[i].index;
        tmpStatus[index] = color;
        int downScore = minMax(tmpStatus, originColor, depth + 1, score, index, scoreMapArray[i].score,
                               newNullIndexSet, substitutionMap, substitutionScoreMap);
//        if (depth == 1) {
//            cout << index << " " << downScore << endl;
//        }
        if (depth % 2 == 1) {
            if (downScore < upperScore) {
                substitutionMap[constructKey(boardStatus, depth, color)] = downScore;
                resultMutex.lock();
                orderIndexs[currentRound + depth][index]++;
                resultMutex.unlock();
                return downScore;
            } else {
                if (downScore < score) {
                    score = downScore;
                }
            }
        } else {
            if (downScore > upperScore) {
                substitutionMap[constructKey(boardStatus, depth, color)] = downScore;
                resultMutex.lock();
                orderIndexs[currentRound + depth][index]++;
                resultMutex.unlock();
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

    for (auto &od : orderIndexs) {
        for (int i = 0; i < LINE_NUM * LINE_NUM; ++i) {
            od.emplace_back(0);
        }
    }
}

void threadFunc(const int statuses[], const vector<ScoreMap> &scoreMapArray, const set<int> &nullIndex) {

    int index, color = currentRound % 2 == 1 ? 1 : -1, boardStatus[LINE_NUM * LINE_NUM], score = -INFI;
    memcpy(boardStatus, statuses, LINE_NUM * LINE_NUM * sizeof(statuses[0]));

    vector<ScoreMap> rmArray(0);
    map<string, int> substitutionMap;
    map<string, vector<ScoreMap>> substitutionScoreMap;

    for (auto &sm : scoreMapArray) {
        index = sm.index;
        boardStatus[index] = color;
        int s = minMax(boardStatus, color, 1, score, index, sm.score, nullIndex, substitutionMap, substitutionScoreMap);
//        cout << endl << index << " " << s << endl << endl;
        if (currentRound <= 4) {
            rmArray.emplace_back(ScoreMap(index, s));
        } else {
            if (s > score) {
                score = s;
                rmArray.clear();
                rmArray.emplace_back(ScoreMap(index, s));
            } else if (s == score) {
                rmArray.emplace_back(ScoreMap(index, s));
            }
        }
        boardStatus[index] = 0;
    }

    resultMutex.lock();
    for (auto &scoreMap : rmArray) {
        resultMapArray.emplace_back(scoreMap);
    }
    resultMutex.unlock();
}

void setDepth(int level) {
    switch (level) {
        case 1:
            MAX_DEPTH = 2;
            MAX_EXTEND = 1;
            break;
        case 2:
            MAX_DEPTH = 4;
            MAX_EXTEND = 1;
            break;
        case 3:
            MAX_DEPTH = 6;
            MAX_EXTEND = 1;
            break;
        case 4:
            MAX_DEPTH = 8;
            MAX_EXTEND = 2;
            break;
        default:
            MAX_DEPTH = 1;
            break;
    }
}

void updateWinRate(double score) {
    WIN_RATE = 100.0 / PI * atan((score + 6 + currentRound / 1.5) / 100.0) + 50;
}

DLLEXPORT double __cdecl getWinRate() {
    return WIN_RATE;
}

DLLEXPORT void __cdecl init() {
    rowArray.clear();
    colArray.clear();
    diagRightUpArray.clear();
    diagRightDownArray.clear();
    currentBoardScore = 0;
    for (auto &od : orderIndexs) {
        od.clear();
    }
}

DLLEXPORT int __cdecl call(int statuses[], int round, int lastIndex, int level) {

    int boardStatus[LINE_NUM * LINE_NUM];
    memcpy(boardStatus, statuses, LINE_NUM * LINE_NUM * sizeof(statuses[0]));

    set<int> nullIndex;
    int tmp, lastTmp, index, result, score = -INFI, color = round % 2 == 1 ? 1 : -1;

    default_random_engine eng;
    eng.seed((unsigned) time(nullptr));
    resultMapArray.clear();
    currentRound = round;

    if (rowArray.empty()) {
        setDepth(level);
        sliceBoard();
        WIN_RATE = 50;
    }

    if (round - lastRound != 1 && lastIndex >= 0) {
        currentBoardScore += calculateScoreChange(boardStatus, color, lastIndex);
    }

    if (round <= 4) {
        MAX_DEPTH = min(MAX_DEPTH, 4);
        MAX_EXTEND = 1;
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

    vector<ScoreMap> scoreMapArray(0);
    vector<int> resultIndex(0);

    for (int i : nullIndex) {
        boardStatus[i] = color;
        int winStatus = judgeWin(boardStatus), newBoardScore;
        if (winStatus == color) {
            return i;
        }
        newBoardScore = currentBoardScore + calculateScoreChange(boardStatus, color, i) +
                        (int) (orderIndexs[round][i] * SCORE_HISTORY_RATIO);
        scoreMapArray.emplace_back(ScoreMap(i, newBoardScore));
        boardStatus[i] = 0;
    }

    sort(scoreMapArray.begin(), scoreMapArray.end(), sortScoreMapMax);

//    cout << endl;
//    for (auto map : scoreMapArray) {
//        cout << map.index << " " << map.score << endl;
//    }

    if (round <= 4) {
        for (int i = 0; i < min((int) scoreMapArray.size(), MAX_FILTER_SIZE); ++i) {
            index = scoreMapArray[i].index;
            boardStatus[index] = color;
            map<string, int> substitutionMap;
            map<string, vector<ScoreMap>> substitutionScoreMap;
            int s = minMax(boardStatus, color, 1, score, index, scoreMapArray[i].score, nullIndex,
                            substitutionMap, substitutionScoreMap);
//            cout << endl << index << " " << s << endl << endl;
            if (s > score) {
                score = s;
                resultIndex.clear();
                resultIndex.emplace_back(index);
            } else if (s == score) {
                resultIndex.emplace_back(index);
            }
            resultMapArray.emplace_back(ScoreMap(index, s));
            boardStatus[index] = 0;
        }

        sort(resultMapArray.begin(), resultMapArray.end(), sortScoreMapMax);
        ScoreMap sm;
        if (round == 3) {
            sm = resultMapArray[eng() % 8];
        } else {
            sm = resultMapArray[eng() % 3];
        }
        result = sm.index;
        updateWinRate(sm.score);
        setDepth(level);
    } else {
        vector<vector<ScoreMap>> smArrays(0);
        for (int i=0; i<THREAD_NUM; ++i) {
            vector<ScoreMap> smArray(0);
            for (int j = 0; j < MAX_FILTER_SIZE / THREAD_NUM; ++j) {
                smArray.emplace_back(scoreMapArray[j * THREAD_NUM + i]);
            }
            smArrays.emplace_back(smArray);
        }
        thread th0(threadFunc, boardStatus, smArrays[0], nullIndex);
        thread th1(threadFunc, boardStatus, smArrays[1], nullIndex);
        thread th2(threadFunc, boardStatus, smArrays[2], nullIndex);
        th0.join();
        th1.join();
        th2.join();

        sort(resultMapArray.begin(), resultMapArray.end(), sortScoreMapMax);
        score = resultMapArray[0].score;
        for (auto &sm : resultMapArray) {
            if (sm.score == score) {
                resultIndex.emplace_back(sm.index);
            } else {
                break;
            }
        }
        result = resultIndex[eng() % resultIndex.size()];
        updateWinRate(score);
    }

    boardStatus[result] = color;
    currentBoardScore += calculateScoreChange(boardStatus, color, result);
    lastRound = round;
    return result;
}
