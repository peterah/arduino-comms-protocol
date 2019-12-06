# arduino-comms-protocol
General serial comms protocol for accessing analogue and digital IO on an Arduino Uno.
Code can easily be modified for other microcontroller development boards.

Protocol:
    Numbers separated by a . (period)
    
    0 = firmware version (ie 0.)
    1 = Write
        1 = Analog write
            Pin #
                PWM Value (0 - 255)
        2 = Digital write
            Pin #
                0 = Low, 1 = High
        3 = Serial LCD (Pin 12)
    2 = Read
        1 = Analog read
            Pin #
        2 = Digital Read
            Pin #
            
    Examples: write digital pin 13 high: 1.2.13.1.
              read analogue pin A0: 2.1.0.
