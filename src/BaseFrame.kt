import java.awt.Dimension
import javax.swing.JFrame

/**
 * Created by HgS_1217_ on 2017/9/11.
 */

abstract class BaseFrame: JFrame() {

    init {
        title = "Gobang"
        defaultCloseOperation = JFrame.EXIT_ON_CLOSE
        isVisible = true
        size = Dimension(GobangConfig.DEFAULT_WIDTH, GobangConfig.DEFAULT_HEIGHT)
    }

    override fun getPreferredSize(): Dimension {
        return Dimension(GobangConfig.DEFAULT_WIDTH, GobangConfig.DEFAULT_HEIGHT)
    }
}