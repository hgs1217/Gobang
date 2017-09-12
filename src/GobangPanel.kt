import java.awt.*
import java.awt.geom.Ellipse2D
import javax.swing.JPanel

/**
 * Created by HgS_1217_ on 2017/9/10.
 */

class GobangPanel : JPanel() {

    private val chessArray: Array<Ellipse2D.Double> =
            Array(Math.pow(GobangConfig.SIDE_LINE.toDouble(), 2.0).toInt(), { index -> getEllipse(index)})
    private val chessBorderArray: Array<Ellipse2D.Double> =
            Array(Math.pow(GobangConfig.SIDE_LINE.toDouble(), 2.0).toInt(), { index -> getBorderEllipse(index)})
    private val nameFont = Font("Arial", Font.BOLD, 30)
    private val subnameFont = Font("Arial", Font.PLAIN, 18)
    private val winnerFont = Font("Calibri", Font.ITALIC, 40)

    init {
        layout = null
    }

    private fun getEllipse(index: Int): Ellipse2D.Double {
        val offsetX: Int = GobangConfig.START_X - GobangConfig.RECT_WIDTH / 2
        val offsetY: Int = GobangConfig.START_Y - GobangConfig.RECT_WIDTH / 2
        val row: Int = index / GobangConfig.SIDE_LINE
        val col: Int = index % GobangConfig.SIDE_LINE

        return Ellipse2D.Double(offsetX.toDouble() + col * GobangConfig.RECT_WIDTH + GobangConfig.SIZE_OFFSET,
                offsetY.toDouble() + row * GobangConfig.RECT_WIDTH + GobangConfig.SIZE_OFFSET,
                GobangConfig.RECT_WIDTH.toDouble() - 2 * GobangConfig.SIZE_OFFSET,
                GobangConfig.RECT_WIDTH.toDouble() - 2 * GobangConfig.SIZE_OFFSET)
    }

    private fun getBorderEllipse(index: Int): Ellipse2D.Double {
        val offsetX: Int = GobangConfig.START_X - GobangConfig.RECT_WIDTH / 2
        val offsetY: Int = GobangConfig.START_Y - GobangConfig.RECT_WIDTH / 2
        val row: Int = index / GobangConfig.SIDE_LINE
        val col: Int = index % GobangConfig.SIDE_LINE
        return Ellipse2D.Double(offsetX.toDouble() + col * GobangConfig.RECT_WIDTH + GobangConfig.BORDER_OFFSET,
                offsetY.toDouble() + row * GobangConfig.RECT_WIDTH + GobangConfig.BORDER_OFFSET,
                GobangConfig.RECT_WIDTH.toDouble() - 2 * GobangConfig.BORDER_OFFSET,
                GobangConfig.RECT_WIDTH.toDouble() - 2 * GobangConfig.BORDER_OFFSET)
    }

    private fun drawPlayerName(g2: Graphics2D) {

        g2.font = subnameFont
        g2.color = Color.BLACK
        g2.drawString(GobangProcessor.aiName[0], 208, 75)
        g2.drawString(GobangProcessor.aiName[1], 508, 75)

        g2.font = nameFont
        g2.drawString(":", 380, 48)
        if (!GobangProcessor.isGameOver) {
            g2.drawString(GobangProcessor.playerName[0], 200, 50)
            g2.drawString(GobangProcessor.playerName[1], 500, 50)
            g2.drawString(GobangProcessor.scores[0].toString(), 310, 50)
            g2.drawString(GobangProcessor.scores[1].toString(), 440, 50)
        } else {
            if (GobangProcessor.round % 2 == 1) {
                g2.drawString(GobangProcessor.playerName[1], 500, 50)
                g2.drawString(GobangProcessor.scores[1].toString(), 440, 50)
                g2.color = Color.RED
                g2.drawString(GobangProcessor.playerName[0], 200, 50)
                g2.drawString(GobangProcessor.scores[0].toString(), 310, 50)
                g2.font = winnerFont
                g2.drawString("Winner!", 40, 50)
            } else {
                g2.drawString(GobangProcessor.playerName[0], 200, 50)
                g2.drawString(GobangProcessor.scores[0].toString(), 310, 50)
                g2.color = Color.RED
                g2.drawString(GobangProcessor.playerName[1], 500, 50)
                g2.drawString(GobangProcessor.scores[1].toString(), 440, 50)
                g2.font = winnerFont
                g2.drawString("Winner!", 600, 50)
            }
        }
    }

