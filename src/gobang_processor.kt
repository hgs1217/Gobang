/**
 * Created by HgS_1217_ on 2017/9/6.
 */

class GobangProcessor {

    companion object {
        var boardStatus: Array<Boolean?> = Array(GobangFrame.SIDE_LINE * GobangFrame.SIDE_LINE, { _ -> null })
        var currentStatus = true

        fun statusChange(index: Int) {
            boardStatus[index] = currentStatus
            currentStatus = !currentStatus
        }
    }

    fun reset() {
        boardStatus = Array(GobangFrame.SIDE_LINE * GobangFrame.SIDE_LINE, { _ -> null })
    }

}
