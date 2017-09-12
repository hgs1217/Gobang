/**
 * Created by HgS_1217_ on 2017/9/11.
 */

class GobangRecorder private constructor() {

    companion object {

        var namesP1: Array<String> = arrayOf("", "", "", "", "")
        var namesP2: Array<String> = arrayOf("", "", "", "", "")
        var resultsP1: Array<String> = arrayOf("", "", "", "", "")
        var resultsP2: Array<String> = arrayOf("", "", "", "", "")

        fun get(): GobangRecorder {
            return innerObj.recorder
        }
    }

    private object innerObj {
        val recorder = GobangRecorder()
    }

    fun insertNew(playerName: Array<String>, round: Int) {
        for (i in 0..4) {
            if (namesP1[i].isEmpty()) {
                namesP1[i] = playerName[0]
                namesP2[i] = playerName[1]
                if (round % 2 == 1) {
                    resultsP1[i] = "${round}W"
                } else {
                    resultsP2[i] = "${round}W"
                }
                return
            }
        }

        for (i in 0..3) {
            namesP1[i] = namesP1[i+1]
            namesP2[i] = namesP2[i+1]
            resultsP1[i] = resultsP1[i+1]
            resultsP2[i] = resultsP2[i+1]
        }
        namesP1[4] = playerName[0]
        namesP2[4] = playerName[1]
        if (round % 2 == 1) {
            resultsP1[4] = "${round}W"
            resultsP2[4] = ""
        } else {
            resultsP1[4] = ""
            resultsP2[4] = "${round}W"
        }
    }

    fun reset() {
        namesP1 = arrayOf("", "", "", "", "")
        namesP2 = arrayOf("", "", "", "", "")
        resultsP1 = arrayOf("", "", "", "", "")
        resultsP2 = arrayOf("", "", "", "", "")
    }
}