
import matplotlib.pyplot as plt
import tkinter as tk
from tkinter import filedialog as fd
from tkinter import messagebox





def showEMG(path):
    f = open(path, "r")
    data=f.read()
    array=[]
    for char in data:
        array.append(char)
    lastarray=[]
    outputList = []
    for character in array:
        if character == '\n':
            continue
        outputList.append(character)


    for i in range(int(len(outputList)/4)):
        lastarray.append((str(outputList[4*i])+str(outputList[4*i+1])+str(outputList[4*i+2])+str(outputList[4*i+3])))

    datalast=[]

    for i in range(len(lastarray)):
        datalast.append(float((int(lastarray[i])/4096)*3.3))

    x=[]
    for i in range(len(datalast)):
        x.append(i*0.5/1000)



    plt.plot(x, datalast,"")
    plt.xlabel("czas [s]")
    plt.ylabel("Napiecie [V]")
    plt.show()















def loadfile():
    file_path=fd.askopenfile(initialdir="/home/dawid/Desktop/Programing and other/GITLAB/eame-projekt-zespol-3/DANE").name
    print(file_path)
    
    showEMG(file_path)





if __name__== '__main__':
    loadfile() 





