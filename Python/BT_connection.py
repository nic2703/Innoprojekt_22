import socket

address = "4C:87:5D:D0:1A:00" #define address
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

#sock.sendfiile() #needs investigation, alternatively test with sock.send()