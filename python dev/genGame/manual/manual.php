<!-- uses ref.json to generate the manual -->
<?php 
    $ref = file_get_contents("ref.json");
    $ref = json_decode($ref, true);
    $caviSNRef = $ref["caviSNRef"];
    $numpadSNRef = $ref["numpadSNRef"];
    $notNotColorRef = $ref["cubeState"];
    $simonSaysLookUpTableRef = $ref["simonSaysLookUpTableRef"];
?>
<html>
    <head>
    <style>
    body {
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
        height: 50px;
        width: 50px;
        border: 5px solid;
    }
    .center {
        text-align: center;
    }
    </style>
    </head>
    <body>
        <h1>Keep Talking and Nobody Explodes</h1>
        <h2>Manual</h2>
        <h3>Cables</h3>
            <h4>blue</h4>
            <p>if the <?php echo caviSNRef[0]?> digit of the serial number is not a vowel, cut the wire</p>
            <h4>yellow</h4>
            <p>if the <?php echo caviSNRef[1]?> digit of the serial number is a number, cut the wire if the number is even</p>
            <h4>green</h4>
            <p>if the <?php echo caviSNRef[2]?> digit of the serial number is an even letter (ACEGIKMOQSUWY), cut the wire</p>
            <h4>purple</h4>
            <p>if the <?php echo caviSNRef[3]?> digit of the serial number is a number cut the wire if the number is odd, otherwise cut the wire</p>
        <h3>Keypad</h3>
            <p>press the <?php echo numpadSNRef[0]?> digit of the serial number</p>
            <p>press the <?php echo numpadSNRef[1]?> digit of the serial number</p>
            <p>press the <?php echo numpadSNRef[2]?> digit of the serial number</p>
            <p>press the <?php echo numpadSNRef[3]?> digit of the serial number</p>
        <h3>Not Not</h3>
    </body>
</html>