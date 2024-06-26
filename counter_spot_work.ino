#include <TM1637Display.h>
#include <EEPROM.h>

const int autoInput = 10;

const int outputInterlock = 45;
int interlockDelay = 0;
int buzzerTrickInterlock = 0;


const int outputAlarmReset = 46;
int alarmResetDelay = 700;

const int CLK = 3; //Set the CLK pin connection to the display
const int DIO = 2; //Set the DIO pin connection to the display
const int spotCounterUpBtn = 12; // spot up count
const int spotCounterDownBtn = 22; // spot down count
float pressLength_spotCounterBtn_milliSeconds = 0;
int optionTwo_spotCounterBtn_milliSeconds = 500;
int optionThree_spotCounterBtn_milliSeconds = 1500;

const int spot_swapDigit = 47;
float pressLength_spotCounterTargetBtn_milliSeconds = 0;

const int resetBtn = 8;
float pressLength_milliSeconds = 0;
int optionTwo_milliSeconds = 2000; 

const int DIO2 = 7;
const int CLK2 = 5;
const int spotCounterTargetUpBtn = 13; // spot target up
const int spotCounterTargetDownBtn = 23; // spot target down
float pressLength_workCounterBtn_milliSeconds = 0;
int optionTwo_spotCounterTargetBtn_milliSeconds = 500;
int optionThree_spotCounterTargetBtn_milliSeconds = 1500;

const int DIO3 = 53;
const int CLK3 = 51;
const int workCounterUpBtn = 29;
const int workCounterDownBtn = 30;
int optionTwo_workCounterBtn_milliSeconds = 500;
int optionThree_workCounterBtn_milliSeconds = 1500;

const int DIO4 = 33;
const int CLK4 = 31;
const int workCounterTargetUpBtn = 41;
const int workCounterTargetDownBtn = 43;
const int manualSwitchInput = 39;
const int autoSwitchInput = 42;
float pressLength_workCounterTargetBtn_milliSeconds = 0;
int optionTwo_workCounterTargetBtn_milliSeconds = 500;
int optionThree_workCounterTargetBtn_milliSeconds = 1500;

int numCounter = 0;
int spotCounterState = 0;
int spotCounterTargetState = 0;
int resetState = 0;

int x = 0; // spotCounter 
int y = 0; // spotCounterTarget
int z = 0; // workCounter
int a = 0; // workCounterTarget

const uint8_t blank[] = {0x00, 0x00, 0x00, 0x00};

const int buzzer = 26;

TM1637Display display(CLK, DIO); //set up the 4-Digit Display.
TM1637Display display2(CLK2, DIO2); //set up the 4-Digit Display.
TM1637Display display3(CLK3, DIO3); //set up the 4-Digit Display.
TM1637Display display4(CLK4, DIO4); //set up the 4-Digit Display.

const uint8_t done[] = {
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G    // A
};

void setup()
{
  Serial.begin(9600);
  setPinMode();
  readEEPROM();
  set7segment();
  digitalWrite(buzzer,HIGH);
  digitalWrite(outputAlarmReset,HIGH);
  digitalWrite(outputInterlock,HIGH);
}

void loop()
{
    weldingInterLock(z == a && z != 0 && a != 0 ? false : true);
    autoMode();
    spotCounterTargetSegment();
    workCounterTargetSegment();
    workCounterSegment();
    spotCounterSegment(false); //Function นับจำนวน Spot เมื่อมีการ กด Button Spot + เพิ่มขึ้น
    resetButton();
    buzzerLimit();
}

void weldingInterLock(bool canReset){
    if(canReset == false && z == a && z != 0 && a != 0){
      if(digitalRead(resetBtn) != LOW) delay(interlockDelay); //interlock-delay
      digitalWrite(outputInterlock, HIGH);
    }else if (canReset == true){
      digitalWrite(outputInterlock,LOW);
      buzzerTrickInterlock=0;
    }else digitalWrite(outputInterlock, HIGH);
}

