import java.awt.Dimension
import java.awt.Font
import java.awt.Rectangle
import java.awt.event.ActionEvent
import java.awt.event.ActionListener
import javax.swing.JButton
import javax.swing.JLabel
import javax.swing.JPanel

/**
 * Created by HgS_1217_ on 2017/9/11.
 */

class SelectFrame(plrs: Array<Int>): BaseFrame() {

    private val panel = JPanel()
    private val players: Array<Int> = plrs

    init {
        panel.layout = null
        setInterface()
        add(panel)
        pack()
    }

    private fun setInterface() {
        panel.layout = null

        val title = JLabel("P1 (" + GobangProcessor.playerName[0] + ") Choose a color", JLabel.CENTER)
        val blackButton = JButton("Black")
        val whiteButton = JButton("White")

        val titleFont = Font("Calibri", Font.BOLD, 50)
        val btnFont = Font("Arial", Font.PLAIN, 30)

        title.font = titleFont
        title.bounds = Rectangle(150, 60, 600, 80)

        blackButton.font = btnFont
        blackButton.bounds = Rectangle(350, 200, 200, 60)
        blackButton.addActionListener(GameAction(true))

        whiteButton.font = btnFont
        whiteButton.bounds = Rectangle(350, 300, 200, 60)
        whiteButton.addActionListener(GameAction(false))

        panel.add(title)
        panel.add(blackButton)
        panel.add(whiteButton)
        add(panel)
    }

    inner class GameAction(statusBlack: Boolean): ActionListener {

        private val status: Boolean = statusBlack

        private fun exchangeData() {
            val black = players[0]
            val white = players[1]
            if (!status) {
                players[1] = black
                players[0] = white
            }
        }

        override fun actionPerformed(e: ActionEvent?) {
            exchangeData()
            GobangProcessor.get().gobangFrameInit(players)
            dispose()
        }
    }
}
