# Readme for Hardware

Physical changes to hardware (ex 3drag)

Theory of operation, Wiring, addtional electonics.

---
## Challenges
1. Limited Hardware

---
## Multiple Tool control from limited hardware.

GRBL as standard does not cater for 3+ axis.

but we will have 4 additional motors to control...

- Movement of tools on gantry ( tool changer)
- Each tool's Z axis drive
    - 0.8mm drill
    - 1.0 mm drill
    - laser module

### Design

Multi-tool control.
By default GRBL is a 3 axis parser, X Y & Z.
In our application, the Z axis will only be moved once the X & Y axis are finished moving.  ( focus laser, drilling movement )
Also, the Tool gantry and individual tools' height will never be changed at the same time.

This will allow us to use the Z Axis commands to control all the tools and gantry.

- Retaining positional information: the Z axis ( tool gantry ) will be homed and then commanded with normal gcode movement commands to position the correct tool.
- When we control the actual tools themselves, we will switch to alternate work co-ordinates ( having the Z axis at zero) - when we are finished with the tool, we will return the tool to the zero position and reset work co-ordinate system to the initial co-ords.

Electronics & wiring.
The challenge is to control 4 stepper motors with a single step/dir set of signals...

We will specify which one of 4 motors we want to drive by using 4 GPIO pins ( see table below)

Normally we would use the EN pins on the drivers so only the correct stepper is moved.
However pulling the EN Pin to ground, removes all holding torque from the motors, possibly allowing tools or axis to move due to vibration / weight.

So stepper drivers will always be enabled, and the step signal pulses will be passed through only to the selected driver ( basically control pin AND to the Step signal)

The Direction pins will be always connected to all the tool stepper drivers, as the dir pin is only important if you are actually moving. so only the driver receiving the step pulses will "use" the dir pin, all the other drivers will be stationary.

The enable Pin will also be used in conjuction with the PWM signals from the board to control the speed / power of the selected tool.
the non-selected tools will in effect receive a 0 volt signal, while the selected tool will be reciving the desired power signal.

| Ref | Motor Function | I/O Pin | GCODE | Comment |
|--- |  ------ | ------- | ------ | -------- |
| A | Tool gantry movement | A3 | M07 |  *FLOOD ON*   GCODE M09 switches off  |
| B | Drill 0.8 Z Axis (drill down & up) | A4 | M08 | *MIST ON*    GCODE M09 Switches off|
| C | Drill 1.0 Z Axis (drill down & up) | A6 | ??? | Custom M code  - To be added to Firmware  ? M09 switches off? |
| D | Laser Module Z Axis (focus distance to work ) | A7 | ???? | Custom M code  - To be added to Firmware ? M09 switches off? |

---

## Gantry design & Tool positioning

The Gantry will be a section of 'C' V-SLOT aluminim extrusion, fixed to the machines vertical uprights.
mounted on this Section, will be a single sliding plate, that is wide enough to mount all three tools.  This plate will be moved left to right to position the plate so that relevant tool is in the correct working position by a stepper motor *REF A*

Each tool on the plate will have a pre-set offset from the Z Home, that will position the specific tool at the normal active position.

```
        M07     ;   Enable Tool Gantry
        G28 Z   ;   Home Gantry
        G92 Z0  ;   set home pos as 0
        G1 Z100 F1000   ;   move gantry for Tool # 1 ( for e.g. )
        M09     ;   Switch off Tool Gantry Pin
        M08     ;   Enable Tool # 1 movement pin
        G55     ;   Set work co-ord for tool # 1 ( this will also set the x & y offset of the tool from the gantry ) work co-ords now X0 Y0 Z0
        M03 S124    ;   set half power on tool # 1 ( spindle speed or laser power)
        ;  continue with gcodes for tool #1
        ;
        ;  Work completed for tool # 1
        M05     ;   tool power to 0
        G0 X0 Y0 Z0 ;   return tool to home position;
        G54     ; return to machine co-ordinates ( i.e. Z is not back to 100 )
        ;
        ; select tool # 2 and position
        M07     ;   Enable Tool Gantry movement
        G1 Z260 F1000   ; ( move gantry so that tool #2 is in the correct position )
        M09     ;
        M200    ;   enable tool # 2 step pin
        G56     ;   set co-ord system for tool # 2
        M03 S255    ;   full power on tool #2
        ;
        ;   continue with tasks for tool #2
        ;
        ;   work completed for tool #2
        M05     ;   power off tool
        G0 X0 Y0 Z0 ;   return to tool #2 home
        G54     ; return to machine co-ords
```


## Spindle design

## Laser mount design