void alarmReset(){
  digitalWrite(outputAlarmReset, LOW);
  delay(alarmResetDelay);
  digitalWrite(outputAlarmReset, HIGH);
}

void autoMode(){
  while (digitalRead(autoSwitchInput) == LOW && digitalRead(autoInput) == LOW)
  {
    spotCounterSegment(true);
  }
  spotCounterSegment(false);
  while(
    ((digitalRead(spotCounterUpBtn) == LOW
    || digitalRead(spotCounterDownBtn) == LOW 
    || digitalRead(spotCounterTargetUpBtn) == LOW 
    || digitalRead(spotCounterTargetDownBtn) == LOW
    || digitalRead(workCounterUpBtn) == LOW
    || digitalRead(workCounterDownBtn) == LOW
    || digitalRead(workCounterTargetUpBtn) == LOW
    || digitalRead(workCounterTargetDownBtn) == LOW
    || digitalRead(resetBtn) == LOW
    ) && digitalRead(autoSwitchInput) == LOW) 
  ){
    display.setSegments(done , 2, 2);
    display2.setSegments(done , 2, 2);
    display3.setSegments(done ,2 ,2);
    display4.setSegments(done ,2 ,2);
  }
  display.showNumberDec(x);
    display2.showNumberDec(y);
    display3.showNumberDec(z);
    display4.showNumberDec(a);
}

// Function to write an int value to EEPROM
void writeIntToEEPROM(int address, long value) {

  EEPROM.put(address, highByte(value));      // Write the upper byte
  EEPROM.put(address + 1, lowByte(value));  // Write the lower byte
}
// Function to write an int value to EEPROM
void updateIntToEEPROM(int address, long value) {
  EEPROM.update(address, highByte(value));      // Write the upper byte
  EEPROM.update(address + 1, lowByte(value));  // Write the lower byte
}

void readEEPROM(){
    int highX = EEPROM.read(1);
    int lowX = EEPROM.read(2);
    x =  (highX << 8) | lowX; 
    int highY = EEPROM.read(3);
    int lowY = EEPROM.read(4);
    y =(highY << 8) | lowY;
    int highZ= EEPROM.read(5);
    int lowZ = EEPROM.read(6);
    z = (highZ << 8) | lowZ;
    int highA= EEPROM.read(7);
    int lowA = EEPROM.read(8);
    a = (highA << 8) | lowA;
}

void setPinMode(){
  pinMode(outputAlarmReset, OUTPUT);
  pinMode(outputInterlock, OUTPUT);
  pinMode(autoInput, INPUT_PULLUP);
  pinMode(spotCounterUpBtn, INPUT_PULLUP);
  pinMode(spotCounterDownBtn, INPUT_PULLUP);
  pinMode(spotCounterTargetUpBtn, INPUT_PULLUP);
  pinMode(spotCounterTargetDownBtn, INPUT_PULLUP);
  pinMode(workCounterUpBtn , INPUT_PULLUP);
  pinMode(workCounterDownBtn , INPUT_PULLUP);
  pinMode(workCounterTargetUpBtn, INPUT_PULLUP);
  pinMode(workCounterTargetDownBtn, INPUT_PULLUP);
  pinMode(autoSwitchInput, INPUT_PULLUP);
  pinMode(manualSwitchInput, INPUT_PULLUP);
  pinMode(resetBtn , INPUT_PULLUP);
  pinMode(buzzer , OUTPUT); 
}

void set7segment(){  
  display.setBrightness(0x0a);
  display2.setBrightness(0x0a); 
  display3.setBrightness(0x0a); 
  display4.setBrightness(0x0a); 
  display.showNumberDec(x); 
  display2.showNumberDec(y);
  display3.showNumberDec(z);
  display4.showNumberDec(a);
}


