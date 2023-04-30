import socket               
import time
        
host = "localhost"
port = 17246         

start = time.time() 
batch=1
for i in range(batch):
    sock = socket.socket() 
    sock.connect((host, port))
    # send hello
    sock.sendall(bytes.fromhex("00"))
    # recv reply
    recvData=sock.recv(1)
    
    with open("res/a.dng","rb") as f:
        data=f.read()

    dngSize=len(data)
    # send data size
    sock.sendall(dngSize.to_bytes(4,byteorder="little",signed=False))# 小端序
    # sned data
    sock.sendall(data)
    # recv png size
    bytes_pngSize=sock.recv(4)
    pngSize=int.from_bytes(bytes=bytes_pngSize,byteorder="little",signed=False)
    # recv png data
    pngData=sock.recv(pngSize)
    print(f"recv pngData,size: {len(pngData)}")
    # send reply
    sock.sendall(bytes.fromhex("00"))
    sock.close()

stop = time.time()  
print(f"total time cast: {(stop-start)*1000} ms")
print(f"total time cast per dng: {(stop-start)*1000/batch} ms")