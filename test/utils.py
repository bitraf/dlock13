
import subprocess
import os

# Utils
def run_dlock13(role, gpiofile, mqttprefix, broker='mqtt://localhost', executable=None):
    if executable is None:
        executable = './dlock13-msgflo/build/dlock13'
    env = os.environ.copy()
    env['MSGFLO_BROKER'] = broker
    args = [executable, role, gpiofile, mqttprefix]
    child = subprocess.Popen(args, shell=False, env=env)
    return child
