# GRBL 1.1 Config Settings for PCB Factory v1

## HARDWARE
MKS DLC v2.0 Board)

## GRBL Version
1.1  (Uno)

## Config Change Log


| Setting | default value | Description |  Pre 9/3 | Changes 9/3 |
| -----------| ---------- | ----------- | --------- | ----------- |
| $0|10 | Step pulse, microseconds              | 30    |       |
| $1|25	|        Step idle delay, milliseconds  | 255   |       |
| $2|0	|        Step port invert, mask         |       |       |
| $3|0	|       Direction port invert, mask     |       |       |
| $4|0	|        Step enable invert, boolean    |       |       |
| $5|0	|        Limit pins invert, boolean     | 1     |       |
| $6|0	|        Probe pin invert, boolean      |       |       |
| $10|1	|        Status report, mask            | 2     |       |
| $11 | 0.01 | Junction deviation, mm           | 0.05  |       |
| $12|0.002	|    Arc tolerance, mm              |       |       |
| $13|0	 |       Report inches, boolean         | 0.002 |       |
| $20|0	 |       Soft limits, boolean           |       |       |
| $21|0	 |       Hard limits, boolean           |       |   1   |
| $22|1	 |       Homing cycle, boolean          | 0     |   1   |
| $23|0	 |       Homing dir invert, mask        | 1     |   6   |
| $24|25.000	|    Homing feed, mm/min        | 0.000 |   100 |
| $25|500.000	| Homing seek, mm/min           | 0.000 |   1000    |
| $26|250	    |    Homing debounce, milliseconds | 2  |       |
| $27|1.000	  |  Homing pull-off, mm            | -3.143 |  2   |
| $31|0.	|        Min spindle speed, RPM     |       |       |
| $32|0	    |    Laser mode, boolean            |       |       |
| $100|250.000	| X steps/mm                    | 200   |       |
| $101|250.000	| Y steps/mm                    | 200   |       |
| $102|250.000	| Z steps/mm                    | 8000  |       |
| $110|500.000	| X Max rate, mm/min            | 10000 |       |
| $111|500.000	| Y Max rate, mm/min            | 10000 |       |
| $112|500.000	| Z Max rate, mm/min            | 200   |       |
| $120|10.000	 |    X Acceleration, mm/sec^2  | 100   |       |
| $121|10.000	  |  Y Acceleration, mm/sec^2   | 100   |       |
| $122|10.000	  |  Z Acceleration, mm/sec^2   | 100   |       |
| $130|200.000	| X Max travel, mm              | 250   |       |
| $131|200.000	| Y Max travel, mm              | 250   |       |
| $132|200.000	| Z Max travel, mm              | 150   |       |

