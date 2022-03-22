import socket
import time

address = "98:da:60:01:ab:e0" #define address
#Vikis address: 98:da:60:01:ab:3c
#Vojtas address: 98:da:60:01:ab:e0
#better way would be to scan for addresses, look into that if possible
port = 1 #the port appears to always be 1

sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM) #initialize sock as a BT socket
sock.connect((address, port))

while True:
    msg = input("Message:  ") #get message and input
    bytes, address = sock.recvfrom(100)
    sock.send(msg.encode()) #.encode() method ona string converts it to bytes
    print("Recieved:", bytes.decode()) #slice notation allows us to ignore the "\n" character sent by the arduino

#sock.sendfile() #needs investigation, alternatively test with sock.send()