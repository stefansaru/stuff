#!/usr/bin/env python
import os

# Remote user, pass
username = 'stsaru'
password = 'qwe123'
hostname = 'localhost'

# port: default 22
portnum = '2022'

# My putty path is:
# D:\2016-10-03-old\2016-06-10-old\putty.exe
putty_path = os.path.join('D','2016-10-03-old','2016-06-10-old','putty.exe')

# Commands to run
cmds_list = [
  'free -m',
  'mpstat',
  'df -h',
]

# Run putty remote ssh command
# http://superuser.com/questions/587629/putty-run-a-remote-command-after-login-keep-the-shell-running

for cmd in cmds_list:
  putty_cmd = [putty_path, '-ssh', '{}:{}@{}'.format(username,password,hostname), '-p', portnum, '-m', '"{}"'.format(cmd)]
  print putty_cmd


