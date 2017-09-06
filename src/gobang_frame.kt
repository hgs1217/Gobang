import java.awt.*
import java.awt.event.MouseEvent
import java.awt.event.MouseListener
import java.awt.geom.Ellipse2D
import java.awt.geom.Rectangle2D
import javax.swing.JFrame

/**
 * Created by HgS_1217_ on 2017/9/6.
 */

class GobangFrame : JFrame(), MouseListener {

    companion object {
        val WINDOW_WIDTH = 1200
        val WINDOW_HEIGHT = 900
        val START_X = 50
        val START_Y = 50
        val RECT_WIDTH = 50
        val SIDE_LINE = 15
        val POINT_RADIUS = 5
        val END_X = START_X + (SIDE_LINE - 1) * RECT_WIDTH
        val END_Y = START_Y + (SIDE_LINE - 1) * RECT_WIDTH
        val EFFECTIVE_REGION = Rectangle(START_X - RECT_WIDTH / 2, START_Y - RECT_WIDTH / 2,
                RECT_WIDTH * SIDE_LINE, RECT_WIDTH * SIDE_LINE)
        val CHESS_ARRAY: Array<Ellipse2D.Double> = Array(SIDE_LINE * SIDE_LINE, { index -> getEllipse(index)})

        private fun getEllipse(index: Int): Ellipse2D.Double {
            val offsetX: Int = START_X - RECT_WIDTH / 2
            val offsetY: Int = START_Y - RECT_WIDTH / 2
            val row: Int = index / SIDE_LINE
            val col: Int = index % SIDE_LINE
            return Ellipse2D.Double(offsetX.toDouble() + col * RECT_WIDTH, offsetY.toDouble() + row * RECT_WIDTH, RECT_WIDTH.toDouble(), RECT_WIDTH.toDouble())
        }
    }

    init {
        val pane = contentPane
        pane.background = Color.WHITE
        title = "Gobang"
        setBounds(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)
        isVisible = true
        layout = null
        isResizable = false
        defaultCloseOperation = JFrame.EXIT_ON_CLOSE

        Thread.sleep(100)
        paintComponents(graphics)
    }

    override fun paintComponents(g: Graphics) {
        super.paintComponents(g)

        val g2 = g as Graphics2D
        g2.stroke = BasicStroke(3.0f)
        g2.color = Color.BLACK
        for (row in 0..SIDE_LINE - 1) {
            g2.drawLine(START_X, START_Y + row * RECT_WIDTH, END_X, START_Y + row * RECT_WIDTH)
        }
        for (col in 0..SIDE_LINE - 1) {
            g2.drawLine(START_X + col * RECT_WIDTH, START_Y, START_X + col * RECT_WIDTH, END_Y)
        }

        val pointList = arrayOf(arrayOf(3, 3), arrayOf(3, 11), arrayOf(7, 7), arrayOf(11, 3), arrayOf(11, 11))
        for ((row, col) in pointList) {
            val x = START_X + col * RECT_WIDTH - POINT_RADIUS
            val y = START_Y + row * RECT_WIDTH - POINT_RADIUS
            g2.fillOval(x, y, 2 * POINT_RADIUS, 2 * POINT_RADIUS)
        }

        for ((index, status) in GobangProcessor.boardStatus.withIndex()) {
            when(status) {
                true    ->  g2.fill(CHESS_ARRAY[index])
                false   ->  g2.draw(CHESS_ARRAY[index])
            }
        }
    }

    override fun mouseClicked(e: MouseEvent?) {
        println("$e.x  $e.y")
        if (e?.clickCount == 1 && EFFECTIVE_REGION.contains(e.x, e.y)) {
            println(GobangProcessor.boardStatus)
            val row: Int = Math.floor((e.y - POINT_RADIUS / 2.0) / POINT_RADIUS).toInt()
            val col: Int = Math.floor((e.x - POINT_RADIUS / 2.0) / POINT_RADIUS).toInt()
            val index = row * SIDE_LINE + col
            if (GobangProcessor.boardStatus[index] == null) {
                GobangProcessor.statusChange(index, true)
                repaint()
            }
        }
    }

    override fun mouseReleased(e: MouseEvent?) {}

    override fun mouseEntered(e: MouseEvent?) {}

    override fun mouseExited(e: MouseEvent?) {}

    override fun mousePressed(e: MouseEvent?) {}
}
