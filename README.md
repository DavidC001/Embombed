# Embombed

## Requirements

### Hardware Requirements

- MSP432P401R
- Educational boosterpack MKII
- 8×8 LED Matrix MAX7219
- 4x4 Membrane Switch
- 3x leds
- LCD Screen (compatible with Hitachi HD44780 driver)
- resistors:
  * 100 Ω x 1
  * 220 Ω x 1
  * 330 Ω x 3
  * 1  KΩ x 2
  * 2  KΩ x 2
  * 10 KΩ x 1

the circuit diagram is the following:
![MSP432 circuit diagram - schematic](https://user-images.githubusercontent.com/40665241/218135720-05fd87a3-022a-4da2-96a1-655c2c376dc8.svg)

### Software Requirements

Code Composer Studio setted up to compile for the MSP432P401R

## Project Layout

### File Structure

The project is organized in the following way:
```
  ├── libs               # drivers used in the project
  │   ├── emulators      # emulators for the drivers to test the project without the hardware
  │   └── ...
  ├── lib use examples   # programs used to test the correct functioning of the drivers and the hardware to run on the MSP, they also show how to use the drivers
  │   └── ...
  ├── games              # game modules
  │   └── ...
  ├── main.c             # main script to run
  |
  ├── testing files      # files used to test the different parts of the project
  |   ├── makefile       # makefile to compile the files with the includes and defines needed
  │   └── ...
  ├── msp                # files needed to run the tests
  |   └── ...
  |
  ├── python server
  │   ├── main.py        # python script to run on the machine connected to the MSP
  │   └── ...
  ├── python dev         # files used to develop the python script
  │   └── ...
  |
  ├── development_process_spec.pdf        # file compiled during the development process containing the general thoughts and the decisions taken (not exhaustive for personal use, mostly drivers)
  ├── README.md          # this file
  ├── LICENSE            # license file
  └── ...

```

### Project Structure

The project useful files are organized in the following 3 categories:
- drivers (libs): they interface with the hardware directly, they are used to have a higer level of abstraction to work with (they have been designed to suit this particular project). To test other parts of the project those have been emulated using parallel threads.
- games: they implement the modules game logics and loops
- main: main script to start the program
- server: python script to run on the machine connected to the MSP that communicates using uart

A visualization in modules of the project is the following:
![MSP432 circuit diagram - class diagram](https://user-images.githubusercontent.com/40665241/218137124-db3e8991-c1d4-4d16-9f7b-4af2d4c6d502.svg)

## Building, Burning, and Running

### project
The only resources needed to run the project are in the following directories:
- "python server": contains the python script main.py to be run on the machine connected to the MSP (ports are for windows11, you may need to change them), it will also create a webpage acessible via port 8080 with the manual
- "libs" and "games": those folders need to be added to the include search path
- "main.c": this is the main script to run for the program to start

Create a CCS project for the MSP, copy the content of libs and games folder and the main.c file into the project. Then you can connect the MSP, flash the program and run main.py script contained in the python server folder.
At first nothing will happen, press the reset button on the MSP (as stated on the webpage at port 8080) and start playing.

### tests
you can run the tests in the "testing files" folder using the command:
```bash
make FILE="file_name.c"
```

## Presentation and Video

- PowerPoint Presentation: [Link to PowerPoint presentation]
- YouTube Video: [[Link to YouTube video]](https://youtu.be/mFVSnlVLkkg)

## Team Members

### Davide Cavicchini
