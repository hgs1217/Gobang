
function init() {
    document.oncontextmenu = function () {
        event.returnValue = false
    }
}

function start(e) {
    location.href = "board.html"
}
