import socket
import csv

STRING_LENGTH_LIMIT = 1000

ADDRESS = "98:da:60:01:ab:e0" #define address
#Vikis address: 98:da:60:01:ab:3c
#Vojtas address: 98:da:60:01:ab:e0
#better way would be to scan for addresses, look into that if possible

PORT = 1 #the port appears to always be 1

sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM) #initialize sock as a BT socket
sock.connect((ADDRESS, PORT)) #connect to the specified address using the specified port

with open("./Log_Files/Log.csv", "w") as file:
    writer = csv.writer(file)

    while True:
        BYTES, RECEIVED_ADDRESS = sock.recvfrom(STRING_LENGTH_LIMIT) #is there a better alternative?
        text = BYTES.decode().strip()

        if text == "q":
            sock.send("t".encode())
            break

        if text:
            try: 
                in_list = text.split()
                sock.send("r".encode())
                writer.writerow([int(x.strip()) for x in in_list])
            except:
                sock.send("w".encode())
        else:
            sock.send("n".encode())

sock.close()