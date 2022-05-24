import socket

STRING_LENGTH_LIMIT = 1000
#python constants who dis?

ADDRESS = "98:da:60:03:ad:5f" #define address
#robotica address: 98:da:60:03:ad:5f
#Vikis address: 98:da:60:01:ab:3c
#Vojtas address: 98:da:60:01:ab:e0
#better way would be to scan for addresses, look into that if possible

PORT = 1 #the port appears to always be 1

sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM) #initialize sock as a BT socket
sock.connect((ADDRESS, PORT)) #connect to the specified address using the specified port

while True:
    BYTES, RECEIVED_ADDRESS = sock.recvfrom(STRING_LENGTH_LIMIT) #is there a better alternative?
    print("Recieved:", BYTES.decode()) #slice notation allows us to ignore the "\n" character sent by the arduino

    MSG = input("Message:  ")+"\r\n" #get message, make sure that it skips to the next line (gud for serialport formatting)
    sock.send(MSG.encode()) #.encode() method ona string converts it to bytes