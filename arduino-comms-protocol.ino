/*****************************************************************************
Description: General communications protocol for communicating with
             analog inputs and digital IO on the Arduino Uno platform.
             
             Optional: serial LCD connected to pin 12.

Version:     1.0    13/05/2015    Initial working protocol
             1.1    05/06/2015    Added serial lcd output on pin D12
             
Programmer:  P Howells
******************************************************************************
Protocol:
    Numbers separated by a . 
    
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

    For example write digital pin 13 high: 1.2.13.1.            
*****************************************************************************/

//#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

//SoftwareSerial lcd(11, 12);  //Rx, Tx
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


double FW_Version = 1.1;
unsigned long serialData;
int inByte;
int digitalState;
int pinNumber;
int pwmVal;
int sensorVal;

void setup()
{
  Serial.begin(115200);
  //Serial lcd init
  //lcd.begin(9600);
  //delay(200);
  //lcd.write(254); // move cursor to beginning of first line
  //lcd.write(128);
  //lcd.write("                ");
  //lcd.write("                ");
  //lcd.write("Comms Protocol: ");
  //lcd.write("Firmware v1.1");
  
  //standard lcd init
  lcd.begin(16, 2);
  lcd.print("Comms Protocol:");
  lcd.setCursor(0, 1);
  lcd.print("Firmware v1.1");
}

void loop()
{
  getSerial();
  switch(serialData)
  {
  case 0:
    {
      Serial.print("Firmware Version: ");
      Serial.println(FW_Version);
      /*
      lcd.write(254); // move cursor to beginning of first line
      lcd.write(128);
      lcd.write("                ");
      lcd.write("                ");
      lcd.write("Comms Protocol: ");
      lcd.write("Firmware v1.1");
      */
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Comms Protocol:");
      lcd.setCursor(0, 1);
      lcd.print("Firmware v1.1");
      break; 
    }
  case 1:
    {
      //write analog/digital
      getSerial();
      switch (serialData)
      {
      case 1:
        {
          //write analog
          getSerial();
          pinNumber = serialData;
          getSerial();
          pwmVal = serialData;
          pinMode(pinNumber, OUTPUT);
          analogWrite(pinNumber, pwmVal);
          /*
          lcd.write(254); // move cursor to beginning of first line
          lcd.write(128);
          lcd.write("                ");
          lcd.write("                ");
          lcd.write("Analog Write:   ");
          lcd.write("Pin ");
          lcd.print(pinNumber);
          lcd.write(" = ");
          lcd.print(pwmVal);
          */
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Analog Write:");
          lcd.setCursor(0, 1);
          lcd.print("Pin ");
          lcd.print(pinNumber);
          lcd.print(" = ");
          lcd.print(pwmVal);
          
          pinNumber = 0;
          break;
        }
      case 2:
        {
          //write digital
          getSerial();
          pinNumber = serialData;
          getSerial();
          digitalState = serialData;
          pinMode(pinNumber, OUTPUT);
          if (digitalState == 0)
          {
            digitalWrite(pinNumber, LOW);
          }
          if (digitalState == 1)
          {
            digitalWrite(pinNumber, HIGH);
          }
          /*
          lcd.write(254); // move cursor to beginning of first line
          lcd.write(128);
          lcd.write("                ");
          lcd.write("                ");
          lcd.write("Digital Write:  ");
          lcd.write("Pin ");
          lcd.print(pinNumber);
          lcd.write(" = ");
          lcd.print(digitalState);
          */
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Digital Write:");
          lcd.setCursor(0, 1);
          lcd.print("Pin ");
          lcd.print(pinNumber);
          lcd.print(" = ");
          lcd.print(digitalState);
          
          pinNumber = 0;
          break;
        }
     }
     break; 
    }
   case 2:
    {
      getSerial();
      switch (serialData)
      {
      case 1:
        {
          //analog read
          int x = 0;
          getSerial();
          pinNumber = serialData;
          pinMode(pinNumber, INPUT);
          sensorVal = analogRead(pinNumber);  //read adc channel and then clear variable to remove any latent charge
          sensorVal = 0;
          for (int i = 0; i <= 2; i++){  //3 sample averaging filter
              sensorVal = sensorVal + analogRead(pinNumber);
          }
          sensorVal = sensorVal / 3;
          Serial.println(sensorVal);
          /*
          lcd.write(254); // move cursor to beginning of first line
          lcd.write(128);
          lcd.write("                ");
          lcd.write("                ");
          lcd.write("Analog Read:    ");
          lcd.write("Pin ");
          lcd.print(pinNumber);
          lcd.write(" = ");
          lcd.print(sensorVal);
          */
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Analog Read:");
          lcd.setCursor(0, 1);
          lcd.print("Pin ");
          lcd.print(pinNumber);
          lcd.print(" = ");
          lcd.print(sensorVal);
          
          sensorVal = 0;
          pinNumber = 0;
          break;
        } 
      case 2:
        {
          //digital read
          getSerial();
          pinNumber = serialData;
          pinMode(pinNumber, INPUT);
          sensorVal = digitalRead(pinNumber);
          Serial.println(sensorVal);
          /*
          lcd.write(254); // move cursor to beginning of first line
          lcd.write(128);
          lcd.write("                ");
          lcd.write("                ");
          lcd.write("Digital Read:   ");
          lcd.write("Pin ");
          lcd.print(pinNumber);
          lcd.write(" = ");
          lcd.print(sensorVal);
          */
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Digital Read:");
          lcd.setCursor(0, 1);
          lcd.print("Pin ");
          lcd.print(pinNumber);
          lcd.print(" = ");
          lcd.print(sensorVal);
          
          sensorVal = 0;
          pinNumber = 0;
          break;
        }
      }
      break;
    }
  }
}

long getSerial()
{
  serialData = 0;
  while (inByte != '.')
  {
    inByte = Serial.read(); 
    if (inByte > 0 && inByte != '.')
    {
      serialData = serialData * 10 + inByte - '0';
    }
  }
  inByte = 0;
  return serialData;
}
