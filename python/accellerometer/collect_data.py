import sys
sys.path.append(R"C:\Users\david\Dropbox\embed\project\git\Embombed\python\serial")
print(sys.path)
from seriale import sendMessagge, awaitResponse, close


#open csv file
with open('data.csv', 'a') as f:
    while(1):
        k = input("Inserisci dato da leggere (0▲-1▼-2←-3→-4invalid): ")
        for i in range(0, 100):
            sendMessagge("S", 2)
            r = awaitResponse(2)
            print(r)
            f.write(r+","+str(k)+"\n")

close()