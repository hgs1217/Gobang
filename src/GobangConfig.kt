import java.awt.Rectangle

/**
 * Created by HgS_1217_ on 2017/9/10.
 */

class GobangConfig private constructor(){

    companion object {

        val DEFAULT_WIDTH = 1000  // 窗口宽度
        val DEFAULT_HEIGHT = 725 // 窗口高度
        val START_X = 100
        val START_Y = 100
        val RECT_WIDTH = 40
        val SIDE_LINE = 15
        val POINT_RADIUS = 5
        val CURRENT_POINT_RADIUS = 3
        val END_X = START_X + (SIDE_LINE - 1) * RECT_WIDTH
        val END_Y = START_Y + (SIDE_LINE - 1) * RECT_WIDTH
        val EFFECTIVE_REGION = Rectangle(START_X - RECT_WIDTH / 2, START_Y - RECT_WIDTH / 2,
                RECT_WIDTH * SIDE_LINE, RECT_WIDTH * SIDE_LINE)
        val SIZE_OFFSET = 5
        val BORDER_OFFSET = 4
        val DIRECTION_ARRAY: Array<Int> = arrayOf(-SIDE_LINE-1, -SIDE_LINE, -SIDE_LINE+1, -1,
                1, SIDE_LINE-1, SIDE_LINE, SIDE_LINE+1)
    }
}
