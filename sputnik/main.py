from PyQt5 import QtWidgets, uic
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from pyqtgraph import PlotWidget
import pyqtgraph as pg
import sys
from threading import Thread
from time import sleep

strdat = ""
data = []

def port_open():
    serial.setPortName(ui.comL.currentText())
    serial.open(QIODevice.ReadWrite)
    text = ui.comL.currentText() + " порт открыт"
    ui.statusbar.showMessage(text)

def parser():
    global data
    name = []
    zn = 0
    for dat in data:
        if dat[0] == "T":
            if dat[1] == "1":
                zn = dat[3:]
                print("T1=")
                print(zn)
            if dat[1] == "2":
                zn = dat[3:]
                print("T2=")
                print(zn)


def serial_send(data):
    txs = ""
    for val in data:
        txs += str(val) + ','
    txs = txs[:-1]
    print(txs)


def port_close():
    serial.close()
    text = ui.comL.currentText() + " порт закрыт"
    ui.statusbar.showMessage(text)

posX = 100
posY = 100
listX = []
notFirstTime=False
for x in range(100): listX.append(x)
listYT = []
for y in range(100): listYT.append(0)
listYH = []
for y in range(100): listYH.append(0)
listYHP = []
for y in range(100): listYHP.append(0)
listYA = []
for y in range(100): listYA.append(0)
listYP = []
for y in range(100): listYP.append(0)
listYPP = []
for y in range(100): listYPP.append(0)
listYRS = []
for y in range(100): listYRS.append(0)
listYRD = []
for y in range(100): listYRD.append(0)
listYTP = []
for y in range(100): listYTP.append(0)


def graphRS(dat):
    global listX
    global listYRS
    listYRS = listYRS[1:]
    if float(dat) > 10000:
        listYRS.append(listYRS[98])
    else:
        listYRS.append(float(dat))
    ui.GraphRS.clear()
    ui.GraphRS.plot(listX, listYRS)
    ui.OsRS.setText(dat+"мкР/ч")


def graphRD(dat):
    global listX
    global listYRD
    listYRD = listYRD[1:]
    if float(dat) > 10000:
        listYRD.append(listYRD[98])
    else:
        listYRD.append(float(dat))
    ui.GraphRD.clear()
    ui.GraphRD.plot(listX, listYRD)
    ui.OsRD.setText(dat+"mkR\\h")


def graphP(dat):
    global listX
    global listYP
    listYP = listYP[1:]
    listYP.append(float(dat))
    ui.GraphP.clear()
    ui.GraphP.plot(listX, listYP)
    ui.OsP.setText(dat+"Pa")


def graphPP(dat):
    global listX
    global listYPP
    listYPP = listYPP[1:]
    d=float(dat)*100
    listYPP.append(d)
    ui.GraphPP.clear()
    ui.GraphPP.plot(listX, listYPP)
    ui.OsPP.setText(str(d)+"Pa")


def graphT(dat):
    global listX
    global listYT
    listYT = listYT[1:]
    listYT.append(float(dat))
    ui.GraphT.clear()
    ui.GraphT.plot(listX, listYT)
    ui.OsTOut.setText(dat+"*C")


def graphTP(dat):
    global listX
    global listYTP
    listYTP = listYTP[1:]
    listYTP.append(float(dat))
    ui.GraphTP.clear()
    ui.GraphTP.plot(listX, listYTP)
    ui.OsTOutP.setText(dat+"*C")


def graphH(dat):
    global listX
    global listYH
    listYH = listYH[1:]
    listYH.append(float(dat))
    ui.GraphH.clear()
    ui.GraphH.plot(listX, listYH)
    ui.OsH.setText(dat+"m")


def graphHP(dat):
    global listX
    global listYHP
    listYHP = listYHP[1:]
    listYHP.append(float(dat))
    ui.GraphHP.clear()
    ui.GraphHP.plot(listX, listYHP)
    ui.OsHP.setText(dat+"m")


