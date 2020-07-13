from flask import Flask, jsonify, request

app = Flask(__name__)


@app.route('/data', methods=['POST'])
def post_data():
    print('received data', request.data)
    return 'Flask is running!'


if __name__ == '__main__':
    app.run(debug=True)
