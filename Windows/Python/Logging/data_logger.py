import socket
import csv
import time

STRING_LENGTH_LIMIT = 1000
START_TIME = time.time()

ADDRESS = "98:da:60:01:ab:e0" #define address
#Vikis address: 98:da:60:01:ab:3c
#Vojtas address: 98:da:60:01:ab:e0
#better way would be to scan for addresses, look into that if possible

PORT = 1 #the port appears to always be 1

sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM) #initialize sock as a BT socket
sock.connect((ADDRESS, PORT)) #connect to the specified address using the specified port

print("Received Data:")

with open(r"Log_Files\Log.csv", 'w+', newline='') as file:
    writer = csv.writer(file)

    while True:
        BYTES, RECEIVED_ADDRESS = sock.recvfrom(STRING_LENGTH_LIMIT) #is there a better alternative?
        text = BYTES.decode().strip()

        if text == "q":
            sock.send("t".encode())
            break

        if text:
            in_list = text.split(",")
            print("Received: ", text)
            try: 
                PROCESSED_LIST = ["", time.time()-START_TIME]+[float(x.strip()) for x in in_list]
                writer.writerow(PROCESSED_LIST)
                sock.send("r".encode())
            except:
                sock.send("w".encode())
        else:
            sock.send("n".encode())

file.close()
sock.close()