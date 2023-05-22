import socket

UDP_IP = "192.168.84.130"  # Replace with the server IP address
UDP_PORT = 12345  # Replace with the server port number
MESSAGE = "Hello, caught you!"

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Send the message to the server
sock.sendto(MESSAGE.encode(), (UDP_IP, UDP_PORT))