void spotCounterSegment(bool active){
  while((digitalRead(spotCounterUpBtn) == LOW && digitalRead(manualSwitchInput) == LOW) || (digitalRead(autoInput) == LOW && active))
  {
    // if(pressLength_spotCounterBtn_milliSeconds < optionThree_spotCounterBtn_milliSeconds) delay(100);
    if(pressLength_spotCounterBtn_milliSeconds == 0 && y != 0 || (pressLength_spotCounterBtn_milliSeconds >= optionThree_spotCounterBtn_milliSeconds && y != 0)){
      if(pressLength_spotCounterBtn_milliSeconds >= 35000 && !active){
        x+=10;
        if(x > y) x = y;
      }else if(pressLength_spotCounterBtn_milliSeconds == 0 && active){
        x++;
      }else if(!active) x++;
      display.showNumberDec(x);
      //EEPROM Check When have data is update EEPROM if not have data is write to address EEPROM
      if(EEPROM.read(1) > 0)
      {
        updateIntToEEPROM(1 , x);
      }
      else{
        writeIntToEEPROM(1, x);
      }
      //*********************************
      if(x >= y) {
        delay(50);
        x = 0;
        if(a != 0 && z != a) {
          z++;
          display3.showNumberDec(z);
          if(z == a) digitalWrite(outputInterlock, HIGH);
           //EEPROM Check When have data is update EEPROM if not have data is write to address EEPROM
          if(EEPROM.read(5) > 0)
          {
            updateIntToEEPROM(5, z);
          }
          else{
            writeIntToEEPROM(5, z);
          }
          //*********************************
        }
        display.showNumberDec(x);
         //EEPROM Check When have data is update EEPROM if not have data is write to address EEPROM
          if(EEPROM.read(1) > 0)
          {
            updateIntToEEPROM(1, x);
          }
          else{
            writeIntToEEPROM(1, x);
          }
          //*********************************
      }
    }
    pressLength_spotCounterBtn_milliSeconds = pressLength_spotCounterBtn_milliSeconds + 100;
  } 
  while((digitalRead(spotCounterDownBtn) == LOW && digitalRead(manualSwitchInput) == LOW))
  {
    if(pressLength_spotCounterBtn_milliSeconds < optionThree_spotCounterBtn_milliSeconds) delay(100);
    if(pressLength_spotCounterBtn_milliSeconds == 0 && x != 0 || (pressLength_spotCounterBtn_milliSeconds >= optionThree_spotCounterBtn_milliSeconds && x != 0)){
      if(x > 0) {
        if(pressLength_spotCounterBtn_milliSeconds >= 35000){
          x-=10;
        }else{
          x--;
        }
      }else x = 0;
      display.showNumberDec(x);
       //EEPROM Check When have data is update EEPROM if not have data is write to address EEPROM
          if(EEPROM.read(1) > 0)
          {
            updateIntToEEPROM(1, x);
          }
          else{
            writeIntToEEPROM(1, x);
          }
          //*********************************
    }
    pressLength_spotCounterBtn_milliSeconds = pressLength_spotCounterBtn_milliSeconds + 100;
  } 
  if((pressLength_spotCounterBtn_milliSeconds + 400) >= optionTwo_spotCounterBtn_milliSeconds){
    pressLength_spotCounterBtn_milliSeconds = 0;
  }else if(pressLength_spotCounterBtn_milliSeconds >= optionThree_spotCounterBtn_milliSeconds && !active){
    pressLength_spotCounterBtn_milliSeconds = 0;
  }
}

