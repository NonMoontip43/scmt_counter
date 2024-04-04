#include <TM1637Display.h>
#include <EEPROM.h>

const int CLK = 3; //Set the CLK pin connection to the display
const int DIO = 2; //Set the DIO pin connection to the display
const int spotCounterBtn = 12;
float pressLength_spotCounterBtn_milliSeconds = 0;
int optionTwo_spotCounterBtn_milliSeconds = 500;

const int spotCounterTargetBtn = 13;
const int manual = 49;
const int spot_swapDigit = 47;
float pressLength_spotCounterTargetBtn_milliSeconds = 0;
int optionTwo_spotCounterTargetBtn_milliSeconds = 500;

const int resetBtn = 8;
float pressLength_milliSeconds = 0;
int optionTwo_milliSeconds = 2000; 

const int DIO2 = 7;
const int CLK2 = 5; //Set the CLK pin connection to the display

const int DIO3 = 53;
const int CLK3 = 51;

const int DIO4 = 33;
const int CLK4 = 31;
const int manualSwitchInput = 39;
const int autoSwitchInput = 42;
const int workCounterTargetBtn = 41;
float pressLength_workCounterTargetBtn_milliSeconds = 0;
int optionTwo_workCounterTargetBtn_milliSeconds = 500;

int numCounter = 0;
int spotCounterState = 0;
int spotCounterTargetState = 0;
int resetState = 0;
int x = 0;
int y = 0;
int z = 0;
int a = 0;

int switchDigit = 0;
int w_switchDigit = 0;
int s_currentDigit = 4;
int w_currentDigit = 4;
int digit = 0;
bool state = false;
bool w_state = false;

int s_digit0 = 0;
int s_digit1 = 0;
int s_digit2 = 0;
int s_digit3 = 0;

int w_digit0 = 0;
int w_digit1 = 0;
int w_digit2 = 0;
int w_digit3 = 0;

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
  pinMode(spotCounterBtn, INPUT_PULLUP);
  pinMode(spotCounterTargetBtn, OUTPUT);
  pinMode(manual, OUTPUT);
  pinMode(spot_swapDigit, OUTPUT);
  pinMode(autoSwitchInput, OUTPUT);
  pinMode(manualSwitchInput, OUTPUT);
  pinMode(workCounterTargetBtn, OUTPUT);
  pinMode(resetBtn , OUTPUT);
  pinMode(10 , OUTPUT);
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
  while(digitalRead(spotCounterBtn) == LOW)
  {
    Serial.println("spontCounter");
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
  if((pressLength_spotCounterBtn_milliSeconds + 400) >= optionTwo_spotCounterBtn_milliSeconds){
    pressLength_spotCounterBtn_milliSeconds = 0;
  }
}

void spotCounterTargetSegment(){
  while(digitalRead(manualSwitchInput) == LOW && digitalRead(spotCounterBtn) != HIGH && digitalRead(resetBtn) != HIGH && digitalRead(manual) == HIGH){
    delay(100);
      while(digitalRead(spot_swapDigit) == HIGH){
        delay(100);
        switchDigit += 100;
      }
     if(switchDigit >= 2000){
          state = false;
          switchDigit = 0;
          s_currentDigit = 4;
          y = (String(s_digit0) + String(s_digit1) + String(s_digit2) + String(s_digit3)).toInt();
          display2.showNumberDec(y);
        }else if(switchDigit >= 100){
          state = true;
          switchDigit = 0;
          if(s_currentDigit == -1) s_currentDigit = 4;
          else {
            s_currentDigit--;
            y = (String(s_digit0) + String(s_digit1) + String(s_digit2) + String(s_digit3)).toInt();
            display2.showNumberDec(y);
            digit = 0;
          }
      }
    if(s_currentDigit != 4) {
      if(s_currentDigit == 3){
        display2.showNumberDec(s_digit3 , false , 1 , s_currentDigit);
        delay(500);
        display2.setSegments(0xff , 1 , s_currentDigit);
        delay(500);
        display2.showNumberDec(s_digit3 , false , 1 , s_currentDigit);
      }else if(s_currentDigit == 2){
        display2.showNumberDec(s_digit2 , false , 1 , s_currentDigit);
        delay(500);
        display2.setSegments(0xff , 1 , s_currentDigit);
        delay(500);
        display2.showNumberDec(s_digit2 , false , 1 , s_currentDigit);
      }else if(s_currentDigit == 1){
        display2.showNumberDec(s_digit1 , false , 1 , s_currentDigit);
        delay(500);
        display2.setSegments(0xff , 1 , s_currentDigit);
        delay(500);
        display2.showNumberDec(s_digit1 , false , 1 , s_currentDigit);
      }else if(s_currentDigit == 0){
        display2.showNumberDec(s_digit0 , false , 1 , s_currentDigit);
        delay(500);
        display2.setSegments(0xff , 1 , s_currentDigit);
        delay(500);
        display2.showNumberDec(s_digit0 , false , 1 , s_currentDigit);
      }
    }
    if(state && digitalRead(spotCounterTargetBtn) == HIGH){
      if(pressLength_spotCounterTargetBtn_milliSeconds >= 0 && s_currentDigit == 3){
        s_digit3+=1;
        display2.showNumberDec(s_digit3 , false , 1 , 3);
      }else if(pressLength_spotCounterTargetBtn_milliSeconds >= 0 && s_currentDigit == 2){
        s_digit2+=1;
        display2.showNumberDec(s_digit2 , false , 1 , 2);
      }
      else if(pressLength_spotCounterTargetBtn_milliSeconds >= 0 && s_currentDigit == 1){
        s_digit1+=1;
        display2.showNumberDec(s_digit1 , false , 1 , 1);
      }
      else if(pressLength_spotCounterTargetBtn_milliSeconds >= 0 && s_currentDigit == 0){
        s_digit0+=1;
        display2.showNumberDec(s_digit0 , false , 1 , 0);
      }
      pressLength_spotCounterTargetBtn_milliSeconds = pressLength_spotCounterTargetBtn_milliSeconds + 100;
    }
    if(pressLength_spotCounterTargetBtn_milliSeconds >= optionTwo_spotCounterTargetBtn_milliSeconds){
      pressLength_spotCounterTargetBtn_milliSeconds = 0;
    }
  }
  
}

