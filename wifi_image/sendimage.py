#! /usr/bin/env python3
import socket
import time
import threading
import ctypes
import os
from PIL import Image, ImageGrab

pc_ipaddress = '192.168.1.0'
m5stick_ipaddress = '192.168.1.1'
port = 25001

redraws = [True]*20
zoom = 8

class POINT(ctypes.Structure):
    _fields_ = [("x", ctypes.c_ulong),("y", ctypes.c_ulong)]

wc = POINT()

class RecvThread(threading.Thread):
    def __init__(self, s):
        self.s = s
        threading.Thread.__init__(self)
    def run(self):
        while 1:
            try:
                rcvmsg = self.s.recv(1024)
                if rcvmsg != '':
                    a = int.from_bytes(rcvmsg, byteorder='big')
                    b = ''
                    for i in rcvmsg:
                        t = bin(i)[2:]
                        b += '0'*(8-len(t))+t
                    b = b[:20]
                    for i in range(20):
                        if b[i] == '1':
                            redraws[i] = True
                        else:
                            redraws[i] = False
            except:
                return

def rgb565(r, g, b):
    return (r>>3)<<11 | (g>>2)<<5 | b>>3

class SendThread(threading.Thread):
    def __init__(self, s):
        self.s = s
        threading.Thread.__init__(self)
    def run(self):
        while 1:
            try:
                w = (160*zoom//2)
                h = (80*zoom//2)
                ctypes.windll.user32.GetCursorPos(ctypes.byref(wc))
                box = (wc.x-w, wc.y-h, wc.x+w, wc.y+h)
                im = ImageGrab.grab(box)
                rgb = im.convert('RGB')
                # start = time.time()
                
                for iy in range(20):
                    if redraws[iy] == False : continue
                    dy = iy*4
                    msg = bytes()
                    for i in range(4):
                        for j in range(160):
                            r, g, b = rgb.getpixel(((j)*zoom, (dy+i)*zoom))
                            t = rgb565(r, g, b)
                            # print('%x'%(t>>8),'%x'%(t&255))
                            msg += t.to_bytes(2, 'big')
                    msg += iy.to_bytes(1, 'big')
                    self.s.sendto(msg, (m5stick_ipaddress, port))
                    
                    # time.sleep(0.0001)
                    # while time.clock() - tmptime < 0.0016 : pass
                # print(time.time() - start)
            except:
                return

print(m5stick_ipaddress, port)
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind((pc_ipaddress, port))
print('送信中')

rcv = RecvThread(s)
rcv.start()
send = SendThread(s)
send.start()
rcv.join()
send.join()
print('終了')
