import server
import generate
import json
import multiprocessing
from seriale import sendMessagge, awaitResponse, close, openSer


def processJobs():
    #write to manual.html message to start press reset button on MSP432
    with open("manual.html", 'w') as outfile:
        outfile.write("<html><head><title>Manual</title></head><body><h1>Manual</h1><p>Press the reset button on the MSP432</p></body></html>")

    serverProcess = multiprocessing.Process(target=server.run)
    serverProcess.start()

    openSer()

    while(1):
        awaitResponse(2)
        gameJson = generate.generateGame()
        generate.genManual()
        #stringify json
        msg = json.dumps(gameJson)
        sendMessagge(msg, 2)

    close()
    serverProcess.terminate()

if __name__ == '__main__':
    processJobs()