void workCounterTargetSegment(){
  while(digitalRead(spot_swapDigit) != HIGH && digitalRead(spotCounterBtn) != HIGH && digitalRead(resetBtn) != HIGH && digitalRead(manual) == HIGH){
    delay(100);
      while(digitalRead(manualSwitchInput) == LOW){
        delay(100);
        w_switchDigit += 100;
      }
     if(w_switchDigit >= 2000){
          w_state = false;
          w_switchDigit = 0;
          w_currentDigit = 4;
          a = (String(w_digit0) + String(w_digit1) + String(w_digit2) + String(w_digit3)).toInt();
          display4.showNumberDec(a);
        }else if(w_switchDigit >= 100){
          w_state = true;
          w_switchDigit = 0;
          if(w_currentDigit == -1) w_currentDigit = 4;
          else {
            w_currentDigit--;
            a = (String(w_digit0) + String(w_digit1) + String(w_digit2) + String(w_digit3)).toInt();
            display4.showNumberDec(a);
            digit = 0;
          }
      }
    if(w_currentDigit != 4) {
      if(w_currentDigit == 3){
        display4.showNumberDec(w_digit3 , false , 1 , w_currentDigit);
        delay(500);
        display4.setSegments(0xff , 1 , w_currentDigit);
        delay(500);
        display4.showNumberDec(w_digit3 , false , 1 , w_currentDigit);
      }else if(w_currentDigit == 2){
        display4.showNumberDec(w_digit2 , false , 1 , w_currentDigit);
        delay(500);
        display4.setSegments(0xff , 1 , w_currentDigit);
        delay(500);
        display4.showNumberDec(w_digit2 , false , 1 , w_currentDigit);
      }else if(w_currentDigit == 1){
        display4.showNumberDec(w_digit1 , false , 1 , w_currentDigit);
        delay(500);
        display4.setSegments(0xff , 1 , w_currentDigit);
        delay(500);
        display4.showNumberDec(w_digit1 , false , 1 , w_currentDigit);
      }else if(w_currentDigit == 0){
        display4.showNumberDec(w_digit0 , false , 1 , w_currentDigit);
        delay(500);
        display4.setSegments(0xff , 1 , w_currentDigit);
        delay(500);
        display4.showNumberDec(w_digit0 , false , 1 , w_currentDigit);
      }
    }
    if(w_state && digitalRead(workCounterTargetBtn) == HIGH){
      if(pressLength_workCounterTargetBtn_milliSeconds >= 0 && w_currentDigit == 3){
        w_digit3+=1;
        display4.showNumberDec(w_digit3 , false , 1 , 3);
      }else if(pressLength_workCounterTargetBtn_milliSeconds >= 0 && w_currentDigit == 2){
        w_digit2+=1;
        display4.showNumberDec(w_digit2 , false , 1 , 2);
      }
      else if(pressLength_workCounterTargetBtn_milliSeconds >= 0 && w_currentDigit == 1){
        w_digit1+=1;
        display4.showNumberDec(w_digit1 , false , 1 , 1);
      }
      else if(pressLength_workCounterTargetBtn_milliSeconds >= 0 && w_currentDigit == 0){
        w_digit0+=1;
        display4.showNumberDec(w_digit0 , false , 1 , 0);
      }
      pressLength_workCounterTargetBtn_milliSeconds = pressLength_workCounterTargetBtn_milliSeconds + 100;
    }
    if(pressLength_workCounterTargetBtn_milliSeconds >= optionTwo_workCounterTargetBtn_milliSeconds){
      pressLength_workCounterTargetBtn_milliSeconds = 0;
    }
  }
}

void resetButton(){
  while(digitalRead(resetBtn) == HIGH && digitalRead(manual) == HIGH)
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



