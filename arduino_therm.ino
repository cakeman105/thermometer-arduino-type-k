/*digital adjustable thermometer with a load control function
  made by cakeman105
  copy or mod this if you want
*/

#include <EEPROM.h>
#include "max6675.h"
#include <millisDelay.h>

#define addr 1

#define a 2 //just used the #define function to make editing the code easier, change these values based on your pinout
#define b 3 //refer to your display datasheet if you don't know
#define c 4
#define d 5
#define e 6
#define f 7 
#define g 8
#define ground1 9
#define ground2 10
#define ground3 A4

#define redPin A5
#define greenPin A3

#define DO 11
#define CLK 12
#define CS 13


//every single number and its corresponding segment pins
int digit0[] = {a, b, c, d, e, f};
int digit1[] = {b, c};
int digit2[] = {a, b, g, e, d};
int digit3[] = {a, b, g, c, d};
int digit4[] = {f, g, b, c};
int digit5[] = {a, f, g, c, d};
int digit6[] = {f, g, c, d, e, a};
int digit7[] = {a, b, c};
int digit8[] = {a, b, c, d, e, f, g};
int digit9[] = {a, b, g, f, c}; 
int gndPins[] = {ground1, ground2, ground3}; //pins the digit transistors are connected to
int number, temperature;
bool tempSet;

MAX6675 thermocouple(CLK, CS, DO);
millisDelay tempReadDelay;

void setup() 
{
  for (int i = 2; i < 9; i++) //declare pins as output
  {
   pinMode(i, OUTPUT);
  }

  pinMode(A0, INPUT); //button pins declared
  pinMode(A1, INPUT);

  pinMode(redPin, OUTPUT); //declared LED pins
  pinMode(greenPin, OUTPUT);


  if (EEPROM.read(addr) ==  0) //makes sure that the number starts at 100 if the EEPROM is overwritten, else it keeps the number entered by the user
  {
    number = 100;
  }
  else
  {
    number = EEPROM.read(addr);
  }
  tempReadDelay.start(1000);
  delay(500);
}

void loop() 
{
  if (digitalRead(A0) == HIGH && digitalRead(A1) == HIGH) //this part checks for + and - button presses at the same time
  {
    tempSet = true;
    delay(2500);
  }
  else
  {
    tempSet = false;
  }


 editTemperature(tempSet); //this allows the user to edit the temperature by pressing the + and - button at the same time
 tempSet = false;
  clearDisplay();

  if (tempReadDelay.justFinished())
  {
    if (thermocouple.readCelsius() < 1000 && thermocouple.readCelsius() >= 0) //checks if the value is in a range from 0 to 1000
    {
      temperature = int(thermocouple.readCelsius()) - 2; //if yes, it displays said number, the minus 2 is to mitigate the thermocouple offset
      displayNumber(temperature);
    }
    else //if no, it displays hyphens, i can't be arsed to make it display negative numbers
    {
      for (int i = 0; i < 3; i++)
      {
        digitalWrite(gndPins[i], HIGH);
        digitalWrite(g, HIGH);
        delay(1);
        digitalWrite(gndPins[i], LOW);
        clearDisplay();
      }
    }
    tempReadDelay.restart();
  }

  if(temperature >= number)
  {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
  }
  else
  {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
  }

  displayNumber(temperature); 
  clearDisplay();
}

void displayNumber(int number)
{
  int numbers[3];

  numbers[0] = number % 10; //this part splits the numbers into 3 digits for displaying
  number /= 10;
  numbers[1] = number % 10;
  number /= 10;
  numbers[2] = number % 10;
  number /= 10;

  for (int j = 0; j < 3; j++)
  {
    digitalWrite(gndPins[j], HIGH);
    switch (numbers[j]) // i think i could've written better code, but fuck it, switch
    {
      case 0:
      for (int i = 0; i < 6; i++)
      {
        digitalWrite(digit0[i], HIGH);
      }
      break;
      case 1:
      for (int i = 0; i < 2; i++)
      {
        digitalWrite(digit1[i], HIGH);
      }
      break;
      case 2:
    for (int i = 0; i < 5; i++)
    {
      digitalWrite(digit2[i], HIGH);
    }
      break;
     case 3:
    for (int i = 0; i < 5; i++)
    {
      digitalWrite(digit3[i], HIGH);
    }
    break;
     case 4:
      for (int i = 0; i < 4; i++)
      {
      digitalWrite(digit4[i], HIGH);
      }
    break;
     case 5:
    for (int i = 0; i < 5; i++)
    {
      digitalWrite(digit5[i], HIGH);
    }
    break;
     case 6:
    for (int i = 0; i < 6; i++)
    {
      digitalWrite(digit6[i], HIGH);
    }
    break;
     case 7:
    for (int i = 0; i < 3; i++)
    {
      digitalWrite(digit7[i], HIGH);
    }
    break;
     case 8:
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(digit8[i], HIGH);
    }
    break;
     case 9:
    for (int i = 0; i < 5; i++)
    {
      digitalWrite(digit9[i], HIGH);
    }
    break;
    
  }
  delay(1);
  digitalWrite(gndPins[j], LOW);
  clearDisplay();
  }
}

void clearDisplay() //turn all segments off
{
  for (int i = 2; i < 9; i++)
  {
    digitalWrite(i, LOW);
  }
}


void editTemperature(bool check) //this is where the user can adjust the temperature the led turns on at aka programming mode
{
  while (check == true) //checks if the butttons have been pressed
  {
    displayNumber(number);
    if (digitalRead(A1) == HIGH && number < 995)
    {
      number += 5;
      delay(150);
    }
    if (digitalRead(A0) == HIGH && number > 0)
    {
      number -= 5;
      delay(150);
    }
    displayNumber(number);
    clearDisplay();
    if(digitalRead(A0) == HIGH && digitalRead(A1) == HIGH) //exits from programming mode
    {
      EEPROM.update(addr, number); //nvm, changing the #define addr to 1 from 0 fixes it 
      delay(2500);
      break;
    }
  }
}
