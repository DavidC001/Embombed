import serial

ser = None

def openSer():
    global ser
    ser = serial.Serial(port='COM7', baudrate=115200, bytesize=8, parity='N', stopbits=1)

def sendMessagge(message, headerSize):
    preso = False
    while not preso:
        print(int.to_bytes(len(message), headerSize, byteorder='little'))
        ser.write(int.to_bytes(len(message), headerSize, byteorder='little'))
        risposta = ser.read().decode("iso-8859-1")
        print(risposta)
        if (risposta == "B"):
            preso = True
            print("Preso")
        else:
            print("Errore")
    print("sending...")
    ser.write(message.encode("iso-8859-1"))

def awaitResponse(headerSize):
    size = ser.read(headerSize)
    message = ser.read(int.from_bytes(size, byteorder='little')).decode("iso-8859-1")
    return message

def close():
    ser.close()