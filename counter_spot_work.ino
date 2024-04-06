#include <TM1637Display.h>
#include <EEPROM.h>

const int CLK = 3; //Set the CLK pin connection to the display
const int DIO = 2; //Set the DIO pin connection to the display
const int spotCounterUpBtn = 12; // spot up count
const int spotCounterDownBtn = 14; // spot down count
float pressLength_spotCounterBtn_milliSeconds = 0;
int optionTwo_spotCounterBtn_milliSeconds = 500;

const int spot_swapDigit = 47;
float pressLength_spotCounterTargetBtn_milliSeconds = 0;
int optionTwo_spotCounterTargetBtn_milliSeconds = 500;

const int resetBtn = 8;
float pressLength_milliSeconds = 0;
int optionTwo_milliSeconds = 2000; 

const int DIO2 = 7;
const int CLK2 = 5;
const int spotCounterTargetUpBtn = 13; // spot target up
const int spotCounterTargetDownBtn = 16; // spot target down
float pressLength_workCounterBtn_milliSeconds = 0;
int optionTwo_workCounterBtn_milliSeconds = 500;

const int DIO3 = 53;
const int CLK3 = 51;
const int workCounterUpBtn = 29;
const int workCounterDownBtn = 30;

const int DIO4 = 33;
const int CLK4 = 31;
const int workCounterTargetUpBtn = 41;
const int workCounterTargetDownBtn = 43;
const int manualSwitchInput = 39;
const int autoSwitchInput = 42;
float pressLength_workCounterTargetBtn_milliSeconds = 0;
int optionTwo_workCounterTargetBtn_milliSeconds = 500;

int numCounter = 0;
int spotCounterState = 0;
int spotCounterTargetState = 0;
int resetState = 0;

int x = 0; // spotCounter 
int y = 0; // spotCounterTarget
int z = 0; // workCounter
int a = 0; // workCounterTarget

TM1637Display display(CLK, DIO); //set up the 4-Digit Display.
TM1637Display display2(CLK2, DIO2); //set up the 4-Digit Display.
TM1637Display display3(CLK3, DIO3); //set up the 4-Digit Display.
TM1637Display display4(CLK4, DIO4); //set up the 4-Digit Display.

void setup()
{
  Serial.begin(9600);
  setPinMode();
  set7segment();
}

void loop()
{
    spotCounterTargetSegment();
    workCounterTargetSegment();
    spotCounterSegment(); //Function นับจำนวน Spot เมื่อมีการ กด Button Spot + เพิ่มขึ้น
    resetButton();
}

void setPinMode(){
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


void spotCounterSegment(){
  while(digitalRead(spotCounterUpBtn) == LOW)
  {
    delay(100);
    if(pressLength_spotCounterBtn_milliSeconds == 0 && y != 0){
      x++;
      display.showNumberDec(x);
      if(x == y) {
        x = 0;
        digitalWrite(10 , HIGH);
        delay(1000);
        digitalWrite(10 , LOW);
        z++;
        display3.showNumberDec(z);
        display.showNumberDec(x);
      }
    }
    pressLength_spotCounterBtn_milliSeconds = pressLength_spotCounterBtn_milliSeconds + 100;
  } 
  while(digitalRead(spotCounterDownBtn) == LOW)
  {
    delay(100);
    if(pressLength_spotCounterBtn_milliSeconds == 0 && x != 0){
      x--;
      display.showNumberDec(x);
    }
    pressLength_spotCounterBtn_milliSeconds = pressLength_spotCounterBtn_milliSeconds + 100;
  } 
  if((pressLength_spotCounterBtn_milliSeconds + 400) >= optionTwo_spotCounterBtn_milliSeconds){
    pressLength_spotCounterBtn_milliSeconds = 0;
  }
}

void workCounterSegment(){
  while(digitalRead(workCounterUpBtn) == LOW && digitalRead(manualSwitchInput) == LOW)
  {
    delay(100);
    if(pressLength_workCounterBtn_milliSeconds == 0){
      z++;
      display.showNumberDec(z);
      if(z == a) {
        z = 0;
        digitalWrite(10 , HIGH);
        delay(1000);
        digitalWrite(10 , LOW);
        a++;
        display3.showNumberDec(a);
        display.showNumberDec(z);
      }
    }
    pressLength_workCounterBtn_milliSeconds = pressLength_workCounterBtn_milliSeconds + 100;
  }
  while(digitalRead(workCounterUpBtn) == LOW && digitalRead(manualSwitchInput) == LOW)
  {
    delay(100);
    if(pressLength_workCounterBtn_milliSeconds == 0){
      z--;
      display.showNumberDec(z);
    }
    pressLength_workCounterBtn_milliSeconds = pressLength_workCounterBtn_milliSeconds + 100;
  }
  
  if((pressLength_workCounterBtn_milliSeconds + 400) >= optionTwo_workCounterBtn_milliSeconds){
    pressLength_workCounterBtn_milliSeconds = 0;
  }
}

void spotCounterTargetSegment(){
  while(digitalRead(manualSwitchInput) == LOW && digitalRead(spotCounterTargetUpBtn) == LOW && digitalRead(resetBtn) != LOW){
    delay(100);
    if(pressLength_spotCounterBtn_milliSeconds == 0){
        y++;
        display.showNumberDec(y);
      }
    pressLength_spotCounterBtn_milliSeconds = pressLength_spotCounterBtn_milliSeconds + 100;
  }
  while(digitalRead(manualSwitchInput) == LOW && digitalRead(spotCounterTargetDownBtn) == LOW && digitalRead(resetBtn) != LOW){
    delay(100);
    if(pressLength_spotCounterBtn_milliSeconds == 0){
        y--;
        display.showNumberDec(y);
      }
    pressLength_spotCounterBtn_milliSeconds = pressLength_spotCounterBtn_milliSeconds + 100;
  }
  if(pressLength_spotCounterTargetBtn_milliSeconds >= optionTwo_spotCounterTargetBtn_milliSeconds){
      pressLength_spotCounterTargetBtn_milliSeconds = 0;
    }
  }
    

void workCounterTargetSegment(){
  while(digitalRead(manualSwitchInput) == LOW && digitalRead(workCounterTargetUpBtn) == LOW && digitalRead(resetBtn) != LOW){
    delay(100);
    if(pressLength_workCounterTargetBtn_milliSeconds == 0){
        a++;
        display.showNumberDec(a);
      }
    pressLength_workCounterTargetBtn_milliSeconds = pressLength_workCounterTargetBtn_milliSeconds + 100;
  }
  while(digitalRead(manualSwitchInput) == LOW && digitalRead(workCounterTargetDownBtn) == LOW && digitalRead(resetBtn) != LOW){
    delay(100);
    if(pressLength_workCounterTargetBtn_milliSeconds == 0){
        a--;
        display.showNumberDec(a);
      }
    pressLength_workCounterTargetBtn_milliSeconds = pressLength_workCounterTargetBtn_milliSeconds + 100;
  }
  if(pressLength_workCounterTargetBtn_milliSeconds >= optionTwo_workCounterTargetBtn_milliSeconds){
      pressLength_workCounterTargetBtn_milliSeconds = 0;
  }
}

void resetButton(){
  while(digitalRead(resetBtn) == LOW && digitalRead(manualSwitchInput) == LOW)
  {
    delay(100);
    pressLength_milliSeconds = pressLength_milliSeconds + 100;
    if(pressLength_milliSeconds >= optionTwo_milliSeconds){
      x = 0;
      z = 0;
      display.showNumberDec(x);
      display3.showNumberDec(z);
      pressLength_milliSeconds = 0;
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



