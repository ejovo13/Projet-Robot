extern "C" {
    #include "piece.h"
}

#include <servo.h>
#include <Ping.h>

#define MAX_Distance 200
#define MIN_Distance 20
#define carSpeed 100
int angle = 0;

// Unltrasonic sensor initialization
int Echo = A4; //Ping that send the wave
int Trig = A5; //Ping that receive the wave
NewPing sonar(Trig,Echo, MAX_Distance); // NewPing setup of pins and maximum distance

//Motor initialization
#define LM 5 //Activation of Left side motors
#define LFW 7 //Left front wheel
#define LBW 8 //Left back wheel

#define RM 6 //Activaton of Right side motors
#define RBW 9 //Right front wheel
#define RFW 11 //Right back wheel

//Ultrasonic initialization
Servo myservo;

//Functions initialization
void forward();
void backward();
void left();
void right();
void stop();


void setup() {
    Serial.begin(9600); //set up the terminal

    myservo.attach(3); //pin linked to the sensor motor
    myservo.write(90); //set sensor motor to 90 degres

    pinMode(LFW, OUTPUT); //set Left wheels pin mode to output
    pinMode(LBW, OUTPUT);
    pinMode(LM,OUTPUT);
    digitalWrite(LM, LOW); //disable left motors

    pinMode(RFW, OUTPUT); //set Right wheels pin mode to output
    pinMode(RBW, OUTPUT);
    pinMode(RM, OUTPUT);
    digitalWrite(RM, LOW); //disable right motors
}

void loop() {
    //do a 360
    if (angle < 360)
    {
        Serial.println(getDistance());
        left();
        i++;
    }
}

//Go forward
void forward() {
    digitalWrite(LM, HIGH); //Turn on left side motor
    digitalWrite(LFW, HIGH); //Forward mode on left side wheels
    digitalWrite(LBW, LOW);

    digitalWrite(RM, HIGH); //Turn on right side motor
    digitalWrite(RFW, HIGH); //Forward mode on right side wheels
    digitalWrite(RBW, LOW);
    Serial.println("Forward");
}

//Go backward
void backward() {
    digitalWrite(LM, HIGH); //Turn on left side motor
    digitalWrite(LFW, LOW); //Backward mode on left side wheels
    digitalWrite(LBW, HIGH);

    digitalWrite(RM, HIGH); //Turn on right side motor
    digitalWrite(RFW, LOW); //Backward mode on right side wheels
    digitalWrite(RBW, HIGH);
    Serial.println("Backward");
}

//Go left
void left() {
    digitalWrite(LM, HIGH); //Turn on left side motor
    digitalWrite(LFW, LOW); //Backward mode on left side wheels
    digitalWrite(LBW, HIGH);

    digitalWrite(RM, HIGH); //Turn on right side motor
    digitalWrite(RFW, HIGH); //Forward mode on right side wheels
    digitalWrite(RBW, LOW);
    Serial.println("Left");
}

//Go right
void right() {
    digitalWrite(LM, HIGH); //Turn on left side motor
    digitalWrite(LFW, HIGH); //Forward mode on left side wheels
    digitalWrite(LBW, LOW);

    digitalWrite(RM, HIGH); //Turn on right side motor
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

//Get distance
int getDistance(){
    int d = sonar.ping_cm();
    if(d==0)
    { return MAX_Distance; } //When out of range always equal to 0 bcs when smth is close, never returns 0
    else
    { return d; }
}
