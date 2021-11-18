//
// extern "C" {
    //  #include "ejovo_matrix.h"
    //   #include "piece.h"
// }

#include <Servo.h>
#include <NewPing.h>
//
#define MAX_DISTANCE 350
#define MIN_Distance 20
//
//// Unltrasonic sensor initialization
int Echo = A4; //Ping that send the wave
int Trig = A5; //Ping that receive the wave
NewPing sonar(Trig,Echo, MAX_DISTANCE); // NewPing setup of pins and maximum distance
//
//Motor initialization
#define LM 5 // Activation of Left side motors
#define LFW 7 // Left forwards
#define LBW 8 // Left backwards

#define RM 6 // Activaton of Right side motors
#define RBW 9 // Right backwards
#define RFW 11 // ight forwards
int carSpeed = 250;

// const int NORTH = 1;
// const int EAST = 2;
// const int SOUTH = 3;
// const int WEST = 4;

//Functions initialization
void forward();
void backward();
void left();
void right();
void stop();
void turn_right(float);
void rightSpeed(int);
void go_square_left();
void go_square_right();


void setup() {
    Serial.begin(9600); //set up the terminal

    Serial.println("What the fuck man");
    Servo myservo;

    Serial.println("Hello my code is running");
//
    myservo.attach(3); //pin linked to the sensor motor
    myservo.write(90); //set sensor motor to 90 degres

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

    // for (int i = 0; i < 8; i++) {
    //     car_advance(1);
    //     delay(1000);
    // }

    // car_advance(8);
    // delay(1000);

    // go_square_right();
    // delay(1000);
    // go_square_left();
    scan360();


// turn 5 degrees at a time, 18 times
//    for (int i = 0; i < 18; i ++) {
//      turn_right(5);
//      delay(500);
//    }


    // for (int i = 0; i < 18; i++) {
    //   turn_left(5);
    //   delay(500);
    // } // turn 90 degrees

    // delay(2500);

    // for (int i = 0; i < 18; i++) {
    //   turn_left(10);
    //   delay(500);
    // }
}

