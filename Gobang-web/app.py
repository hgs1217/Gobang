# @Author:      HgS_1217_
# @Create Date: 2017/9/17

from flask import Flask
from route import bp
from config import APP_NAME


def create_app():
    app = Flask(APP_NAME)
    app.register_blueprint(bp)
    return app


if __name__ == '__main__':
    app = create_app()
    app.run(host='localhost', debug=True, threaded=False)
