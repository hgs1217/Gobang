const SIDE_NUM = 15,
    RECT_WIDTH = 32,
    CHESS_LEFT = 116,
    CHESS_TOP = 141,
    BOARD_STAR_ROW = [3, 3, 7, 11, 11],
    BOARD_STAR_COL = [3, 11, 7, 3, 11],
    DIRECTION_ARRAY = [-SIDE_NUM - 1, -SIDE_NUM, -SIDE_NUM + 1, -1, 1, SIDE_NUM - 1, SIDE_NUM, SIDE_NUM + 1]

let board,
    container,
    chess,
    names,
    scores,
    subnames,
    playersWinner,
    draw,
    nowContent,
    recordP1Names,
    recordP1Scores,
    recordP2Names,
    recordP2Scores,
    playerTypes = [0, 0],
    playerNames = ['', ''],
    playerScores = [0, 0],
    playerSubNames = ['', ''],
    recordsNamesP1 = ['', '', '', '', ''],
    recordsNamesP2 = ['', '', '', '', ''],
    recordsScoresP1 = ['', '', '', '', ''],
    recordsScoresP2 = ['', '', '', '', ''],
    indexs = [],
    round,
    turnOn,
    boardStatus,
    chessArray = Array.apply(null, Array(SIDE_NUM * SIDE_NUM)).map(function (item, i) {
        return null
    }),
    auxiPointsArray = Array.apply(null, Array(SIDE_NUM * SIDE_NUM)).map(function (item, i) {
        return null
    })

$.extend({
    get: function (url, args) {
        let body = $(document.body),
            form = $("<form></form>"),
            input
        form.attr({'action': url})
        $.each(args, function (key, value) {
            input = $("<input type='hidden'>")
            input.attr({"name": key})
            input.val(value)
            form.append(input)
        })
        form.appendTo(body)
        form.submit()
        body.removeChild(form)
    }
})

function init() {
    document.oncontextmenu = function () {
        event.returnValue = false
    }

    initElement()
    createBoard()
    createChess()
    setName()
    resetData()
}

function onRestart() {
    if (!confirm('P1(' + playerNames[0] + ') still choose color BLACK ?')) {
        [playerTypes[0], playerTypes[1]] = [playerTypes[1], playerTypes[0]];
        [playerNames[0], playerNames[1]] = [playerNames[1], playerNames[0]];
        [playerSubNames[0], playerSubNames[1]] = [playerSubNames[1], playerSubNames[0]];
        [playerScores[0], playerScores[1]] = [playerScores[1], playerScores[0]];
    }
    initAI()
    resetData()
}

function onBack() {
    $.get('/')
}

function initElement() {
    board = $('#board')
    container = $('#container')
    chess = $('#chess')
    names = [$('#p1-name'), $('#p2-name')]
    scores = [$('#p1-score'), $('#p2-score')]
    subnames = [$('#p1-sub-name'), $('#p2-sub-name')]
    playersWinner = [$('#p1-winner'), $('#p2-winner')]
    draw = $('#draw')
    nowContent = $('#now-content')
    recordP1Names = [$('#g1-p1-name'), $('#g2-p1-name'),
        $('#g3-p1-name'), $('#g4-p1-name'),
        $('#g5-p1-name')]
    recordP1Scores = [$('#g1-p1-score'), $('#g2-p1-score'),
        $('#g3-p1-score'), $('#g4-p1-score'), $('#g5-p1-score')]
    recordP2Names = [$('#g1-p2-name'), $('#g2-p2-name'),
        $('#g3-p2-name'), $('#g4-p2-name'),
        $('#g5-p2-name')]
    recordP2Scores = [$('#g1-p2-score'), $('#g2-p2-score'),
        $('#g3-p2-score'), $('#g4-p2-score'),
        $('#g5-p2-score')]
    playerTypes = [parseInt($('#p1-type').text()), parseInt($('#p2-type').text())]
}

function setName() {
    if (playerTypes[0] === 0 && playerTypes[1] === 0) {
        playerNames = ['PLR1', 'PLR2']
        playerSubNames = ['', '']
    } else if (playerTypes[0] > 0 && playerTypes[1] > 0) {
        playerNames = ['COM1', 'COM2']
        playerSubNames = ['(AI LV' + playerTypes[0] + ')', '(AI LV' + playerTypes[1] + ')']
    } else if (playerTypes[0] === 0 && playerTypes[1] > 0) {
        playerNames = ['YOU', 'COM']
        playerSubNames = ['', '(AI LV' + playerTypes[1] + ')']
    } else if (playerTypes[0] > 0 && playerTypes[1] === 0) {
        playerNames = ['COM', 'YOU']
        playerSubNames = ['(AI LV' + playerTypes[0] + ')', '']
    }
}

