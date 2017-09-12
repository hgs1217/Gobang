import java.awt.Dimension
import java.awt.Font
import java.awt.Rectangle
import java.awt.event.ActionEvent
import java.awt.event.ActionListener
import java.awt.event.ItemEvent
import java.awt.event.ItemListener
import javax.swing.*

/**
 * Created by HgS_1217_ on 2017/9/11.
 */

class MainFrame : BaseFrame() {

    private val panel = JPanel()
    private val comboboxItem = arrayOf("Human", "AI")
    private val players: Array<Int> = arrayOf(0, 0)

    init {
        panel.layout = null
        setInterface()
        add(panel)
        pack()
    }

    private fun setInterface() {
        val title = JLabel("Gobang", JLabel.CENTER)
        val author = JLabel("by: HgS_1217_", JLabel.CENTER)
        val version = JLabel("v1.00", JLabel.CENTER)
        val black = JLabel("Black", JLabel.CENTER)
        val white = JLabel("White", JLabel.CENTER)
        val player1 = JComboBox<String>()
        val player2 = JComboBox<String>()

        for (tag in comboboxItem) {
            player1.addItem(tag)
            player2.addItem(tag)
        }

        val startButton = JButton("Start")

        val titleFont = Font("Calibri", Font.BOLD, 60)
        val authorFont = Font("Calibri", Font.PLAIN, 20)
        val btnFont = Font("Arial", Font.PLAIN, 25)
        val bwFont = Font("Calibri", Font.BOLD, 40)

        title.font = titleFont
        title.bounds = Rectangle(100, 30, 800, 80)

        author.font = authorFont
        author.bounds = Rectangle(800, 625, 200, 80)

        version.font = btnFont
        version.bounds = Rectangle(650, 60, 200, 80)

        black.font = bwFont
        black.bounds = Rectangle(200, 200, 200, 80)

        white.font = bwFont
        white.bounds = Rectangle(600, 200, 200, 80)

        startButton.font = btnFont
        startButton.bounds = Rectangle(400, 450, 200, 50)
        startButton.addActionListener(SwitchAction())

        player1.font = btnFont
        player1.bounds = Rectangle(200, 300, 200, 50)
        player1.addItemListener(PlayerItemListener(player1, 0))

        player2.font = btnFont
        player2.bounds = Rectangle(600, 300, 200, 50)
        player2.addItemListener(PlayerItemListener(player2, 1))

        panel.add(title)
        panel.add(author)
        panel.add(version)
        panel.add(black)
        panel.add(white)
        panel.add(startButton)
        panel.add(player1)
        panel.add(player2)
    }

    inner class SwitchAction: ActionListener {

        override fun actionPerformed(e: ActionEvent?) {
            GobangProcessor.get().gobangFrameInit(players)
            dispose()
        }
    }

    inner class PlayerItemListener(playerCombobox: JComboBox<String>, playerNum: Int): ItemListener {

        private var combobox: JComboBox<String> = playerCombobox
        private var num: Int = playerNum

        override fun itemStateChanged(e: ItemEvent?) {
            players[num] = combobox.selectedIndex
        }
    }
}