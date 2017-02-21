#!/usr/bin/env python
# python -m pip install paramiko
import paramiko

# Credentials
username = 'stsaru'
password = 'qwe123'
hostname = '127.0.0.1'
port = 2022
command = 'ls -lart'

# Client
ssh = paramiko.SSHClient()
ssh.load_system_host_keys()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh.connect(hostname=hostname, username=username, password=password, port=port)
ssh_stdin, ssh_stdout, ssh_stderr = ssh.exec_command(command)
print (ssh_stdout.readlines())
ssh.close()