function resetData() {
    for (let winner of playersWinner) {
        winner.attr('hidden', true)
    }
    draw.attr('hidden', true)
    nowContent.text('BLACK 1')
    if (indexs.length > 0) {
        auxiPointsArray[indexs[indexs.length - 1]].attr('hidden', true)
    }
    indexs = []
    round = 1
    boardStatus = Array.apply(null, Array(SIDE_NUM * SIDE_NUM)).map(function (item, i) {
        return 0
    })
    for (let chess of chessArray) {
        chess.attr('hidden', true)
    }
    updateData()

    if (playerTypes[0] > 0) {
        callAI(playerTypes[0])
    } else {
        turnOn = true
    }
}

function updateData() {
    for (let i = 0; i < 2; ++i) {
        names[i].text(playerNames[i])
        names[i].css('color', '#000')
        scores[i].text(playerScores[i])
        scores[i].css('color', '#000')
        subnames[i].text(playerSubNames[i])
        subnames[i].css('color', '#000')
    }
    for (let i = 0; i < 5; ++i) {
        recordP1Names[i].text(recordsNamesP1[i])
        recordP1Scores[i].text(recordsScoresP1[i])
        recordP2Names[i].text(recordsNamesP2[i])
        recordP2Scores[i].text(recordsScoresP2[i])
        if (recordsScoresP1[i].length > 0 && recordsScoresP2[i].length === 0) {
            recordP1Names[i].css('color', '#f00')
            recordP1Scores[i].css('color', '#f00')
            recordP2Names[i].css('color', '#000')
            recordP2Scores[i].css('color', '#000')
        } else if (recordsScoresP2[i].length > 0 && recordsScoresP1[i].length === 0) {
            recordP2Names[i].css('color', '#f00')
            recordP2Scores[i].css('color', '#f00')
            recordP1Names[i].css('color', '#000')
            recordP1Scores[i].css('color', '#000')
        } else {
            recordP1Names[i].css('color', '#000')
            recordP1Scores[i].css('color', '#000')
            recordP2Names[i].css('color', '#000')
            recordP2Scores[i].css('color', '#000')
        }
    }
}

function createChess() {
    for (let row = 0; row < SIDE_NUM; ++row) {
        for (let col = 0; col < SIDE_NUM; ++col) {
            let circle = $("<div class='chess-circle' hidden></div>")
            circle.attr('id', 'chess' + (row * SIDE_NUM + col))
            circle.css({
                'left': col * RECT_WIDTH + 'px',
                'top': row * RECT_WIDTH + 'px'
            })
            circle.appendTo(chess)
            chessArray[row * SIDE_NUM + col] = circle

            let point = $("<div class='auxi-points' hidden></div>")
            point.attr('id', 'point' + (row * SIDE_NUM + col))
            point.css({
                'left': col * RECT_WIDTH + 'px',
                'top': row * RECT_WIDTH + 'px'
            })
            point.appendTo(chess)
            auxiPointsArray[row * SIDE_NUM + col] = point
        }
    }
}

function createBoard() {
    for (let row = 0; row < SIDE_NUM - 1; ++row) {
        for (let col = 0; col < SIDE_NUM - 1; ++col) {
            let rect = $("<div class='board-rect'></div>")
            rect.attr('id', 'rect' + (row * SIDE_NUM + col))
            rect.css({
                'left': col * RECT_WIDTH + 'px',
                'top': row * RECT_WIDTH + 'px'
            })
            rect.appendTo(board)
        }
    }

    for (let row = 0; row < SIDE_NUM; ++row) {
        let rect = $("<div class='board-mark'></div>")
        rect.attr('id', 'mark-row' + row)
        rect.css({
            'left': -1.2 * RECT_WIDTH + 'px',
            'top': (row * RECT_WIDTH - RECT_WIDTH / 4) + 'px'
        })
        rect.text(row + 1);
        rect.appendTo(board)
    }

    for (let col = 0; col < SIDE_NUM; ++col) {
        let rect = $("<div class='board-mark'></div>")
        rect.attr('id', 'mark-col' + col)
        rect.css({
            'left': (col * RECT_WIDTH - RECT_WIDTH / 2) + 'px',
            'top': -RECT_WIDTH + 'px'
        })
        rect.text(col + 1);
        rect.appendTo(board)
    }

    for (let i = 0; i < BOARD_STAR_ROW.length; ++i) {
        let circle = $("<div class='board-star'></div>")
        circle.attr('id', 'star' + (BOARD_STAR_ROW[i] * SIDE_NUM + BOARD_STAR_COL[i]))
        circle.css({
            'left': BOARD_STAR_COL[i] * RECT_WIDTH + 'px',
            'top': BOARD_STAR_ROW[i] * RECT_WIDTH + 'px'
        })
        circle.appendTo(chess)
    }
}

function onChessClick(e) {
    if (!turnOn) return

    let x = e.pageX - CHESS_LEFT,
        y = e.pageY - CHESS_TOP,
        i = Math.floor(y / RECT_WIDTH) * SIDE_NUM + Math.floor(x / RECT_WIDTH)
    if (boardStatus[i] === 0) {
        turnOn = false
        putChess(i)
    }
}

