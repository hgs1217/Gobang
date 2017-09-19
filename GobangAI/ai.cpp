// g++ cmd: g++ -std=c++11 -m64 -o ai.dll -shared -fPIC main.cpp
#define DLLEXPORT extern "C" __declspec(dllexport)

#include <random>
#include <ctime>
#include <set>

using namespace std;

const int LINE_NUM = 15;
const int DIRECTIONS[8] = {-LINE_NUM - 1, -LINE_NUM, -LINE_NUM + 1, -1, 1, LINE_NUM - 1, LINE_NUM, LINE_NUM + 1};

DLLEXPORT int __stdcall call(int statuses[], int index, int level) {

    vector<int> boardStatus(&statuses[0], &statuses[LINE_NUM * LINE_NUM]);
    set<int> nullIndex;

    default_random_engine eng;
    eng.seed((unsigned) time(nullptr));

    int tmp, lastTmp, lastRow, lastCol;
    for (int i = 0; i < boardStatus.size(); ++i) {
        if (boardStatus[i] != 0) {
            for (int dir : DIRECTIONS) {
                for (int j = 1; j <= 2; ++j) {
                    tmp = i + j * dir;
                    lastTmp = tmp - dir;
                    lastRow = lastTmp / LINE_NUM;
                    lastCol = lastTmp % LINE_NUM;
                    if (lastCol == 0 && (dir + 2 * LINE_NUM) % LINE_NUM == LINE_NUM - 1 ||
                        lastCol == LINE_NUM - 1 && (dir + LINE_NUM) % LINE_NUM == 1 ||
                        lastRow == 0 && dir <= -LINE_NUM + 1 ||
                        lastRow == LINE_NUM - 1 && dir >= LINE_NUM - 1) {
                        break;
                    }
                    if (boardStatus[tmp] == 0) {
                        nullIndex.insert(tmp);
                    }
                }
            }
        }
    }

    if (nullIndex.empty()) {
        return LINE_NUM * LINE_NUM / 2;
    }

    auto it = nullIndex.begin();
    auto ran = (int) (eng() % nullIndex.size());
    for (int i = 0; i < ran; ++i) {
        it++;
    }

    return *it;
}
