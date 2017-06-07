
import dlock13

import unittest
import time

# FIXME: setup child process

def test_unlock_success():
    lock = dlock13.Doorlock({'first': 'test/door/first', 'second': 'test/door/second'})
    duration = 5
    until = lock.open('first', duration)
    assert until > time.time()+(duration/2)

def test_unlock_invalid_duration():
    pass

def test_unlock_invalid_name():
    pass

def test_unlock_wrong_topic():
    pass

def test_unlock_topic_trailing_slash():
    pass

def test_unlock_timeout():
    pass

def test_unlock_success_again():
    #lock.open('second', 10)
    pass
    