function putChess(i) {
    chessArray[i].attr('hidden', false)
    chessArray[i].css('background', round % 2 === 1 ? '#000' : '#fff')
    boardStatus[i] = round % 2 === 1 ? 1 : -1
    indexs.push(i)
    execTurn()
}

function execTurn() {
    if (indexs.length > 1) {
        auxiPointsArray[indexs[indexs.length - 2]].attr('hidden', true)
        auxiPointsArray[indexs[indexs.length - 1]].attr('hidden', false)
    } else {
        auxiPointsArray[indexs[indexs.length - 1]].attr('hidden', false)
    }

    if (judgeWin()) {
        let color = (round - 1) % 2
        playerScores[color]++
        playersWinner[color].attr('hidden', false)
        insertRecord()
        updateData()
        names[color].css('color', '#f00')
        scores[color].css('color', '#f00')
        subnames[color].css('color', '#f00')
    } else {
        round++
        let color = (round - 1) % 2
        nowContent.text(color === 0 ? 'BLACK ' + round : 'WHITE ' + round)
        if (round > SIDE_NUM * SIDE_NUM) {
            insertRecord()
            updateData()
            draw.attr('hidden', false)
        } else {
            if (playerTypes[color] > 0) {
                callAI(playerTypes[color])
            } else {
                turnOn = true
            }
        }
    }
}

function judgeWin() {
    const chessArray = []
    let candidateIndex,
        lastIndex,
        lastRow,
        lastCol,
        status
    for (let i = 0; i < boardStatus.length; ++i) {
        if (boardStatus[i] !== 0) {
            chessArray.push(i)
        }
    }

    for (let index of chessArray) {
        status = boardStatus[index]
        for (let dir of DIRECTION_ARRAY) {
            for (let i = 1; i <= 4; ++i) {
                candidateIndex = index + i * dir
                lastIndex = candidateIndex - dir
                lastRow = lastIndex / SIDE_NUM
                lastCol = lastIndex % SIDE_NUM
                if (lastCol === 0 && (dir + 2 * SIDE_NUM) % SIDE_NUM === SIDE_NUM - 1 ||
                    lastCol === SIDE_NUM - 1 && (dir + SIDE_NUM) % SIDE_NUM === 1 ||
                    lastRow === 0 && dir <= -SIDE_NUM + 1 ||
                    lastRow === SIDE_NUM - 1 && dir >= SIDE_NUM - 1) {
                    break
                }
                if (boardStatus[candidateIndex] === status) {
                    if (i === 4) {
                        return true
                    }
                    continue
                }
                break
            }
        }
    }
    return false
}

function insertRecord() {
    for (let i = 0; i < recordsNamesP1.length; ++i) {
        if (recordsNamesP1[i].length === 0) {
            recordsNamesP1[i] = playerNames[0]
            recordsNamesP2[i] = playerNames[1]
            if (round > SIDE_NUM * SIDE_NUM) {
                recordsScoresP1[i] = 'DRAW'
                recordsScoresP2[i] = 'DRAW'
            } else {
                if (round % 2 === 1) {
                    recordsScoresP1[i] = round + 'W'
                } else {
                    recordsScoresP2[i] = round + 'W'
                }
            }
            return
        }
    }
    for (let i = 0; i < recordsNamesP1.length - 1; ++i) {
        recordsNamesP1[i] = recordsNamesP1[i + 1]
        recordsNamesP2[i] = recordsNamesP2[i + 1]
        recordsScoresP1[i] = recordsScoresP1[i + 1]
        recordsScoresP2[i] = recordsScoresP2[i + 1]
    }
    recordsNamesP1[recordsNamesP1.length - 1] = playerNames[0]
    recordsNamesP2[recordsNamesP1.length - 1] = playerNames[1]
    if (round > SIDE_NUM * SIDE_NUM) {
        recordsScoresP1[recordsNamesP1.length - 1] = 'DRAW'
        recordsScoresP2[recordsNamesP1.length - 1] = 'DRAW'
    } else {
        if (round % 2 === 1) {
            recordsScoresP1[recordsNamesP1.length - 1] = round + 'W'
            recordsScoresP2[recordsNamesP1.length - 1] = ''
        } else {
            recordsScoresP2[recordsNamesP1.length - 1] = round + 'W'
            recordsScoresP1[recordsNamesP1.length - 1] = ''
        }
    }
}

function callAI(level) {
    let data = {
        'boardStatus': boardStatus,
        'round': round,
        'level': level,
        'index': indexs.length > 0 ? indexs[indexs.length - 1] : -1
    }
    $.ajax({
        type: 'POST',
        url: '/ai',
        data: JSON.stringify(data),
        contentType: 'application/json; charset=UTF-8',
        dataType: 'json',
        success: function (data) {
            putChess(data.index)
        },
        error: function (xhr, type) {
            console.log(xhr)
        }
    })
}

function initAI() {
    $.ajax({
        type: 'POST',
        url: '/init',
        success: function (data) {
            console.log(data)
        },
        error: function (xhr, type) {
            console.log(xhr)
        }
    })
}
