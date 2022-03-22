import socket
import time

address = "98:da:60:01:ab:3c" #define address
#better way would be to scan for addresses, look into that if possible

def test(port):
    try:
        sock.connect((address, port)) #attempt connection
        print("Connected to", address, "using port", port)
        return True
    except:
        print("Could not connect to", address, "using port", port)
        return False


sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM) #initialize sock as a BT socket

for p in range(10): #check the first 10 ports
    if test(p): #if the connection could be initialized, great
        break


while True:
    sock.send("Hello\n".encode())
    print("Sent Hello")
    time.sleep(2)

#sock.sendfile() #needs investigation, alternatively test with sock.send()