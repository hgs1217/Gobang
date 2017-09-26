# @Author:      HgS_1217_
# @Create Date: 2017/9/17

import json
from flask import Blueprint, render_template, request, jsonify
from config import VERSION
from ai_manager import call, init


bp = Blueprint('Gobang', __name__, template_folder='templates')


@bp.route('/', methods=['GET'])
def index():
    return render_template('index.html', version=VERSION)


@bp.route('/board', methods=['POST'])
def board():
    init()
    p1_type, p2_type = request.form.get('p1Type'), request.form.get('p2Type')
    return render_template('board.html', p1_type=p1_type, p2_type=p2_type)


@bp.route('/ai', methods=['POST'])
def call_ai():
    data = json.loads(request.get_data())
    index = call(data)
    return jsonify({'index': index}), 200


@bp.route('/init', methods=['POST'])
def init_ai():
    init()
    return jsonify({'msg': 'success'}), 200
