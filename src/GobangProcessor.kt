import java.awt.Font
import java.awt.Rectangle
import java.awt.event.ActionEvent
import java.awt.event.ActionListener
import java.awt.event.MouseAdapter
import java.awt.event.MouseEvent
import javax.swing.JButton

/**
 * Created by HgS_1217_ on 2017/9/6.
 */

class GobangProcessor private constructor() {

    companion object {

        var boardStatus: Array<Boolean?> = Array(GobangConfig.SIDE_LINE * GobangConfig.SIDE_LINE, { _ -> null })
        var round = 1
        var isGameOver = false
        var players: Array<Int> = arrayOf(0, 0)
        val scores: Array<Int> = arrayOf(0, 0)
        var currentIndex = -1
        val playerName: Array<String> = arrayOf("", "")
        val aiName: Array<String> = arrayOf("", "")
        val btnFont = Font("Arial", Font.PLAIN, 20)

        fun get(): GobangProcessor {
            return innerObj.processor
        }
    }

    private var mainFrame: MainFrame = MainFrame()
    private var selectFrame: SelectFrame? = null
    private var frame: GobangFrame? = null
    private var panel: GobangPanel? = null
    private val rule: GobangRule = GobangRule.get()
    private val recorder: GobangRecorder = GobangRecorder.get()

    private object innerObj {
        val processor = GobangProcessor()
    }

    private fun restart() {
        boardStatus = Array(GobangConfig.SIDE_LINE * GobangConfig.SIDE_LINE, { _ -> null })
        round = 1
        currentIndex = -1
        isGameOver = false
    }

    private fun btnInit() {
        val restartBtn = JButton("Restart")
        val backBtn = JButton("Back")

        restartBtn.font = btnFont
        restartBtn.bounds = Rectangle(700, 630, 120, 40)
        restartBtn.addActionListener(RestartAction())

        backBtn.font = btnFont
        backBtn.bounds = Rectangle(850, 630, 120, 40)
        backBtn.addActionListener(BackAction())

        panel?.add(restartBtn)
        panel?.add(backBtn)
    }

    private fun setName() {
        if (players[0] == 0 && players[1] == 0) {
            playerName[0] = "PLR1"
            playerName[1] = "PLR2"
        } else if (players[0] == 0 && players[1] != 0) {
            playerName[0] = "YOU"
            playerName[1] = "COM"
            aiName[1] = "(AI LV${players[1]})"
        } else if (players[0] != 0 && players[1] == 0) {
            playerName[0] = "COM"
            playerName[1] = "YOU"
            aiName[0] = "(AI LV${players[0]})"
        } else {
            playerName[0] = "COM1"
            playerName[1] = "COM2"
            aiName[0] = "(AI LV${players[0]})"
            aiName[1] = "(AI LV${players[1]})"
        }
    }

    private fun currentTurn() {
        if (boardStatus[currentIndex] == null) {
            boardStatus[currentIndex] = round % 2 == 1
            if (rule.judgeWin(boardStatus)) {
                isGameOver = true
                scores[(round-1) % 2]++
                recorder.insertNew(playerName, round)
            } else {
                round++
            }
            panel?.repaint()
        }
    }

    fun gobangFrameInit(plrs: Array<Int>) {
        players = plrs
        restart()

        frame = GobangFrame()
        panel = GobangPanel()
        panel?.addMouseListener(MouseHandler())
        frame?.add(panel)
        frame?.pack()

        btnInit()
        setName()
    }

    inner class MouseHandler: MouseAdapter() {

        override fun mousePressed(e: MouseEvent?) {
            super.mousePressed(e)

            if (isGameOver) return

            if (e?.clickCount == 1 && GobangConfig.EFFECTIVE_REGION.contains(e.x, e.y)) {
                val row = Math.floor((e.y - GobangConfig.START_Y - GobangConfig.RECT_WIDTH / 2.0) /
                        GobangConfig.RECT_WIDTH).toInt() + 1
                val col = Math.floor((e.x - GobangConfig.START_X - GobangConfig.RECT_WIDTH / 2.0) /
                        GobangConfig.RECT_WIDTH).toInt() + 1
                currentIndex = row * GobangConfig.SIDE_LINE + col
                currentTurn()
            }
        }
    }

    inner class RestartAction: ActionListener {

        override fun actionPerformed(e: ActionEvent?) {
            frame?.dispose()
            selectFrame = SelectFrame(players)
        }
    }

    inner class BackAction: ActionListener {

        override fun actionPerformed(e: ActionEvent?) {
            frame?.dispose()
            recorder.reset()
            mainFrame = MainFrame()
        }
    }
}