void workCounterSegment(){
  while(digitalRead(workCounterUpBtn) == LOW && digitalRead(manualSwitchInput) == LOW)
  {
    // if(pressLength_workCounterBtn_milliSeconds < optionThree_workCounterBtn_milliSeconds) delay(100);
    if(pressLength_workCounterBtn_milliSeconds == 0 || (pressLength_workCounterBtn_milliSeconds >= optionThree_workCounterBtn_milliSeconds)){
      if(z <= a && a != 0){
        if(pressLength_workCounterBtn_milliSeconds >= 35000){
          z += 10;
          if(z > a) {
            z = a;
          }
        }else z++;
        display3.showNumberDec(z);
        //EEPROM Check When have data is update EEPROM if not have data is write to address EEPROM
          if(EEPROM.read(5) > 0)
          {
            updateIntToEEPROM(5, z);
          }
          else{
            writeIntToEEPROM(5, z);
          }
          //*********************************
      }
    }
    pressLength_workCounterBtn_milliSeconds = pressLength_workCounterBtn_milliSeconds + 100;
  }
  while(digitalRead(workCounterDownBtn) == LOW && digitalRead(manualSwitchInput) == LOW)
  {
    if(pressLength_workCounterBtn_milliSeconds < optionThree_workCounterBtn_milliSeconds) delay(100);
    if(pressLength_workCounterBtn_milliSeconds == 0 || (pressLength_workCounterBtn_milliSeconds >= optionThree_workCounterBtn_milliSeconds)){
      if(z > 0){
        if(pressLength_workCounterBtn_milliSeconds >= 35000){
          z -= 10;
        }else z--;
      }else z = 0;
      display3.showNumberDec(z);
      //EEPROM Check When have data is update EEPROM if not have data is write to address EEPROM
          if(EEPROM.read(5) > 0)
          {
            updateIntToEEPROM(5, z);
          }
          else{
            writeIntToEEPROM(5, z);
          }
          //*********************************
    }
    pressLength_workCounterBtn_milliSeconds = pressLength_workCounterBtn_milliSeconds + 100;
  }
  
  if((pressLength_workCounterBtn_milliSeconds + 400) >= optionTwo_workCounterBtn_milliSeconds){
    pressLength_workCounterBtn_milliSeconds = 0;
  }else if(pressLength_workCounterBtn_milliSeconds >= optionThree_workCounterBtn_milliSeconds){
    pressLength_workCounterBtn_milliSeconds = 0;
  }
}

void spotCounterTargetSegment(){
  while(digitalRead(manualSwitchInput) == LOW && digitalRead(spotCounterTargetUpBtn) == LOW && digitalRead(resetBtn) != LOW){
    if(pressLength_spotCounterTargetBtn_milliSeconds < optionThree_spotCounterTargetBtn_milliSeconds) delay(100);
    if(pressLength_spotCounterTargetBtn_milliSeconds == 0 || (pressLength_spotCounterTargetBtn_milliSeconds >= optionThree_spotCounterTargetBtn_milliSeconds)){
        if(pressLength_spotCounterTargetBtn_milliSeconds >= 35000){
          y+=10;
        }else{
          y++;
        }
        display2.showNumberDec(y);
        //EEPROM Check When have data is update EEPROM if not have data is write to address EEPROM
          if(EEPROM.read(3) > 0)
          {
            updateIntToEEPROM(3, y);
          }
          else{
            writeIntToEEPROM(3, y);
          }
          //*********************************
      }
    pressLength_spotCounterTargetBtn_milliSeconds = pressLength_spotCounterTargetBtn_milliSeconds + 100;
  }
  while(digitalRead(manualSwitchInput) == LOW && digitalRead(spotCounterTargetDownBtn) == LOW && digitalRead(resetBtn) != LOW){
    if(pressLength_spotCounterTargetBtn_milliSeconds < optionThree_spotCounterTargetBtn_milliSeconds) delay(100);
    if(pressLength_spotCounterTargetBtn_milliSeconds == 0 || (pressLength_spotCounterTargetBtn_milliSeconds >= optionThree_spotCounterTargetBtn_milliSeconds)){
        if(y > 0){
          if(pressLength_spotCounterTargetBtn_milliSeconds >= 35000){
            y -= 10;
          }else y--;
        }else y = 0;
        display2.showNumberDec(y);
        //EEPROM Check When have data is update EEPROM if not have data is write to address EEPROM
          if(EEPROM.read(3) > 0)
          {
            updateIntToEEPROM(3, y);
          }
          else{
            writeIntToEEPROM(3, y);
          }
          //*********************************
      }
    pressLength_spotCounterTargetBtn_milliSeconds = pressLength_spotCounterTargetBtn_milliSeconds + 100;
  }
  if((pressLength_spotCounterTargetBtn_milliSeconds+400) >= optionTwo_spotCounterTargetBtn_milliSeconds){
      pressLength_spotCounterTargetBtn_milliSeconds = 0;
    }else if(pressLength_spotCounterTargetBtn_milliSeconds >= optionThree_spotCounterTargetBtn_milliSeconds){
      pressLength_spotCounterTargetBtn_milliSeconds = 0;
    }
  }
    
