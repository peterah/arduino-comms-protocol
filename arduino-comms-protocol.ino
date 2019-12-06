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

    For example:  write digital pin 13 high: 1.2.13.1.
                  read analogue pin A0: 2.1.0.
*****************************************************************************/

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
  delay(200);
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
      break; 
    }
  case 1:       //write analog/digital
    {
      getSerial();
      switch (serialData)
      {
      case 1:   //write analog (PWM)
        {
          getSerial();
          pinNumber = serialData;
          getSerial();
          pwmVal = serialData;
          pinMode(pinNumber, OUTPUT);
          analogWrite(pinNumber, pwmVal);
          
          Serial.print("Write: PWM: Pin ");
          Serial.print(pinNumber);
          Serial.print(" = ");
          Serial.println(pwmVal);
          
          pinNumber = 0;
          break;
        }
      case 2:       //write digital
        {
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
          Serial.print("Write: Digital: Pin ");
          Serial.print(pinNumber);
          Serial.print(" = ");
          Serial.println(digitalState);
          
          pinNumber = 0;
          break;
        }
     }
     break; 
    }
   case 2:        //read
    {
      getSerial();
      switch (serialData)
      {
      case 1:     //read analogue
        {
          int x = 0;
          getSerial();
          pinNumber = serialData;
          pinMode(pinNumber, INPUT);
          sensorVal = analogRead(pinNumber);  //read adc channel and then clear variable to remove any latent charge
          sensorVal = 0;
          for (int i = 0; i <= 2; i++){       //3 point averaging filter
              sensorVal = sensorVal + analogRead(pinNumber);
          }
          sensorVal = sensorVal / 3;
          
          Serial.print("Read: Analogue: Pin ");
          Serial.print(pinNumber);
          Serial.print(" = ");
          Serial.println(sensorVal);
          
          sensorVal = 0;
          pinNumber = 0;
          break;
        } 
      case 2:     //read digital
        {
          getSerial();
          pinNumber = serialData;
          pinMode(pinNumber, INPUT);
          sensorVal = digitalRead(pinNumber);
          
          Serial.print("Read: Digital: Pin ");
          Serial.print(pinNumber);
          Serial.print(" = ");
          Serial.println(sensorVal);

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
