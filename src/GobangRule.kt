/**
 * Created by HgS_1217_ on 2017/9/11.
 */

class GobangRule private constructor() {

    companion object {

        fun get(): GobangRule {
            return innerObj.gobangRule
        }
    }

    private object innerObj {
        val gobangRule = GobangRule()
    }

    fun judgeWin (boardStatus: Array<Boolean?>): Boolean {

        val chessArray: MutableSet<Int> = HashSet()
        for ((index, status) in boardStatus.withIndex()) {
            if (status != null) {
                chessArray.add(index)
            }
        }

        var candidateIndex: Int
        var status: Boolean?
        for (index in chessArray) {
            status = boardStatus[index]
            for (dir in GobangConfig.DIRECTION_ARRAY) {
                for (i in 1..4) {
                    candidateIndex = index + i * dir
                    if (candidateIndex >= 0 && candidateIndex < boardStatus.size && boardStatus[candidateIndex] == status) {
                        if (i == 4) {
                            return true
                        }
                        continue
                    }
                    break
                }
            }
        }
        return false
    }
}
