/**
 * Created by HgS_1217_ on 2017/9/6.
 */

class GobangProcessor {

    companion object {
        var boardStatus: Array<Boolean?> = Array(GobangFrame.SIDE_LINE * GobangFrame.SIDE_LINE, { _ -> null })

        fun statusChange(index: Int, status: Boolean) {
            boardStatus[index] = status
        }
    }

    fun reset() {
        boardStatus = Array(GobangFrame.SIDE_LINE * GobangFrame.SIDE_LINE, { _ -> null })
    }

}