    private fun drawBoard(g2: Graphics2D) {
        g2.stroke = BasicStroke(8.0f)
        g2.color = Color.BLACK

        g2.drawLine(GobangConfig.START_X, GobangConfig.START_Y, GobangConfig.END_X, GobangConfig.START_Y)
        g2.drawLine(GobangConfig.START_X, GobangConfig.START_Y + (GobangConfig.SIDE_LINE - 1) * GobangConfig.RECT_WIDTH,
                GobangConfig.END_X, GobangConfig.START_Y + (GobangConfig.SIDE_LINE - 1) * GobangConfig.RECT_WIDTH)
        g2.drawLine(GobangConfig.START_X, GobangConfig.START_Y, GobangConfig.START_X, GobangConfig.END_Y)
        g2.drawLine(GobangConfig.START_X + (GobangConfig.SIDE_LINE - 1) * GobangConfig.RECT_WIDTH, GobangConfig.START_Y,
                GobangConfig.START_X + (GobangConfig.SIDE_LINE - 1) * GobangConfig.RECT_WIDTH, GobangConfig.END_Y)

        g2.stroke = BasicStroke(3.0f)
        for (row in 1 until GobangConfig.SIDE_LINE - 1) {
            g2.drawLine(GobangConfig.START_X, GobangConfig.START_Y + row * GobangConfig.RECT_WIDTH,
                    GobangConfig.END_X, GobangConfig.START_Y + row * GobangConfig.RECT_WIDTH)
        }
        for (col in 1 until GobangConfig.SIDE_LINE - 1) {
            g2.drawLine(GobangConfig.START_X + col * GobangConfig.RECT_WIDTH, GobangConfig.START_Y,
                    GobangConfig.START_X + col * GobangConfig.RECT_WIDTH, GobangConfig.END_Y)
        }

        val pointList = arrayOf(arrayOf(3, 3), arrayOf(3, 11), arrayOf(7, 7), arrayOf(11, 3), arrayOf(11, 11))
        for ((row, col) in pointList) {
            val x = GobangConfig.START_X + col * GobangConfig.RECT_WIDTH - GobangConfig.POINT_RADIUS
            val y = GobangConfig.START_Y + row * GobangConfig.RECT_WIDTH - GobangConfig.POINT_RADIUS
            g2.fillOval(x, y, 2 * GobangConfig.POINT_RADIUS, 2 * GobangConfig.POINT_RADIUS)
        }

        for ((index, status) in GobangProcessor.boardStatus.withIndex()) {
            if (status != null) {
                g2.color = Color.BLACK
                g2.draw(chessBorderArray[index])
                if (status) {
                    g2.color = Color.BLACK
                } else {
                    g2.color = Color.WHITE
                }
                g2.fill(chessArray[index])
            }
        }

        if (GobangProcessor.currentIndex >= 0) {
            val row = GobangProcessor.currentIndex / GobangConfig.SIDE_LINE
            val col = GobangProcessor.currentIndex % GobangConfig.SIDE_LINE
            val x = GobangConfig.START_X + col * GobangConfig.RECT_WIDTH - GobangConfig.CURRENT_POINT_RADIUS
            val y = GobangConfig.START_Y + row * GobangConfig.RECT_WIDTH - GobangConfig.CURRENT_POINT_RADIUS
            g2.color = Color.RED
            g2.fillOval(x, y, 2 * GobangConfig.CURRENT_POINT_RADIUS, 2 * GobangConfig.CURRENT_POINT_RADIUS)
        }
    }

    fun drawRound(g2: Graphics2D) {
        g2.color = Color.BLACK
        g2.font = nameFont
        g2.drawString("Now: ", 715, 200)

        val currentPlayer: String = if (GobangProcessor.round % 2 == 1) "BLACK  ${GobangProcessor.round}"
            else "WHITE  ${GobangProcessor.round}"
        g2.drawString(currentPlayer, 815, 200)
    }

    fun drawRecord(g2: Graphics2D) {
        g2.color = Color.BLACK
        g2.font = subnameFont
        g2.drawString("Last 5 games:", 715, 400)

        g2.stroke = BasicStroke(1f)

        var circle = Ellipse2D.Double(680.0, 412.0, 20.0, 20.0)
        g2.fill(circle)
        circle = Ellipse2D.Double(950.0, 412.0, 20.0, 20.0)
        g2.draw(circle)

        for (i in 0..4) {
            if (!GobangRecorder.namesP1[i].isEmpty()) {
                g2.color = Color.BLACK
                if (!GobangRecorder.resultsP1[i].isEmpty()) {
                    g2.drawString(GobangRecorder.namesP2[i], 890, 430 + 30 * i)
                    g2.color = Color.RED
                    g2.drawString(GobangRecorder.namesP1[i], 715, 430 + 30 * i)
                    g2.drawString(GobangRecorder.resultsP1[i], 780, 430 + 30 * i)
                } else {
                    g2.drawString(GobangRecorder.namesP1[i], 715, 430 + 30 * i)
                    g2.color = Color.RED
                    g2.drawString(GobangRecorder.namesP2[i], 890, 430 + 30 * i)
                    g2.drawString(GobangRecorder.resultsP2[i], 835, 430 + 30 * i)
                }
            }
        }
    }

    override fun paintComponent(g: Graphics) {
        val g2: Graphics2D = g as Graphics2D
        g2.clearRect(0, 0, GobangConfig.DEFAULT_WIDTH, GobangConfig.DEFAULT_HEIGHT)

        drawPlayerName(g2)
        drawBoard(g2)
        drawRound(g2)
        drawRecord(g2)
    }

    override fun getPreferredSize(): Dimension {
        return Dimension(GobangConfig.DEFAULT_WIDTH, GobangConfig.DEFAULT_HEIGHT)
    }
}
