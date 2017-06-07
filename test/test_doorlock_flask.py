
import dlock13
import utils

import os, time
import unittest

from flask import Flask

def setup_flask_app():
    app = Flask(__name__)
    doors = {
        'working': 'test/door/flasky',
        'broken': 'test/door/ffff',
    }
    app.locks = dlock13.Doorlock(doors)

    @app.route('/open/<door>', methods=['POST'])
    def open(door):
        try:
            until = app.locks.open(door, 10)
            return "Door %s is open until %d" % (door, until)
        except dlock13.Timeout, e:
            return ("Failed to open, timeout", 500)

    return app

# Setup/teardown
class DummyObject(object):
    pass
sub = DummyObject()
sub.file = 'flasky.gpio'
sub.child = None
sub.app = None
def setup_module():
    if not os.access(sub.file, os.F_OK):
        os.mknod(sub.file)
    sub.child = utils.run_dlock13('flasky', sub.file, 'test/door/')
    time.sleep(3)
    sub.app = setup_flask_app()

def teardown_module():
    sub.child.kill()
    os.unlink(sub.file)
    sub.app.locks = None
    sub.app = None

class FlaskTestCase(unittest.TestCase):

    def setUp(self):
        self.flask_app = sub.app
        self.client = self.flask_app.test_client()

    def tearDown(self):
        self.flask_app = None

    def test_success(self):
        r = self.client.post('/open/working')
        self.assertEqual(r.status_code, 200)
        self.assertIn('is open until', r.data)

    def test_error(self):
        r = self.client.post('/open/broken')
        self.assertEqual(r.status_code, 500)
        self.assertIn('timeout', r.data)

if __name__ == '__main__':
    app = setup_flask_app()
    app.run()
