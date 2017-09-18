# @Author:      HgS_1217_
# @Create Date: 2017/9/18

import random


def call(data):
    board_status, round, level = [(i, data['boardStatus'][i]) for i in range(len(data['boardStatus']))], int(
        data['round']), int(data['level'])
    legals = list(map(lambda x: x[0], filter(lambda x: x[1] is None, board_status)))
    return random.choice(legals)
