import os, signal
print('yes')

os.kill(os.getpid(), signal.SIGKILL)
