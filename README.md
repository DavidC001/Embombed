# Keep talking and nobody explodes

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

the circuit diagram is avaible at the following link:
![MSP432 circuit diagram - schematic](https://user-images.githubusercontent.com/40665241/218135720-05fd87a3-022a-4da2-96a1-655c2c376dc8.svg)

### Software Requirements

Code Composer Studio setted up to compile for the MSP432P401R

## Project Layout

### Source Code Organization

The project is separated into 3 main categories:
- drivers (libs): they interface with the hardware directly, they are used to have a higer level of abstraction to work with (they have been designed to suit this particular project). To test other parts of the project those have been emulated using parallel threads.
- games: they implement the modules game logics and loops
- main: main script to start the program

A visualization in modules of the project is avaible at the following link:
![MSP432 circuit diagram - class diagram](https://user-images.githubusercontent.com/40665241/218137124-db3e8991-c1d4-4d16-9f7b-4af2d4c6d502.svg)

## Building, Burning, and Running

The only resources needed to run the project are in the following directories:
- "python server": contains the python script main.py to be run on the machine connected to the MSP (ports are for windows11, you may need to change them), it will also create a webpage acessible via port 8080
- "libs" and "games": those folders need to be added to the include search path
- "main.c": this is the main script to run for the program to start

## Presentation and Video

- PowerPoint Presentation: [Link to PowerPoint presentation]
- YouTube Video: [Link to YouTube video]

## Team Members

### Davide Cavicchini
