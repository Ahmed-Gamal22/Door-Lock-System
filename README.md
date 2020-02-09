# Door-Lock-System
Door Locker Security System (Embedded Application).
Consists of two ECU’s, The first ECU called HMI responsible for interfacing with the user and the second
ECU called control ECU which is responsible for the system operations and control. In the project I useed the following drivers
Keypad, LCD, DC Motor, UART, Timer, I2C and External EEPROM.
----------------------------------------------------------------------------------------------------------------
1- HMI micro: 

                         
         
 				 | 7 | 8 | 9 | # |                  
 				 |---------------|                  -----------------------
 				 | 4 | 5 | 6 | * |                  | Enter New Pass:     |
 				 |---------------|      -------->   | *****               |
 				 | 1 | 2 | 3 |   |                  -----------------------
 				 |---------------|
 				 |-> | 0 |   |   |
 				 
 
  These Are the used Buttons in the 4*4 keypad.
 ----------------------------------------------------------------------------------------------------------------
 2- Control micro used for all checking and decision maker:
 
                ----------------------             ---------------
                |                    |             |             |
                |                    | ----------> |   EEPROM    | (Used for nonvolatile saving user password)
                |                    |             |             | 
                |        ECU         |             ---------------
                |                    |                        ---------------
                |                    |                        |             |
                |                    | ---------------------> |    MOTOR    | (To open and Close the Door)
                ----------------------                        |             | 
                            |                                 ---------------
                            |                          
                            |                   /|  / 
                            |----------------> ( |  -    (Security Alarm system)
                                                \|  \
