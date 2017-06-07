
import dlock13
import utils

import unittest
from nose.tools import assert_raises

import time
import os, sys


# Setup/teardown
class DummyObject(object):
    pass
first = DummyObject()
first.file = 'first.gpio'
first.child = None
s = DummyObject()
s.lock = None
def setup_module():
    if not os.access(first.file, os.F_OK):
        os.mknod(first.file)
    first.child = utils.run_dlock13('first', first.file, 'test/door/')

    s.lock = dlock13.Opener({'first': 'test/door/first', 'notrunning': 'test/door/notrunning'})

    time.sleep(3)

def teardown_module():
    s.lock = None
    first.child.kill()
    os.unlink(first.file)

## Tests
# All the tests are ran against the same long-lived dlock13 instance(s)
# as this more closely reflects how it is normally used

def test_unlock_success():
    duration = 6
    until = s.lock.open('first', duration)
    assert until > time.time()+(duration/2)

def test_unlock_invalid_duration():
    assert_raises(ValueError, s.lock.open, 'first', 'text-is-not-number')

def test_unlock_extremely_long_duration():
    assert_raises(ValueError, s.lock.open, 'first', 1000)

def test_unlock_extremely_negative_duration():
    assert_raises(ValueError, s.lock.open, 'first', -1)

def test_unlock_invalid_name():
    assert_raises(ValueError, s.lock.open, 'not-a-door', 30)

def test_unlock_wrong_topic():
    assert_raises(dlock13.Timeout, s.lock.open, 'notrunning', 30)

def test_unlock_error_longer_than_max():
    with assert_raises(dlock13.UnlockingError) as cm:
        duration_above_max = 4*60
        s.lock.open('first', duration_above_max)
    assert 'longer than maximum' in cm.exception.message

def test_unlock_success_again():
    time.sleep(6) # FIXME: fails if not enough time has elapsed
    until = s.lock.open('first', 10)
    assert until > time.time()

    