void workCounterTargetSegment(){
  while(digitalRead(manualSwitchInput) == LOW && digitalRead(workCounterTargetUpBtn) == LOW && digitalRead(resetBtn) != LOW){
    if(pressLength_workCounterTargetBtn_milliSeconds < optionThree_workCounterTargetBtn_milliSeconds) delay(100);
    if(pressLength_workCounterTargetBtn_milliSeconds == 0 || (pressLength_workCounterTargetBtn_milliSeconds >= optionThree_workCounterTargetBtn_milliSeconds)){
        if(pressLength_workCounterTargetBtn_milliSeconds >= 35000){
          a += 10;
        }else a++;
        display4.showNumberDec(a);
        //EEPROM Check When have data is update EEPROM if not have data is write to address EEPROM
          if(EEPROM.read(7) > 0)
          {
            updateIntToEEPROM(7, a);
          }
          else{
            writeIntToEEPROM(7, a);
          }
          //*********************************
      }
    pressLength_workCounterTargetBtn_milliSeconds = pressLength_workCounterTargetBtn_milliSeconds + 100;
  }
  while(digitalRead(manualSwitchInput) == LOW && digitalRead(workCounterTargetDownBtn) == LOW && digitalRead(resetBtn) != LOW){
    if(pressLength_workCounterTargetBtn_milliSeconds < optionThree_workCounterTargetBtn_milliSeconds) delay(100);
    if(pressLength_workCounterTargetBtn_milliSeconds == 0 || (pressLength_workCounterTargetBtn_milliSeconds >= optionThree_workCounterTargetBtn_milliSeconds)){
        if(a > 0){
          if(pressLength_workCounterTargetBtn_milliSeconds >= 35000){
            a -= 10;
          }else a--;
        }else a = 0;
        display4.showNumberDec(a);
        //EEPROM Check When have data is update EEPROM if not have data is write to address EEPROM
          if(EEPROM.read(7) > 0)
          {
            updateIntToEEPROM(7, a);
          }
          else{
            writeIntToEEPROM(7, a);
          }
          //*********************************
      }
    pressLength_workCounterTargetBtn_milliSeconds = pressLength_workCounterTargetBtn_milliSeconds + 100;
  }
  if((pressLength_workCounterTargetBtn_milliSeconds+400) >= optionTwo_workCounterTargetBtn_milliSeconds){
      pressLength_workCounterTargetBtn_milliSeconds = 0;
  }else if(pressLength_workCounterTargetBtn_milliSeconds >= optionThree_workCounterTargetBtn_milliSeconds){
      pressLength_workCounterTargetBtn_milliSeconds = 0;
  }
}

void resetButton(){
  while(digitalRead(resetBtn) == LOW && digitalRead(manualSwitchInput) == LOW)
  {
    delay(100);
    pressLength_milliSeconds = pressLength_milliSeconds + 100;
    if(pressLength_milliSeconds >= optionTwo_milliSeconds){
      if(z==a && z!=0 && a!=0) alarmReset();
      x = 0;
      z = 0;
      digitalWrite(buzzer,HIGH);
      display.showNumberDec(x);
      //EEPROM Check When have data is update EEPROM if not have data is write to address EEPROM
          if(EEPROM.read(1) > 0)
          {
            updateIntToEEPROM(1, x);
          }
          else{
            writeIntToEEPROM(1, x);
          }
          //*********************************
      display3.showNumberDec(z);
       //EEPROM Check When have data is update EEPROM if not have data is write to address EEPROM
          if(EEPROM.read(5) > 0)
          {
            updateIntToEEPROM(5, z);
          }
          else{
            writeIntToEEPROM(5, z);
          }
          //*********************************
      pressLength_milliSeconds = 0;
      weldingInterLock(true);
    } 
  }
}


