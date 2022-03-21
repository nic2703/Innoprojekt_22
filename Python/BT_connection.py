import socket

address = "4C:87:5D:D0:1A:00"    

def test(port):
    try:
        sock.connect((address, port))
        print("Connected to", address, "using port", port)
        return True
    except:
        print("Could not connect to", address, "using port", port)
        return False


sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)

for p in range(1000): #check the first 1000 ports
    if test(p):
        break