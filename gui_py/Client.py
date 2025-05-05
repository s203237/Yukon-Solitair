import socket

# Connection to C-server.
def connect_to_server(host='localhost', port=12345):
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((host, port))
        print(f"[Client] Connect to server on {host}:{port}")
        return sock
    except Exception as e:
        print(f"[Client] Error concerning connection: {e}")
        return None

# Send a command and getting the answer.
def send_command(sock, cmd):
    try:
        sock.sendall(cmd.encode())
        data = sock.recv(4096).decode()
        return data
    except Exception as e:
        print(f"[Client] Error concerning communication: {e}")
        return ""
