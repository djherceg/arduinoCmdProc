DjH_CmdProc library

Verzije
=======
14.12.2025 v0.2.0
Junction points:
- Za PlatformIO: folder ./lib/DjH_CmdProc je junction na folder ./src
- Za Arduino: folder  C:\Users\djordje\Documents\Arduino\libraries\DjH_CmdProc je junction na
                      C:\users\djordje\documents\github\arduinoCmdProc\src

11.11.2022 v1.5.0
- The arduinoCmdProc project is now separate from the rest of the arduinoConfig project.

25.4.2021. v1.4.1
- Start developing support for ESP32. 
  The first issue is to remove the dependency avr/pgmspace.h and to read pin names from RAM instead of PROGMEM.
  Arduino version still requires pin names in PROGMEM

TODO:
- Enable pin names to be stored either in PROGMEM or RAM on Arduino