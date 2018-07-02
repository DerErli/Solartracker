#include <Arduino.h>
#include <Stepper.h>
#include <stdlib.h>


//pins (Pinbelegung Endpunkte)
#define endPinVert 2
#define endPinHor 3

//define const
#define normSpeed 30
#define stepsPerRevolution 200
#define tolerance 15

//define sensorpairs
#define UP 1
#define DOWN 2
#define LEFT 4
#define RIGHT 3

//functions
void moveHor(int steps);
void moveVert(int steps);
void reference();
void initialize();
void updateSens();
void outSens();
int avSens(int pos);

//sensors
int lo;
int ro;
int lu;
int ru;

//var
float hor;
float vert;

//Stepper (Pinbelegung Motoren)
Stepper horStepper(stepsPerRevolution, 7, 6, 5, 4);
Stepper vertStepper(stepsPerRevolution, 11, 10, 9, 8);

void setup() {
  Serial.begin(9600);

  
  Serial.println("HALLO?");
  pinMode(endPinHor, INPUT_PULLUP);
  pinMode(endPinVert, INPUT_PULLUP);
  reference();
  initialize();
}

void loop() {
  outSens();

  if(avSens(LEFT)>avSens(RIGHT)){
    moveHor(1);
  }else if(avSens(LEFT)<avSens(RIGHT)){
    moveHor(-1);
  }

  if(avSens(UP)>avSens(DOWN)){
    moveHor(1);
  }else if(avSens(UP)<avSens(DOWN)){
    moveHor(-1);
  }
}

void reference() {
  Serial.println("Here");
  horStepper.setSpeed(normSpeed);
  while(digitalRead(endPinHor) != LOW){
    Serial.println("Hrtr");
         moveHor(1);
  }
  hor = 0;

  vertStepper.setSpeed(normSpeed);
  while(digitalRead(endPinVert) != LOW){
     moveVert(1);
  }
  vert = 0;
}

void initialize(){
  moveVert(25);
  int bright [40];
  for(int i = 0; i<40; i++){
    bright[i]=(avSens(LEFT)+avSens(RIGHT))/2;
    moveHor(5);
  }
  int highest = 0;
  int index;
  for(int x = 0; x<40; x++){
    if(bright[x]>highest){
      highest = bright[x];
      index = x;
    }
  }
  moveHor(hor-index*5);
}

void outSens(){
  updateSens();
  Serial.print(lo);
  Serial.print(";");
  Serial.print(ro);
  Serial.print(";");
  Serial.print(lu);
  Serial.print(";");
  Serial.print(ru);
}

int avSens(int pos){
  updateSens();
  switch(pos){
  case 1:
    return (ro+lo)/2;
  case 2:
    return (ru+lu)/2;
  case 3:
    return (lo+lu)/2;
  case 4:
    return (ro+ru)/2;
  }
}

void updateSens(){
  lo = analogRead(A0);
  ro = analogRead(A1);
  lu = analogRead(A2);
  ru = analogRead(A3);
}

void moveVert(int steps) {
  if(vert<198 && vert>0){
      vertStepper.step(steps);
  }
  vert += steps;
}

void moveHor(int steps) {
  if(hor<200-steps){
    horStepper.step(steps);
  }else if(hor<0){
    hor=0;
    horStepper.step(200-steps);
  }else{
    hor=0;
    horStepper.step(-200+steps);
  }
  hor += steps;
}
