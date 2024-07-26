int delayTime = 200;
static uint32_t lastestPinDIRConfig_A = 0x00000000;
static uint32_t lastestPinOUTConfig_A = 0x00000000;
static uint32_t lastestPinDIRConfig_B = 0x00000000;
static uint32_t lastestPinOUTConfig_B = 0x00000000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void Reset_Pins()
{
  PORT->Group[0].DIR.reg=0x00000000;
  PORT->Group[1].DIR.reg=0x00000000;
  PORT->Group[0].OUT.reg=0x00000000;
  PORT->Group[1].OUT.reg=0x00000000;
  //
  lastestPinDIRConfig_A = 0x00000000;
  lastestPinOUTConfig_A = 0x00000000;
  lastestPinDIRConfig_B = 0x00000000;
  lastestPinOUTConfig_B = 0x00000000;
}

uint32_t Pin_Translator(char PinNum, bool inverse)
{
  String tempPin;
  if (inverse == false)
  {
    for (int i = 0; i <= (31-(PinNum+1)); i++) 
    {
      tempPin = tempPin + "0";
    }

    tempPin = tempPin + "1";

    for (int i = 0; i < PinNum; i++) 
    {
      tempPin = tempPin + "0";
    }
  }
  else if (inverse == true)
  {
    for (int i = 0; i <= (31-(PinNum+1)); i++) 
    {
      tempPin = tempPin + "1";
    }

    tempPin = tempPin + "0";

    for (int i = 0; i < PinNum; i++) 
    {
      tempPin = tempPin + "1";
    }
  }

  uint32_t result = 0;
  for (unsigned int i = 0; i < tempPin.length(); ++i)
  {
    result = result << 1;
    result = result | (tempPin[i] & 1);
  }

  // Serial.println(result, HEX);
  return result;
}

void Config_GPIO(char PortNum, char PinNum, String Mode)
{
  int group;
  int allocatedConfig;

  // setting the mode
  if(Mode == "OUTPUT")
  {
    if(PortNum == 'A')
    {
      group = 0;

      // saving | getting the pin value and OR it to the current pin configuration
      lastestPinDIRConfig_A = lastestPinDIRConfig_A | Pin_Translator(PinNum, false);
      allocatedConfig = lastestPinDIRConfig_A;
    }
    else if(PortNum == 'B')
    {
      group = 1;

      // saving | getting the pin value and OR it to the current pin configuration
      lastestPinDIRConfig_B = lastestPinDIRConfig_B | Pin_Translator(PinNum, false);
      allocatedConfig = lastestPinDIRConfig_B;
    } 

    Serial.println("Set to output");

    Serial.println(allocatedConfig, HEX);
    PORT->Group[group].DIR.reg=allocatedConfig;
  }
  else if(Mode == "INPUT")
  {
    if(PortNum == 'A')
    {
      group = 0;

      // saving | getting inverse of the pin value and AND it to the current pin configuration
      lastestPinDIRConfig_A = lastestPinDIRConfig_A & Pin_Translator(PinNum, true);
      allocatedConfig = lastestPinDIRConfig_A;
    }
    else if(PortNum == 'B')
    {
      group = 1;

      // saving | getting inverse of the pin value and AND it to the current pin configuration
      lastestPinDIRConfig_B = lastestPinDIRConfig_B & Pin_Translator(PinNum, true);
      allocatedConfig = lastestPinDIRConfig_B;
    } 

    Serial.println("Set to input");

    Serial.println(allocatedConfig, HEX);
    PORT->Group[group].DIR.reg=allocatedConfig;
  }
}

