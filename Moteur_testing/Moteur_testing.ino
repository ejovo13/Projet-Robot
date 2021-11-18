//
extern "C" {
      #include "ejovo_matrix.h"
       #include "piece.h"
}


//*********** TIMING TESTS **************\\

// 10 rotations
// carSpeed   right   left
//  150       39.75s  38.11s
//  150       39.55s  38.18s
//  150       40.58s  38.20s
//
// MOYENNE    3.996s  3.81633
//
//  200
//  200
//  200
//
// **** 20 rotations for 250
//  250        27.55
//  250        27.62
//  250        27.61
//
// MOYENNE     1.37967

// 360 / 1.37967 = 260.93259


// 360 degrees
// 360/3.996s


#include <Servo.h>
#include <NewPing.h>
//
#define MAX_Distance 200
#define MIN_Distance 20
int angle = 0;
//
//// Unltrasonic sensor initialization
int Echo = A4; //Ping that send the wave
int Trig = A5; //Ping that receive the wave
NewPing sonar(Trig,Echo, MAX_Distance); // NewPing setup of pins and maximum distance
//
//Motor initialization
#define LM 5 // Activation of Left side motors
#define LFW 7 // Left forwards
#define LBW 8 // Left backwards

#define RM 6 // Activaton of Right side motors
#define RBW 9 // Right backwards
#define RFW 11 // ight forwards
int carSpeed = 250;

//Ultrasonic initialization

//Functions initialization
void forward();
void backward();
void left();
void right();
void stop();
void turn_right(float);
void rightSpeed(int);
void Serial_print_piece(const Piece *p); // print piece to the serial

//Go forward
void forward() {
    analogWrite(LM, carSpeed); // Turn on left side motor
    analogWrite(RM, carSpeed);
    digitalWrite(LFW, HIGH); // Left side move forward
    digitalWrite(LBW, LOW);  // left side move backwards
    digitalWrite(RFW, HIGH);  // right side move forward
    digitalWrite(RBW, LOW);   // right side move backwards

//    digitalWrite(RM, HIGH); //Turn on right side motor
//    digitalWrite(RFW, HIGH); //Forward mode on right side wheels
//    digitalWrite(RBW, LOW);
    Serial.println("Forward");
}

Piece *p = NULL;

void setup() {

    p = Piece_new(80);

    Serial.begin(9600); //set up the terminal

    Serial.println("What the fuck man");
    Servo myservo;

    Serial.println("Hello my code is running");
//
    myservo.attach(3); //pin linked to the sensor motor
    myservo.write(50); //set sensor motor to 90 degres

    pinMode(10, INPUT);
    pinMode(4, INPUT);
    pinMode(2, INPUT);
//
    pinMode(RFW, OUTPUT); //set Left wheels pin mode to output
    pinMode(RBW, OUTPUT);
    pinMode(LFW,  OUTPUT);
    pinMode(LBW, OUTPUT);

    pinMode(LM, OUTPUT);
    pinMode(RM, OUTPUT);

    digitalWrite(RM, HIGH);
    digitalWrite(LM, HIGH);

    delay(1000); // chill out before turning

    // turn 5 degrees at a time, 18 times
//    for (int i = 0; i < 18; i ++) {
//      turn_right(5);
//      delay(500);
//    }

    for (int i = 0; i < 10; i++) {
      turn_right(9);
      delay(500);
    }

    turn_right(90);
    delay(1000);
    turn_right(90);
    delay(1000);
    turn_right(90);
    delay(1000);
    turn_right(90);
    delay(1000);
    delay(1000);
//
//    turn_right(180);
//    delay(1000);
//    turn_right(180);
//    delay(1000);
//    turn_right(180);
//    delay(1000);
//    turn_right(180);
//    delay(2000);
//
//    turn_right(360);
//    delay(1000);
//    turn_right(360);
//    delay(1000);
//    turn_right(360);

//    digitalWrite(LM, LOW); //disable left motors
////
//    pinMode(RFW, OUTPUT); //set Right wheels pin mode to output
//    pinMode(RBW, OUTPUT);
//    pinMode(RM, OUTPUT);
//    digitalWrite(RM, LOW); //disable right motors
}

void loop() {
    //do a 360
//    if (angle < 360)
//    {
//        Serial.println(getDistance());
//        left();
//        angle++;
//    }
//      delay(1000);
//      forward();
//      right();
//      left();
//      backward();
//      stop_n_go(1000);


     delay(3000);
      Serial.println("Looping");
      Serial_print_piece(p);
}



void stop_n_go(int duration) {
  forward();
  delay(duration);
  stop();
  delay(duration);
  backward();
  delay(duration);
  stop();
  delay(duration);
}

//Go backward
void backward() {
    analogWrite(LM, carSpeed); //Turn on left side motor
    digitalWrite(LFW, LOW); //Backward mode on left side wheels
    digitalWrite(LBW, HIGH);

    analogWrite(RM, carSpeed); //Turn on right side motor
    digitalWrite(RFW, LOW); //Backward mode on right side wheels
    digitalWrite(RBW, HIGH);
    Serial.println("Backward");
}

//Go left
void left() {

    analogWrite(LM, carSpeed); //Turn on left side motor
    digitalWrite(LFW, LOW); //Backward mode on left side wheels
    digitalWrite(LBW, HIGH);

    analogWrite(RM, carSpeed); //Turn on right side motor
    digitalWrite(RFW, HIGH); //Forward mode on right side wheels
    digitalWrite(RBW, LOW);
    Serial.println("Left");
}

void rightSpeed(int speed) {
  carSpeed = speed;
  right();
}

//Go right
void right() {
    analogWrite(LM, carSpeed); //Turn on left side motor
    digitalWrite(LFW, HIGH); //Forward mode on left side wheels
    digitalWrite(LBW, LOW);

    analogWrite(RM, carSpeed); //Turn on right side motor
    digitalWrite(RFW, LOW); //Backwrd mode on right side wheels
    digitalWrite(RBW, HIGH);
    Serial.println("Right");
}

//Stop
void stop() {
    digitalWrite(LM, LOW); //Turn off left side motors
    digitalWrite(RM, LOW); //Turn off right side motors
    Serial.println("Stop");
}

// fonction qui prend degrees en argument et qui rotates that amount
const float angVelRight = 90.09009;
const float angVelLeft = 94.33138;
const float timeMultiplierRight = 1.035; // extend the rotation time by this amount

const float angVelRight250 = 260.93259;


// turn right at 250 carSpeed!!
void turn_right(float __degrees) {

  // pour combien de temps?
  float turnTime = __degrees / angVelRight250; // give us the time it takes to rotate __degrees in seconds
  float waitTime = 0;

  float surgeTime = 6.6f / angVelRight250; // overcome friction

  if (__degrees <= 90) {
     waitTime = turnTime * 1.05;
  } else {
    waitTime = 0;
  }

  // surge
  rightSpeed(250);
  delay(surgeTime * 1000);

  rightSpeed(250);
  delay( waitTime * 1000);
  stop();

}

//Get distance
//int getDistance(){
////    int d = sonar.ping_cm();
//    if(d==0)
//    { return MAX_Distance; } //When out of range always equal to 0 bcs when smth is close, never returns 0
//    else
//    { return d; }
//}

void Serial_print_piece(const Piece *__p) {

    for (size_t i = 0; i < __p->nrows; i++) {

        for (size_t j = 0; j < __p->ncols; j++) {

            // Serial.print(Piece_get(__p, i, j));
            Serial.print(Matrix_at(__p, i, j));
            Serial.print(" ");
        }

        Serial.println("");
    }
}
