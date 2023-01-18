import sys
sys.path.append(R"..\serial")
from seriale import sendMessagge, awaitResponse, close

json = """
{
    "SN": "AB123456789",
    "TIME": 20,
    "cavi": [0,1,0,1],
    "numPad": "1234AjjSDsd",
    "notNot":{
        "messaggi":[
            "down",
            "up",
            "not down",
            "not up"
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

awaitResponse(2)
sendMessagge(json, 2)

while(1):
    message = awaitResponse(2)
    print(message)
    if(message == "S"):
        sendMessagge(json, 2)

close()