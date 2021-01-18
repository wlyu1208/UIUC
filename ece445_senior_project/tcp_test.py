'''
    This code is used to test TCP communication with a PC. This code serves as th 
    server, where the PC serves as a client.
'''
import socket


# Device IP and Port
myIP = '192.168.0.200'
myPort = 8888

# create a TCP/IP socket
mySocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
mySocket.bind((myIP,myPort))

# sets socket as a server, tells it to listen for incoming messages
mySocket.listen(1)

# returns an open connection between the server(device) and the client(PC)
connection, client_address = mySocket.accept()

# use the connection to recieve data
while True:
    data = connection.recv(1024)
    print("data : ", data)
    if data == b'End':
        break

print("closing connection")
connection.close()