def graphA(dat):
    global listX
    global listYA
    listYA = listYA[1:]
    listYA.append(float(dat))
    ui.GraphA.clear()
    ui.GraphA.plot(listX, listYA)
    ui.OsA.setText(dat+"m\\s^2")


def read_on():
    rx = serial.readLine()
    rxs = str(rx, 'utf-8')
    print(rxs)
    global strdat
    global data
    global notFirstTime
    strdat += rxs
    if ';' in strdat:
            # anDat=strdat.split(';')
            # print(len(anDat))
            strdat=strdat.translate({ord('\n'): None, ord(' '): None, ord(';'): None, ord('\r'): None})
            f.write(strdat+'\n')
            data = strdat.split(',')
            if data[0] == 'PS':
                pars_piko()
            else:
                pars()
            strdat = ""
            data.clear()


def pars_piko():
    global data
    for dat in data:
        analis = dat.split('=')
        # print(analis)
        if check_data(analis):
            if analis[0] == 'T1':
                graphTP(analis[1])
                continue
            if analis[0] == 'T2':
                ui.PSTB.setValue(int(float(analis[1])))
                ui.PSTL.setText(analis[1])
                continue
            # if analis[0] == 'h':
            #     if analis[1] == '0':
            #         ui.OsHL.setText("Нагреватель выключен")
            #     if analis[1] == '1':
            #         ui.OsHL.setText("Нагреватель включён")
            #     continue
            # if analis[0] == 'V':
            #     ui.OsVolB.setValue(int(float(analis[1]) * 10))
            #     ui.OsVolL.setText(analis[1])
            #     continue
            if analis[0] == 'h':
                graphHP(analis[1])
                continue
            if analis[0] == 'P':
                graphPP(analis[1])
                continue


def pars():
    global data
    for dat in data:
        analis = dat.split('=')
        # print(analis)
        if analis[0]=='PS':
            pars_piko()
        if check_data(analis):
            if analis[0] == 'T1':
                graphT(analis[1])
                continue
            if analis[0] == 'T2':
                ui.OsTB.setValue(int(float(analis[1])))
                ui.OsTV.setText(analis[1])
                continue
            if analis[0] == 'h':
                if analis[1] == '0':
                    ui.OsHL.setText("Нагреватель выключен")
                if analis[1] == '1':
                    ui.OsHL.setText("Нагреватель включён")
                continue
            if analis[0] == 'V14':
                ui.OsVolB.setValue(int(float(analis[1]) * 10))#10,5 примерно половина заряда
                ui.OsVolL.setText(analis[1])
                continue
            if analis[0] == 'gi':
                graphA(analis[1])
                continue
            # if analis[0] == 'AH':
            #     graphH(analis[1])
            #     continue
            # if analis[0] == 'P':
            #     graphP(analis[1])
            #     continue
            if analis[0] == 'RS':
                graphRS(analis[1])
                continue
            if analis[0] == 'RD':
                graphRD(analis[1])
                continue


def check_data(an):
    if len(an) == 1:
        return 0
    try:
        float(an[1])
        return 1
    except ValueError:
        return 0


def signal(val):
    if val == 2:
        print("in cikle")
        serial.write(str(val).encode())


app = QtWidgets.QApplication([])
ui = uic.loadUi("sput.ui")
ui.setWindowTitle("Sputnik")


serial = QSerialPort()
portList = []
serial.setBaudRate(9600)
ports = QSerialPortInfo().availablePorts()
for port in ports:
            portList.append(port.portName())
ui.comL.addItems(portList)

f = open("telemetr.txt", 'w')

serial.readyRead.connect(read_on)
ui.BOpen.clicked.connect(port_open)
ui.BClose.clicked.connect(port_close)
ui.Signal.stateChanged.connect(signal)

ui.show()
app.exec()

f.close()