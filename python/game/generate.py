import random
import os

#vars to generate the manual
numpadSNRef = []
caviSNRef = []
notNotColorRef = []
simonSaysLookUpTableRef = []

def genCavi(SN):
    # generate a random cavi based on the serial number
    cavi = []
    SNlen = len(SN)
    caviSNRef.append(random.randint(0, SNlen - 1))
    if SN[caviSNRef[0]] in "AEIOU":
        cavi.append(1)
    else:
        cavi.append(0)
    caviSNRef.append(random.randint(0, SNlen - 1))
    if SN[caviSNRef[1]] in "0123456789":
        if int(SN[caviSNRef[1]]) % 2 == 0:
            cavi.append(1)
        else:
            cavi.append(0)
    else:
        cavi.append(1)
    caviSNRef.append(0)
    if SN[caviSNRef[2]] in "ACEGIKMOQSUWY":
        cavi.append(1)
    else:
        cavi.append(0)
    caviSNRef.append(random.randint(0, SNlen - 1))
    if SN[caviSNRef[3]] in "0123456789":
        if int(SN[caviSNRef[3]]) % 2 == 0:
            cavi.append(0)
        else:
            cavi.append(1)
    else:
        cavi.append(0)
    return cavi

def genNumPad(SN):
    # generate a random numPad
    numPad = ""
    snlen = len(SN)
    for i in range(random.randint(4, 10)):
        n = random.randint(0, snlen - 1)
        while (SN[n] not in "0123456789ABCD*#"):
            n = random.randint(0, snlen - 1)
        numpadSNRef.append(n)
        numPad += SN[n]
    return numPad

def genNotNot():
    numTurni = random.randint(6, 15)
    notNotColors = ["red", "blue", "green", "yellow", "purple", "orange", "white", "black"]
    messaggi = [] # composed of N negations and then the color
    mosse = [] # composed of the correct possible moves
    
    for i in range(numTurni):
        #generate 4 random colors
        mosseTurno = []
        colors = []
        for j in range(4):
            colors.append(notNotColors[random.randint(0, len(notNotColors) - 1)])
        #save colors in the reference
        notNotColorRef.append(colors)
        #generate the message
        numNeg = random.randint(0, 3)
        messaggio = "".join("not " for _ in range(numNeg))
        numColor = random.randint(0, 3)
        messaggio += colors[numColor]
        for j in range(4):
            if (numNeg == 2 or numNeg==0):
                if j==numColor:
                    mosseTurno.append(1)
                else:
                    mosseTurno.append(0)
            else:
                if j==numColor:
                    mosseTurno.append(0)
                else:
                    mosseTurno.append(1)
        messaggi.append(messaggio)
        mosse.append(mosseTurno)
    notNot = {
        "messaggi": messaggi,
        "mosse": mosse
    }
    return notNot

def genSimonSays():
    #for each turn use a table with all the possible combinations of leds and the associated moves
    #use it to generate the led matrix and the moves
    #save the table to be used in the manual
    global simonSaysLookUpTableRef
    numTurni = random.randint(4, 10)
    lookUpTables = []
    for i in range (0, numTurni):
        turnTable = []
        #generate a random 3x3 0-1 matrix and save it in the table
        for j in range(0, 20):
            alreadyIn = True
            mat = []
            while alreadyIn:
                mat = [[random.randint(0, 1) for _ in range(3)] for _ in range(3)]
                #check if the matrix is already in the table
                alreadyIn = False
                for k in range(0, len(turnTable)):
                    if mat == turnTable[k]["mat"]:
                        alreadyIn = True
                        break
            turnTable.append({"mat": mat, "move":random.randint(1, 9)})
        lookUpTables.append(turnTable)
    simonSaysLookUpTableRef = lookUpTables
    #generate the game
    matrix = []
    moves = []
    for i in range(0, numTurni):
        movesTurno = []
        matTurno = []
        numStep = random.randint(4, 8)
        for j in range(0, numStep):
            move = random.randint(0, 19)
            movesTurno.append(lookUpTables[i][move]["move"])
            #convert the mat in 8x8 like follows and encode each row as a char
            mat8x8 = []
            for k in range(0, 8):
                mat8x8.append([])
                for l in range(0, 8):
                    mat8x8[k].append(0)
            for k in range(0, 3):
                for l in range(0, 3):
                    if lookUpTables[i][move]["mat"][k][l] == 1:
                        for m in range(0, 2):
                            for n in range(0, 2):
                                if(k*3+m<8 and l*3+n<8):
                                    mat8x8[k*3+m][l*3+n] = 1
            #encode the matrix
            chrArray = []
            for k in range(0, 8):
                chrArray.append(0)
                for l in range(0, 8):
                    chrArray[k] += mat8x8[k][l] * 2**(l)
            matTurno.append(chrArray)
        matrix.append(matTurno)
        moves.append(movesTurno)
    simonSays = {
        "ledAccesi": matrix,
        "mosse": moves
    }
    return simonSays