void Write_GPIO(char PortNum, char PinNum, bool State)
{
  int group;
  int allocatedConfig;

  if(State == HIGH)
  {
    if(PortNum == 'A')
    {
      group = 0;

      // saving | getting the pin value and OR it to the current pin configuration
      lastestPinOUTConfig_A = lastestPinOUTConfig_A | Pin_Translator(PinNum, false);
      allocatedConfig = lastestPinOUTConfig_A;
    }
    else if(PortNum == 'B')
    {
      group = 1;

      // saving | getting the pin value and OR it to the current pin configuration
      lastestPinOUTConfig_B = lastestPinOUTConfig_B | Pin_Translator(PinNum, false);
      allocatedConfig = lastestPinOUTConfig_B;
    }

    Serial.println("Set to HIGH");

    Serial.println(allocatedConfig, HEX);
    PORT->Group[group].OUT.reg=allocatedConfig;
  }
  else if(State == LOW)
  {
    if(PortNum == 'A')
    {
      group = 0;

      // saving | getting inverse of the pin value and AND it to the current pin configuration
      lastestPinOUTConfig_A = lastestPinOUTConfig_A & Pin_Translator(PinNum, true);
      allocatedConfig = lastestPinOUTConfig_A;
    }
    else if(PortNum == 'B')
    {
      group = 1;

      // saving | getting inverse of the pin value and AND it to the current pin configuration
      lastestPinOUTConfig_B = lastestPinOUTConfig_B & Pin_Translator(PinNum, true);
      allocatedConfig = lastestPinOUTConfig_B;
    } 

    Serial.println("Set to LOW");

    Serial.println(allocatedConfig, HEX);
    PORT->Group[group].OUT.reg=allocatedConfig;
  }
}

void Read_GPIO(char PortNum, char PinNum)
{
  int allocatedConfig;

  // getting the mcu port type
  if(PortNum == 'A')
  {
    allocatedConfig = lastestPinOUTConfig_A;
  }
  else if(PortNum == 'B')
  {
    allocatedConfig = lastestPinOUTConfig_B;
  }

  Serial.println(allocatedConfig, BIN);

  int temp = allocatedConfig & Pin_Translator(PinNum, false);
  
  Serial.println(temp, BIN);
  temp = temp >> PinNum;
  Serial.println(temp, BIN);
  
  if (temp == 1)
  {
    Serial.println("Pin is ACTIVE");
  }
  else
  {
    Serial.println("Pin is INACTIVE");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  // -- Demo -- //
  // Serial.println("starting...");
  // delay(10000);
  // Config_GPIO('A', 2, "OUTPUT");
  // delay(10000);
  // Config_GPIO('B', 10, "OUTPUT");
  // delay(10000);
  // Write_GPIO('A', 2, HIGH);
  // Read_GPIO('A', 2);

  // -- PIN DIR Demo -- //
  // Serial.println("starting...");
  // delay(5000);
  // Config_GPIO('A', 1, "OUTPUT");
  // delay(5000);
  // Config_GPIO('A', 5, "OUTPUT");
  // delay(5000);
  // Config_GPIO('A', 1, "INPUT");
  // Serial.println("ending...");
  // delay(10000);
  // Reset_Pins();

  // -- PIN OUT Demo -- //
  // Reset_Pins();
  // Serial.println("starting...");
  // delay(5000);
  // Config_GPIO('A', 2, "OUTPUT");
  // delay(5000);
  // Config_GPIO('B', 2, "OUTPUT");
  // delay(5000);
  // Write_GPIO('A', 2, HIGH);
  // delay(5000);
  // Write_GPIO('B', 2, HIGH);
  // delay(5000);
  // Write_GPIO('A', 2, LOW);
  // Serial.println("ending...");
  // delay(10000);

  // -- Reading Demo -- //
  // Reset_Pins();
  // Serial.println("starting...");
  // delay(5000);
  // Config_GPIO('A', 2, "OUTPUT");
  // Config_GPIO('B', 2, "OUTPUT");
  // Config_GPIO('B', 10, "OUTPUT");
  // Write_GPIO('A', 2, HIGH);
  // Write_GPIO('B', 2, HIGH);
  // delay(5000);
  // Write_GPIO('B', 2, LOW);
  // delay(5000);
  // Serial.println("reading...");
  // Read_GPIO('B', 2);
  // delay(5000);
  // Serial.println("ending...");
  // delay(10000);
  
  // -- Blink -- //
  // Reset_Pins();
  // Config_GPIO('B', 11, "OUTPUT");
  // Write_GPIO('B', 11, HIGH);
  // delay(delayTime);
  // Write_GPIO('B', 11, LOW);
  // delay(delayTime);


  // -- Debug -- //
  // Serial.println(Pin_Translator(2, false), BIN);
  // Serial.println(Pin_Translator(2, true), BIN);

}
