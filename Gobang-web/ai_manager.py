# @Author:      HgS_1217_
# @Create Date: 2017/9/18

from ctypes import *


dll = None


def call(data):
    global dll
    board_status, round, level = list(map(lambda x: int(x), data['boardStatus'])), int(data['round']), int(
        data['level'])
    if not dll:
        dll = WinDLL('libGobangAI.dll')
    func = dll.call
    func.restype = c_int
    func.argtypes = POINTER(c_int), c_int, c_int
    board_status_array = (c_int * len(board_status))(*board_status)
    index = func(board_status_array, round, level)
    print(index)
    return index
