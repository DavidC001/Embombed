import serial

ser = serial.Serial(port='COM7', baudrate=115200, bytesize=8, parity='N', stopbits=1)

json = """G
{
    "SN": "AB123456789",
    "TIME": 20,
    "cavi": [0,1,0,1],
    "numPad": "1234AjjSDsd",
    "notNot":{
        "messaggi":[
            "down",
            "up",
            "not not down",
            "not not up"
        ],
        "mosse":[
            [0,0,0,1],
            [0,1,0,0],
            [1,1,1,0],
            [1,0,1,1]
        ]
    },
    "simonSays":{
        "ledAccesi":[
            [
                ["a","b","c","d","a","b","c","d"],
                ["a","b","c","d","a","b","c","d"],
                ["a","b","c","d","a","b","c","d"]
            ],
            [
                ["a","b","c","d","a","b","c","d"],
                ["a","b","c","d","a","b","c","d"]
            ]
        ],
        "mosse":[
            [1 , 3, 4],
            [2 , 5]
        ]
    }
}
"""

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
    print("sending...")
    ser.write(message.encode("iso-8859-1"))

def awaitResponse(headerSize):
    size = ser.read(headerSize)
    message = ser.read(int.from_bytes(size, byteorder='little')).decode("iso-8859-1")
    return message

awaitResponse(2)

sendMessagge(json, 2)
message = awaitResponse(2)
print(message)

while(1):
    message = awaitResponse(2)
    print(message)
    if(message == "start"):
        sendMessagge(json, 2)

ser.close()

# {"user": "johndoe", "admin": false, "uid": 1000, "groups": ["users", "wheel", "audio", "video"]}
