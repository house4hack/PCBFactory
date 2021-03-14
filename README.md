# PCBFactory
A major impediment to be able to realise our projects quickly is the process to manufacture the PCBs required.  Chinese companies like PCBWay and JLPCB provide exeptional service and low cost, but even with (for the industry) very short turn around times, this still means that projects are put on hold for a few weeks.


The danger of this delay is that it allows Squirrels[^1] to manifest!
To combat these Squirrels, we decided we needed a quick way to prototype PCBs, and the PCB Factory was born!
*Note, this project was the result of a squirrel, which just goes to show how insiduos these wretched beasts are!*

in addition to the Squirrels, this project is also the offspring of other dubios parents.

The physical Hardware started its life as a 3d Printer called 3drag (https://reprap.org/wiki/3drag). It was donated to House4Hack and after not getting any love for a while, it was repurposed as the base for our PCB Factory.

The Controlling Software is the next evolution/remix of a stalled project that combined vision and CNC control to automate the drilling of etched PCBs.

Unlike the above, the CNC control software for the Hardware is based on the excellent [GRBL project](https://github.com/gnea/grbl)

## Theory of Operation:
We start with a blank PCB, which we coat with a protective layer of paint [^2] or UV curable film. The PCBFactory will then burn away the protective layer around the tracks and leave a protective layer over the tracks so that when we etch the board, the protected areas are not etched away and only the unprotected areas are etched away. Furthermore it will be able to drill all the neccessary holes, and even mill the edges.

## Hardware, Firmware & Software
The relevant folders for these are linked below.

[Hardware](Hardware/Readme.md)

The Hardware is the skeleton and muscles of the factory.  The 3drag donor printer is relativley unique in 3d printer hardware as the bed moves in the X & Y but is fixed in the z direction.

This makes this a great foundatiion for the PCB Factory as the tools will remain stationary while the work object is moved below.

The intent is to fix the gantry and allow each tool to move independently in the Z direction.  To facilitate multiple tools, the tools themselves will be moved into the work position laterally on the gantry.

The custom fixtures needed i.e. z axis for spindles & laser, Gantry design etc is also located here.

[Firmware](Software/Readme.md)

The Firmware is the brain of the hardware, it receives commands from the software and directs the hardware how to act.
Entirely based on GRBL, the firmware will be modified to allow the control of multiple tools.

[Software](Software/Readme.md)

The Software is the part of the process that acccepts the PCB Gerber files, and translates these into instructions (GCODE) that the firmware can understand.


## Current features:

1. It can move in X,Y,Z

## Next steps:

- [x] Add enstops  * done 9/3/21 *
- [ ] Add a box below for the electronics.
- [x] Control laser power via PWM  * done 9/3/21 *


## Future development:
### MVP1:
1. A laser to burn away uv curable film or paint, so that we can etch the board.
2. A drill to drill holes for components.

### MVP2:
1. Be able to do a double sided board.

### MVPz:
1. Add a camera to calculate the rotation of the PCB.

### MVPy:
1. Have multiple drill heads with different sizes.

### MVPx:
1. Be able to do isolation routing of a PCB.

### MVPw:
1. Be able to do edge milling of a PCB.

[^1]: 'Squirrels' is the term used to describe the danger and temptation that newer, sexier projects become more attractive to the maker than the current project that has been delayed.  As in "Look! A Sqirrel!"

[^2]: Paint must be Acrylic Spray Paint - See allow to dry completely before lasering.  more coats == more lasing but better "potential" resistance to etching.  Lasing paint leaves carbonised paint residue, this **MUST** be removed before etching.  Possibe.