void buzzerLimit(){
  // work = workTarget
  while (digitalRead(resetBtn) != LOW && z == a && z != 0 && a != 0)
  {
    digitalWrite(buzzer, LOW);
    delay(500);
    buzzerTrickInterlock++;
    if(buzzerTrickInterlock == 1){
      delay(interlockDelay);
      digitalWrite(outputInterlock, HIGH);
    }
  }
}
// void spotCounterTargetSegment(){
//   while(digitalRead(spotCounterTargetBtn) == HIGH && digitalRead(manual) == HIGH){
//     delay(100);
//     if(pressLength_spotCounterTargetBtn_milliSeconds == 0 && currentDigit == 3){
//       display2.showNumberDec(0 , false , 1 , 3);
//       digit+=1;
//       display2.showNumberDec(digit , false , 1 , 3);
//     }else if(pressLength_spotCounterTargetBtn_milliSeconds == 0 && currentDigit == 2){
//       display2.showNumberDec(0 , false , 1 , 2);
//       digit+=1;
//       display2.showNumberDec(digit , false , 1 , 2);
//     }
//     else if(pressLength_spotCounterTargetBtn_milliSeconds == 0 && currentDigit == 1){
//       display2.showNumberDec(0 , false , 1 , 1);
//       digit+=1;
//       display2.showNumberDec(digit , false , 1 , 1);
//     }
//     else if(pressLength_spotCounterTargetBtn_milliSeconds == 0 && currentDigit == 0){
//       display2.showNumberDec(0 , false , 1 , 0);
//       digit+=1;
//       display2.showNumberDec(digit , false , 1 , 0);
//     }
//     switchDigit += 100;
//     pressLength_spotCounterTargetBtn_milliSeconds = pressLength_spotCounterTargetBtn_milliSeconds + 100;
//   }

//   if(switchDigit >= 2000 && switchDigit < 3000){
//     switchDigit = 0;
//     if(currentDigit == -1) currentDigit = 4;
//     else {
//       currentDigit--;
//       y = y > 0 ? (String(digit) + String(y)).toInt() : digit;
//       digit = 0;
//     }
//   }

//   if(currentDigit != 4) {
//       display2.showNumberDec(digit , false , 1 , currentDigit);
//       delay(500);
//       display2.setSegments(0xff , 1 , currentDigit);
//       delay(500);
//       display2.showNumberDec(digit , false , 1 , currentDigit);
//   }

//   if(switchDigit >= 3000){
//     currentDigit = 4;
//     y = y > 0 ? (String(digit) + String(y)).toInt() : digit;
//     Serial.println(y);
//   }



//   if((pressLength_spotCounterTargetBtn_milliSeconds + 400) >= optionTwo_spotCounterTargetBtn_milliSeconds){
//     pressLength_spotCounterTargetBtn_milliSeconds = 0;
//     switchDigit = 0;
//   }
// }

// void spotCounterTargetSegment(){
//   while(digitalRead(spotCounterTargetBtn) == HIGH && digitalRead(manual) == HIGH){
//     delay(100);
//     if(pressLength_spotCounterTargetBtn_milliSeconds == 0){
//       y++;
//       display2.showNumberDec(y);
//     }
//     pressLength_spotCounterTargetBtn_milliSeconds = pressLength_spotCounterTargetBtn_milliSeconds + 100;
//   }
//   if((pressLength_spotCounterTargetBtn_milliSeconds + 400) >= optionTwo_spotCounterTargetBtn_milliSeconds){
//     pressLength_spotCounterTargetBtn_milliSeconds = 0;
//   }
// }



