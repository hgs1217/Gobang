const SIDE_NUM = 15,
    RECT_WIDTH = 32,
    CHESS_LEFT = 116,
    CHESS_TOP = 141,
    BOARD_STAR_ROW = [3, 3, 7, 11, 11],
    BOARD_STAR_COL = [3, 11, 7, 3, 11],
    DIRECTION_ARRAY = [-SIDE_NUM - 1, -SIDE_NUM, -SIDE_NUM + 1, -1, 1, SIDE_NUM - 1, SIDE_NUM, SIDE_NUM + 1]

let board,
    chess,
    names,
    scores,
    subnames,
    playersWinner,
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

function init() {
    document.oncontextmenu = function () {
        event.returnValue = false
    }

    $.getJSON('/board', function (data) {
        alert(data)
    })

    initElement()
    createBoard()
    createChess()
    setName()
    resetData()
}

function onRestart() {
    if (!confirm('P1('+playerNames[0]+') choose still choose color BLACK ?')) {
        [playerNames[0], playerNames[1]] = [playerNames[1], playerNames[0]];
        [playerSubNames[0], playerSubNames[1]] = [playerSubNames[1], playerSubNames[0]];
        [playerScores[0], playerScores[1]] = [playerScores[1], playerScores[0]];
    }
    resetData()
}

function initElement() {
    board = document.getElementById('board')
    chess = document.getElementById('chess')
    names = [document.getElementById('p1-name'), document.getElementById('p2-name')]
    scores = [document.getElementById('p1-score'), document.getElementById('p2-score')]
    subnames = [document.getElementById('p1-sub-name'), document.getElementById('p2-sub-name')]
    playersWinner = [document.getElementById('p1-winner'), document.getElementById('p2-winner')]
    nowContent = document.getElementById('now-content')
    recordP1Names = [document.getElementById('g1-p1-name'), document.getElementById('g2-p1-name'),
        document.getElementById('g3-p1-name'), document.getElementById('g4-p1-name'),
        document.getElementById('g5-p1-name')]
    recordP1Scores = [document.getElementById('g1-p1-score'), document.getElementById('g2-p1-score'),
        document.getElementById('g3-p1-score'), document.getElementById('g4-p1-score'), document.getElementById('g5-p1-score')]
    recordP2Names = [document.getElementById('g1-p2-name'), document.getElementById('g2-p2-name'),
        document.getElementById('g3-p2-name'), document.getElementById('g4-p2-name'),
        document.getElementById('g5-p2-name')]
    recordP2Scores = [document.getElementById('g1-p2-score'), document.getElementById('g2-p2-score'),
        document.getElementById('g3-p2-score'), document.getElementById('g4-p2-score'),
        document.getElementById('g5-p2-score')]
}

function setName() {
    if (playerTypes[0] === 0 && playerTypes[1] === 0) {
        playerNames = ['PLR1', 'PLR2']
        playerSubNames = ['', '']
    } else if (playerTypes[0] > 0 && playerTypes[1] > 0) {
        playerNames = ['COM1', 'COM2']
        playerSubNames = ['(AI LV'+playerTypes[0]+')', '(AI LV'+playerTypes[1]+')']
    } else if (playerTypes[0] === 0 && playerTypes[1] > 0) {
        playerNames = ['YOU', 'COM']
        playerSubNames = ['', '(AI LV'+playerTypes[1]+')']
    } else if (playerTypes[0] > 0 && playerTypes[1] === 0) {
        playerNames = ['COM', 'YOU']
        playerSubNames = ['(AI LV'+playerTypes[0]+')', '']
    }
}

function resetData() {
    playersWinner[0].hidden = true
    playersWinner[1].hidden = true
    nowContent.innerText = 'BLACK 1'
    if (indexs.length > 0) {
        auxiPointsArray[indexs[indexs.length-1]].hidden = true
    }
    indexs = []
    round = 1
    turnOn = true
    boardStatus = Array.apply(null, Array(SIDE_NUM * SIDE_NUM)).map(function (item, i) {
        return null
    })
    for (let chess of chessArray) {
        chess.hidden = true
    }
    updateData()
}

function updateData() {
    for (let i = 0; i < 2; ++i) {
        names[i].innerText = playerNames[i]
        names[i].style.color = '#000'
        scores[i].innerText = playerScores[i]
        scores[i].style.color = '#000'
        subnames[i].innerText = playerSubNames[i]
        subnames[i].style.color = '#000'
    }
    for (let i = 0; i < 5; ++i) {
        recordP1Names[i].innerText = recordsNamesP1[i]
        recordP1Scores[i].innerText = recordsScoresP1[i]
        recordP2Names[i].innerText = recordsNamesP2[i]
        recordP2Scores[i].innerText = recordsScoresP2[i]
        if (recordsScoresP1[i].length > 0) {
            recordP1Names[i].style.color = '#f00'
            recordP1Scores[i].style.color = '#f00'
        } else {
            recordP1Names[i].style.color = '#000'
            recordP1Scores[i].style.color = '#000'
        }
        if (recordsScoresP2[i].length > 0) {
            recordP2Names[i].style.color = '#f00'
            recordP2Scores[i].style.color = '#f00'
        } else {
            recordP2Names[i].style.color = '#000'
            recordP2Scores[i].style.color = '#000'
        }
    }
}

