# @Author:      HgS_1217_
# @Create Date: 2017/9/18

from ctypes import *
import time

dll = None


def call(data):
    global dll
    board_status, round, index, level = list(map(lambda x: int(x), data['boardStatus'])), int(data['round']), int(
        data['index']), int(data['level'])
    if not dll:
        dll = CDLL('libGobangAI.dll')
    func = dll.call
    func.restype = c_int
    func.argtypes = POINTER(c_int), c_int, c_int, c_int
    board_status_array = (c_int * len(board_status))(*board_status)
    start_time = time.time()
    result = func(board_status_array, round, index, level)
    end_time = time.time()
    print(end_time - start_time)
    print(result)
    return result


def init():
    global dll
    if not dll:
        dll = CDLL('libGobangAI.dll')
    dll.init()


def get_win_rate():
    global dll
    if not dll:
        dll = CDLL('libGobangAI.dll')
    func = dll.getWinRate
    func.restype = c_double
    result = func()
    return result