void loop() {
      Serial.println("Looping");
}
//Go forward
void forward() {
    analogWrite(LM, carSpeed); // Turn on left side motor
    analogWrite(RM, 0.948 * carSpeed); // compensate for physical
    digitalWrite(LFW, HIGH); // Left side move forward
    digitalWrite(LBW, LOW);  // left side move backwards
    digitalWrite(RFW, HIGH);  // right side move forward
    digitalWrite(RBW, LOW);   // right side move backwards
    Serial.println("Forward");
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

//Setup left rotation speed
void leftSpeed(int speed) {
  carSpeed = speed;
  left();
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

//Setup right rotation speed 
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

//*********** CAR COORDONEES ****************\\

enum direction { NORTH, EAST, SOUTH, WEST };
float g_car_i = 19;
float g_car_j = 19;
int g_car_direction = NORTH;
int g_car_speed = 0;
float g_car_angle = 0;

//*********** SPEED ANGLE TEST  **************\\
//
// **** 20 rotations for speed 250 ******
//          right(s)    left(s)
//  250     27.55       25.37
//  250     27.62       26.81
//  250     27.61       26.33
//
// MOYENNE  1.37967     1.3085    ---TIME for 1 rotation
//
// 360 / 1.37967 = 260.93259      --- RIGHT angle speed
// 360 / 1.3085  = 275.124188     --- LEFT angle speed
// fonction qui prend degrees en argument et qui rotates that amount

const float timeMultiplierRight = 1.035; // extend the rotation time by this amount
const float angVelRight250 = 260.93259;
const float angVelLeft250 = 275.124188;

// turn right at 250 carSpeed!!
void turn_right(float __degrees) {

  g_car_angle -= __degrees;
  float speedScalar = 1.12;

  // pour combien de temps?
  float turnTime = __degrees / angVelRight250; // give us the time it takes to rotate __degrees in seconds
  float waitTime = 0;

  float surgeTime = 5.23f / angVelRight250; // overcome friction

  if (__degrees <= 90) {
     waitTime = turnTime * speedScalar;
  } else {
    waitTime = 0;
  }

  // surge
  rightSpeed(250); //Do the first 5.23 degrees with surgetime to overcome friction
  delay(surgeTime * 1000);

  rightSpeed(250); 
  delay( waitTime * 1000);
  stop()

}

// turn right at 250 carSpeed!!
void turn_left(float __degrees) {

  g_car_direction += __degrees;

  // pour combien de temps?
  float speedScalar = 1.12;

  float turnTime = __degrees / angVelLeft250; // give us the time it takes to rotate __degrees in seconds
  float waitTime = 0;

  float surgeTime = 6.6f / angVelLeft250; // overcome friction

  if (__degrees <= 90) {
     waitTime = turnTime * speedScalar; 
  } else {
    waitTime = 0;
  }

  // surge
  leftSpeed(250); //Do the first 6.6 degrees with surge time to over come friction
  delay(surgeTime * 1000);

  leftSpeed(250);
  delay( waitTime * 1000);
  stop();

}

// Car turn left by 90 degrees and update direction
void car_turn_left(void) {

    switch ( g_car_direction ) {

        case NORTH:

            g_car_direction = WEST;
            break;

        case EAST:

            g_car_direction = NORTH;
            break;

        case SOUTH:

            g_car_direction = EAST;
            break;

        case WEST:
            g_car_direction = SOUTH;
            break;

    }

    turn_left(90); // this function also adjusts the variable g_car_angle
}

// Car turn right by 90 degrees and update direction
void car_turn_right(void) {

    switch ( g_car_direction ) {

        case NORTH:

            g_car_direction = EAST;
            break;

        case EAST:

            g_car_direction = SOUTH;
            break;

        case SOUTH:

            g_car_direction = WEST;
            break;

        case WEST:
            g_car_direction = NORTH;
            break;

    }

    turn_right(90);
}

void go_square_left() {

    car_advance(5);
    delay(200);
    car_turn_left();
    delay(200);
    car_advance(5);
    delay(200);
    car_turn_left();
    delay(200);
    car_advance(5);
    delay(200);
    car_turn_left();
    delay(200);
    car_advance(5);
    delay(200);
    car_turn_left();
}

void go_square_right() {

    car_advance(5);
    delay(200);
    car_turn_right();
    delay(200);
    car_advance(5);
    delay(200);
    car_turn_right();
    delay(200);
    car_advance(5);
    delay(200);
    car_turn_right();
    delay(200);
    car_advance(5);
    delay(200);
    car_turn_right();
    delay(200);
}

//************ FORWARD SPEED FOR ONE CASE ********\\
// piece 80 x 80 pour une piece de 400cm * 400cm
// ca fait une matrix 80 x 10
// ==> 800 bytes (we have ~2000)
// une case = 5 x 5 cm
//
// distance    time
// -------- ----------
//   40 cm    0.62s
//            0.68s
//            0.69s
//            0.62s
//            0.62s
//
// moyenne := 61.919504 cm / s
// pour faire une case, on veux 5 / moyenne = 0.08075 s
// ===> delay(81)

const float carSpeedForward250 = 61.919504;
const int cmPerCase = 5;

// Advance __ncases forward
void car_advance(int __ncases) {

    float speedScalar = 1.12;
    float surgeTime = 0.285 * (cmPerCase / carSpeedForward250);
    float waitTime = speedScalar * __ncases * (cmPerCase / carSpeedForward250);

    switch ( g_car_direction ) {

        case NORTH:
            g_car_i -= __ncases;
            break;

        case EAST:
            g_car_j += __ncases;
            break;

        case SOUTH:

            g_car_i += __ncases;
            break;

        case WEST:

            g_car_j -= __ncases;
            break;
    }

    forward(); //Do 0.285 case for surgetime to overcome friction
    delay(surgeTime * 1000);
    forward();
    delay(waitTime * 1000);
    stop();

}

// Get distance
int getDistance(){

   int d = sonar.ping_cm();
   if(d == 0 || d > MAX_DISTANCE)
   { return -1; } //When out of range always equal to 0 bcs when smth is close, never returns 0
   else
   { return d; }
}


void scan360() {

    for (int i = 0; i < 180; i++) {
        turn_right(2);
        int distance_cm = getDistance();

        if (distance_cm == -1) { // out of range
            //do nothing
            continue;
        } else {

            // interpret distance
            // do trigonometry with the car angle to find the appropriate piece coordinate
            float angle_radian = ( g_car_angle / 180 ) * PI;

            // calculate i and j from angle_radian and distance_cm
            //

            float offsetLeftCol = sin(angle_radian) * distance_cm; // if x is positive, move `offsetLeftCol` centimeters to the left
            float offsetUpRow = cos(angle_radian) * distance_cm; // if y is positive, ascend `offsetUpRow` centimeters up

            int colOffset = (int) offsetLeftCol / 5;
            int rowOffset = (int) offsetUpRow / 5;

            int i = g_car_i - rowOffset;
            int j = g_car_j - colOffset;

            // Piece_set(g_piece, i, j, true);


        }

        delay(75);
    }

}