function createChess() {
    for (let row = 0; row < SIDE_NUM; ++row) {
        for (let col = 0; col < SIDE_NUM; ++col) {
            let circle = document.createElement('div')
            circle.className = 'chess-circle'
            circle.id = 'chess' + (row * SIDE_NUM + col).toString()
            circle.style.left = (col * RECT_WIDTH) + 'px'
            circle.style.top = (row * RECT_WIDTH) + 'px'
            circle.hidden = true
            chess.appendChild(circle)
            chessArray[row * SIDE_NUM + col] = circle

            let point = document.createElement('div')
            point.className = 'auxi-points'
            point.id = 'point' + (row * SIDE_NUM + col).toString()
            point.style.left = (col * RECT_WIDTH) + 'px'
            point.style.top = (row * RECT_WIDTH) + 'px'
            point.hidden = true
            chess.appendChild(point)
            auxiPointsArray[row * SIDE_NUM + col] = point
        }
    }
}

function createBoard() {
    for (let row = 0; row < SIDE_NUM - 1; ++row) {
        for (let col = 0; col < SIDE_NUM - 1; ++col) {
            let rect = document.createElement('div')
            rect.className = 'board-rect'
            rect.id = 'rect' + (row * SIDE_NUM + col).toString()
            rect.style.left = (col * RECT_WIDTH) + 'px'
            rect.style.top = (row * RECT_WIDTH) + 'px'
            board.appendChild(rect)
        }
    }
    for (let i = 0; i < BOARD_STAR_ROW.length; ++i) {
        let circle = document.createElement('div')
        circle.className = 'board-star'
        circle.id = BOARD_STAR_ROW[i] * SIDE_NUM + BOARD_STAR_COL[i]
        circle.style.left = (BOARD_STAR_COL[i] * RECT_WIDTH) + 'px'
        circle.style.top = (BOARD_STAR_ROW[i] * RECT_WIDTH) + 'px'
        chess.appendChild(circle)
    }
}

function onChessClick(e) {
    if (!turnOn) return

    let x = e.pageX - CHESS_LEFT,
        y = e.pageY - CHESS_TOP,
        i = Math.floor(y / RECT_WIDTH) * SIDE_NUM + Math.floor(x / RECT_WIDTH)
    if (boardStatus[i] === null) {
        turnOn = false
        chessArray[i].hidden = false
        chessArray[i].style.background = round % 2 === 1 ? '#000' : '#fff'
        boardStatus[i] = round % 2 === 1
        indexs.push(i)
        execTurn()
    }
}

function execTurn() {
    if (indexs.length > 1) {
        auxiPointsArray[indexs[indexs.length-2]].hidden = true
        auxiPointsArray[indexs[indexs.length-1]].hidden = false
    } else {
        auxiPointsArray[indexs[indexs.length-1]].hidden = false
    }

    if (judgeWin()) {
        let color = (round - 1) % 2
        playerScores[color]++
        playersWinner[color].hidden = false
        insertRecord()
        updateData()
        names[color].style.color = '#f00'
        scores[color].style.color = '#f00'
        subnames[color].style.color = '#f00'
    } else {
        round++
        nowContent.innerText = round % 2 === 1 ? 'BLACK '+round : 'WHITE '+round
        turnOn = true
    }
}

function judgeWin() {
    const chessArray = []
    let candidateIndex,
        status
    for (let i = 0; i < boardStatus.length; ++i) {
        if (boardStatus[i] !== null) {
            chessArray.push(i)
        }
    }

    for (let index of chessArray) {
        status = boardStatus[index]
        for (let dir of DIRECTION_ARRAY) {
            for (let i = 1; i <= 4; ++i) {
                candidateIndex = index + i * dir
                if (candidateIndex >= 0 && candidateIndex < boardStatus.length &&
                    boardStatus[candidateIndex] === status) {
                    if (i === 4) return true
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
            if (round % 2 === 1) {
                recordsScoresP1[i] = round + 'W'
            } else {
                recordsScoresP2[i] = round + 'W'
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
    if (round % 2 === 1) {
        recordsScoresP1[recordsNamesP1.length - 1] = round + 'W'
    } else {
        recordsScoresP2[recordsNamesP1.length - 1] = round + 'W'
    }
}
