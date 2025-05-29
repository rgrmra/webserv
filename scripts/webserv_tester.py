#!/usr/bin/python3
import socket
import sys
import json


def open_connection(host, port):

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, int(port)))

    return sock


def handle_response(sock):

    response = ''

    while True:
        tmp = sock.recv(1024).decode('utf-8')

        response += tmp

        if len(tmp) < 1024:
            break

    return response


def connection(request, host, port):

    try:
        sock = open_connection(host, port)
    except:
        print(f'\033[0;91mO servidor {host}:{port} está offline!\033[0;m');
        sys.exit(1);

    sock.sendall(request.encode('utf-8'))

    response = handle_response(sock)

    sock.close()

    return response


def format_host_port(request, host, port):

    request = request.replace('%h', host);
    request = request.replace('%p', str(port));

    return request


def open_configuration(path: str) -> dict:

    with open(path, 'r', encoding='utf-8') as file:

        return json.load(file)


if __name__ == "__main__":

    try:
        configuration: dict = open_configuration(sys.argv[1])
    except Exception as e:
        print(str(e))
        print('webserv_test: failed to open file')
        print(f'Usage: python ./{sys.argv[0]}<file>.json')
        sys.exit(1)

    host = configuration.get('host')
    port = configuration.get('port')

    if host == None or port == None:
        print(f'invalid host:port: {host}:{port}')
        exit(1)

    for request_dict in configuration.get('requests', ''):

        request = format_host_port(request_dict.get('request'), host, port)

        response = connection(request, host, port)

        response_startline = response.split('\r\n')[0]
        response_status = response_startline.split(' ')
        if len(response_status) > 1:
            response_status: str = response_status[1]
        else:
            response_status: str = 'clonnection closed'

        requested_status: str = request_dict.get('status')

        if response_status == request_dict.get('status'):
            sys.stdout.write(f'{request_dict.get("id")}. \033[0;92mOK\033[0;m ')
        else:
            sys.stdout.write(f'{request_dict.get("id")}. \033[0;91mKO\033[0;m \n')

            print(f'response status: {response_status}, requested: {requested_status}')
            exit(1)

    print('')
    exit(0)

