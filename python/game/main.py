import server
import generate
import json
from seriale import sendMessagge, awaitResponse, close
import threading

serverThread = threading.Thread(target=server.run)
serverThread.start()

while(1):
    awaitResponse(2)
    gameJson = generate.generateGame()
    generate.genManual()
    #stringify json
    msg = json.dumps(gameJson)
    sendMessagge(msg, 2)

close()
serverThread.join()