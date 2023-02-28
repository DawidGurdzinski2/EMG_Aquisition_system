
    
def printdata():
    try:
        canvas.delete("plot")
        for i in range(len(fulldata)-1):
            canvas.create_line(i,height-1-fulldata[i]*height/3.3,i+1,height-fulldata[i+1]*height/3.3,fill="red",tag="plot")
    except:
        pass 
    
def acqusition():
    
    i=0
    #data=[2,2]
    while True:
        data=s.recv(1024)  
        # data.append(115)  
        # data.append(48)
        # data.append(48)
        # data.append(54)
        # data.append(54)
        while(i<len(data)):
            print(data)
            if data[i]==115:
                for j in range(4):
                    try:
                        if data[j+i+1] != 115:
                            tempbuf.append(data[i+j+1])
                            
                    except:
                        pass
                
                if len(tempbuf) ==4:  
                    adcbuf1.append(tempbuf[0])            
                    adcbuf1.append(tempbuf[1])    
                    adcbuf1.append(tempbuf[2])    
                    adcbuf1.append(tempbuf[3])    
                    i+=4
                tempbuf.clear()
                
            i+=1
            print(adcbuf1)
            if(len(adcbuf1)>=40):
                for h in range(10):
                    fulldata.append(int(str(adcbuf1[4*h]-48)+str(adcbuf1[4*h+1]-48)+str(adcbuf1[4*h+2]-48)+str(adcbuf1[4*h+3]-48))*3.3/4096)
                    fulldata.pop(0)   
                adcbuf1.clear()  
        #print(fulldata)
            printdata()
        i=0
        sleep(0.01)


from tkinter import messagebox
import tkinter as tk
import threading
import time 
import socket
from time import sleep

## nie niechciało mi sie robic klas ani tego pożadkowac to ma dzialac a nie wygladac  

window =tk.Tk() 
windowHeight=411
windowWidth=611
window.geometry(str(windowWidth)+"x"+str(windowHeight))
window.title("Main Window")
window.config(background="white")
frame=tk.Frame(window,bg="black",bd=5)
frame.grid(row=0,column=0,sticky="nswe")
height=800
width=600
canvas = tk.Canvas(frame,width=width,height=height+2,bg="black")
canvas.grid(row=0,column=0)


fulldatalen=600

m5stick_addr = '00:22:06:30:7A:7A'
port = 1
s = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
s.connect((m5stick_addr, port))
fulldata=[]
adcbuf1=[]
tempbuf=[]
for w in range(600):
    fulldata.append(0)


x=threading.Thread(target=acqusition,daemon=True)
x.start()


def on_closing():
    if messagebox.askokcancel("Quit", "Do you want to quit?"):
        window.destroy()
        
window.protocol("WM_DELETE_WINDOW", on_closing)
window.mainloop() 



s.close()



