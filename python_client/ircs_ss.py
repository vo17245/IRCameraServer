import socket               
import time
        
host = "192.168.1.9"
port = 17245         

start = time.time() 
batch=100
for i in range(batch):
    sock = socket.socket() 
    sock.connect((host, port))
    # send hello
    sock.send(bytes.fromhex("00"))
    # recv reply
    recvData=sock.recv(1)
    # send data
    with open("a.dng","rb") as f:
        data=f.read()

    dngSize=len(data)

    sock.send(dngSize.to_bytes(4,byteorder="little",signed=False))# 小端序
    sock.send(data)
    # recv server recv reply
    recvData=sock.recv(1)
    sock.close()

stop = time.time()  
print(f"total time cast: {(stop-start)*1000} ms")
print(f"total time cast per dng: {(stop-start)*1000/batch} ms")