def generateGame():
    # generate a random serial number
    global SN
    SN = ''.join(random.choice("ABCD0123456789") for _ in range(0,13))
    # generate a random time
    TIME = random.randint(120, 240)
    # generate a random cavi
    cavi = genCavi(SN)
    numPad = genNumPad(SN)
    # generate a random notNot
    notNot = genNotNot()
    # generate a random simonSays
    simonSays = genSimonSays()

    game = {
        "SN": SN,
        "TIME": TIME,
        "cavi": cavi,
        "numPad": numPad,
        "notNot": notNot,
        "simonSays": simonSays
    }
    print("game generated")
    return game

def genManual():
    # header
    manual = """
    <html>
    <head>

    <style>
    html {
        display: table;
        margin: auto;
    }
    body {
    display: table-cell;
    vertical-align: middle;
    font-family: Arial, Helvetica, sans-serif;
    }
    table, th, td {
        border: 1px solid black;
        border-collapse: collapse;
    }
    th, td {
        padding: 5px;
        text-align: left;
    }
    .square {
        height: 100px;
        width: 100px;
        border: 5px solid;
        align: center;
    }
    </style>

    <script>
        var simon = -1;
        var notNot = -1;
        function showSimon(id){
            for (let i = 0; i < """+f"{len(simonSaysLookUpTableRef)}"+"""; i++) 
            {
                document.getElementById("Simon"+i).style.display = "none";
            }
            document.getElementById("Simon"+id).style.display = "block";
            simon = id;
            if (id == """+f"{len(simonSaysLookUpTableRef)-1}"+"""){
                document.getElementById("nextSimon").disabled = true;
            }else{
                document.getElementById("nextSimon").disabled = false;
            }
        }

        function showNotNot(id){
            for (let i = 0; i < +"""+f"{len(notNotColorRef)}"+"""; i++)
        {
                document.getElementById("NotNot"+i).style.display = "none";
            }
            document.getElementById("NotNot"+id).style.display = "block";
            notNot = id;
            if (id == """+f"{len(notNotColorRef)-1}"+"""){
                document.getElementById("nextNotNot").disabled = true;
            }else{
                document.getElementById("nextNotNot").disabled = false;
            }
        }
    </script>
    
    </head>
    <body class="content-center">
    <h1>Keep Talking and Nobody Explodes</h1>
    <h2>Manual</h2>
    """

    # cable cut
    manual += f"""
    <h3>Cables</h3>
    <h4>blue</h4>
    <p>if the {caviSNRef[0]+1} digit of the serial number is not a vowel, cut the wire</p>
    <h4>yellow</h4>
    <p>if the {caviSNRef[1]+1} digit of the serial number is a number, cut the wire if the number is even</p>
    <h4>green</h4>
    <p>if the {caviSNRef[2]+1} digit of the serial number is an even letter (ACEGIKMOQSUWY), cut the wire</p>
    <h4>purple</h4>
    <p>if the {caviSNRef[3]+1} digit of the serial number is a number cut the wire if the number is odd, otherwise cut the wire</p>
    """

    # keypad
    manual += "<h3>Keypad</h3>"
    for i in range(len(numpadSNRef)):
        manual += f"<p>if the {i+1} digit of the code is the {numpadSNRef[i]+1} char of the SN</p>"

    # not not
    manual += """
    <h3>Not Not</h3>
    <p>This is the cube state for every turn</p>
    <div class="dropdown">
        <button class="btn btn-primary dropdown-toggle" type="button" data-toggle="dropdown">Turn
        <span class="caret"></span></button>
        <ul class="dropdown-menu">
    """
    for i in range(len(notNotColorRef)):
        manual += f"<li ><a onclick=\"showNotNot({i})\">Turn {i+1}</a></li>"
    manual += """
        </ul>
    </div>
    <button onclick="showNotNot(0)" class="btn btn-primary"><span class="glyphicon glyphicon-repeat"></span></button>
    <button onclick="showNotNot(notNot+1)" class="btn btn-primary" id="nextNotNot"><span class="glyphicon glyphicon-forward"></span></button>
    """
    #the cube state is shown as a square with the sides colored as the colors in the list
    for i in range(len(notNotColorRef)):
        manual += f"""
        <div id="NotNot{i}" style="display: none">
        <h5>Turn {i+1}</h5>
        <div class="square" style="border-top-color: {notNotColorRef[i][0]}; border-right-color: {notNotColorRef[i][1]}; border-bottom-color: {notNotColorRef[i][2]}; border-left-color: {notNotColorRef[i][3]}"></div>
        </div>
        """
    
    # simon says
    #use simonSaysLookUpTableRef to show the moves and the matrix
    manual += "<h3>Simon Says</h3>"
    #add dropdown menu to show the moves and the matrix
    manual += """
    <div class="dropdown">
        <button class="btn btn-primary dropdown-toggle" type="button" id="dropdownMenuButton"
            data-bs-toggle="dropdown" aria-haspopup="true" aria-expanded="false">
            Seleziona turno <span class="caret"></span>
        </button>
        <ul class="dropdown-menu">
    """
    for i in range(0, len(simonSaysLookUpTableRef)):
        manual+=f"""
        <li>
            <a class="dropdown-item" onclick="showSimon({i})">
                Turn {i+1}
            </a>
        </li>
        """
    manual += """
    </ul>
    </div>
    <button class="btn btn-primary" type="button" onclick="showSimon(0)"><span class="glyphicon glyphicon-repeat"></span></button>
    <button class="btn btn-primary" type="button" onclick="showSimon(simon+1)" id="nextSimon"><span class="glyphicon glyphicon-forward"></span></button>
    """
    for i in range(len(simonSaysLookUpTableRef)):
        manual += f"""
        <div id="Simon{i}" style="display: none">
        <h4>Turn {i+1}</h4>
        <table class="table table-bordered row">
        <tr>
        <th>Move</th>
        <th>Matrix</th>
        </tr>
        """
        for j in range(len(simonSaysLookUpTableRef[i])):
            manual += f"""
            <tr>
            <td>{simonSaysLookUpTableRef[i][j]["move"]}</td>
            <td>
            <table class="table table-bordered col">"""
            for k in range(3):
                manual += "<tr>"
                for l in range(3):
                    manual += "<td>"
                    if simonSaysLookUpTableRef[i][j]["mat"][k][l] == 0:
                        #empty square
                        manual += "O"
                    else:
                        manual += "X"
                    manual += "</td>"
                manual += "</tr>"
            manual +="</table> </td> </tr>"

        manual += """
        </table>
        </div>"""

    # footer with bootstrap
    manual+="""
    <br><br>
    <!-- Jquery -->
    <script src="https://code.jquery.com/jquery-3.6.0.min.js" ></script>
    <!-- Bootstrap -->
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.9.2/dist/umd/popper.min.js" integrity="sha384-IQsoLXl5PILFhosVNubq5LC7Qb9DXgDA9i+tQ8Zj3iwWAwPtgFTxbJ8NT4GN1R8p" crossorigin="anonymous"></script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.min.js" integrity="sha384-cVKIPhGWiC2Al4u+LWgxfKTRIcfu0JTxR+EQDz/bgldoEyl4H0zUF0QKbrJ0EcQF" crossorigin="anonymous"></script>
    <script src="https://gitcdn.github.io/bootstrap-toggle/2.2.2/js/bootstrap-toggle.min.js"></script>

    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/css/bootstrap.min.css">
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/js/bootstrap.min.js"></script>
    </body>
    </html>
    """

    # save the manual in a html file
    with open("manual.html", 'w', encoding="utf8") as outfile:
        outfile.write(manual)
    print("manual.html created")
    
random.seed()
