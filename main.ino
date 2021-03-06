extern "C" {
    #include "ejovo_matrix.h"
    #include "piece.h"
}

#include <Servo.h>
#include <NewPing.h>

#define MAX_DISTANCE 350
#define MIN_Distance 20

// Unltrasonic sensor initialization
int Echo = A4; //Ping that send the wave
int Trig = A5; //Ping that receive the wave
NewPing sonar(Trig,Echo, MAX_DISTANCE); // NewPing setup of pins and maximum distance

//Motor initialization
#define L_MOTOR 5 // Activation of Left side motors
#define L_FORWARD 7 // Left forwards
#define L_BACKWARD 8 // Left backwards

#define R_MOTOR 6 // Activaton of Right side motors
#define R_BACKWARD 9 // Right backwards
#define R_FORWARD 11 // Right forwards
int carSpeed = 250;

// these declarations should probably be in a header file

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
void Serial_print_piece(const Piece *p); // print piece to the serial
void setpins();
int getDistance();
void startFollowWalls();
void followRightWall();

// declare global variables?
Piece *g_piece = NULL;
Servo g_myservo;

// Global car variables
enum direction { NORTH, EAST, SOUTH, WEST };
float g_car_i = 39;
float g_car_j = 39;
int g_car_direction = NORTH;
int g_car_speed = 0;
float g_car_angle = 0;


void setup() {

    Serial.begin(9600); //set up the terminal

    g_myservo.attach(3); //pin linked to the sensor motor
    g_myservo.write(90); //set sensor motor to point straight forward

    g_piece = Piece_new(80);
    // for (size_t i = 0; i < 8; i++) { Piece_set(g_piece, i, i, 1); } // Set the diagonals to true

    // set all motor pins to output mode
    setpins();

    delay(1000); // chill out before turning

    // scan360(4);

    startFollowWalls();
    followRightWall();
    // followRightWall();
    // followRightWall();
    // followRightWall();

    // scan360(4);

}

void loop() {

    Serial.print("Current direction: ");
    Serial.println(g_car_direction);
    Serial.println("Looping");
    delay(5000);
    Serial_print_piece(g_piece); // need to figure out how to print an 80 x 80

}


/**========================================================================
 *!                    Utility functions used in setup
 *========================================================================**/
void setpins() {

    // bro wtf is this shit??
    pinMode(10, INPUT);
    pinMode(4, INPUT);
    pinMode(2, INPUT);

    // Set motor pins mode to receive power
    pinMode(L_MOTOR, OUTPUT);
    pinMode(R_MOTOR, OUTPUT);

    pinMode(R_FORWARD, OUTPUT);
    pinMode(R_BACKWARD, OUTPUT);
    pinMode(L_FORWARD,  OUTPUT);
    pinMode(L_BACKWARD, OUTPUT);

    digitalWrite(R_MOTOR, HIGH);
    digitalWrite(L_MOTOR, HIGH);

    digitalWrite(R_MOTOR, LOW);
    digitalWrite(L_MOTOR, LOW);

    Serial.println("Pins succesfully set");

}

/**========================================================================
 *!                           Movement Functions (No speed)
 *========================================================================**/
//Go forward
void forward() {
    analogWrite(L_MOTOR, carSpeed); // Turn on left side motor
    analogWrite(R_MOTOR, 0.948 * carSpeed); // compensate for physical
    digitalWrite(L_FORWARD, HIGH); // Left side move forward
    digitalWrite(L_BACKWARD, LOW);  // left side move backwards
    digitalWrite(R_FORWARD, HIGH);  // right side move forward
    digitalWrite(R_BACKWARD, LOW);   // right side move backwards
    Serial.println("Forward");
}

//Go backward
void backward() {
    analogWrite(L_MOTOR, carSpeed); //Turn on left side motor
    digitalWrite(L_FORWARD, LOW); //Backward mode on left side wheels
    digitalWrite(L_BACKWARD, HIGH);

    analogWrite(R_MOTOR, carSpeed); //Turn on right side motor
    digitalWrite(R_FORWARD, LOW); //Backward mode on right side wheels
    digitalWrite(R_BACKWARD, HIGH);
    Serial.println("Backward");
}

//Go left
void left() {
    analogWrite(L_MOTOR, carSpeed); //Turn on left side motor
    digitalWrite(L_FORWARD, LOW); //Backward mode on left side wheels
    digitalWrite(L_BACKWARD, HIGH);

    analogWrite(R_MOTOR, carSpeed); //Turn on right side motor
    digitalWrite(R_FORWARD, HIGH); //Forward mode on right side wheels
    digitalWrite(R_BACKWARD, LOW);
    Serial.println("Left");
}

//Go right
void right() {
    analogWrite(L_MOTOR, carSpeed); //Turn on left side motor
    digitalWrite(L_FORWARD, HIGH); //Forward mode on left side wheels
    digitalWrite(L_BACKWARD, LOW);

    analogWrite(R_MOTOR, carSpeed); //Turn on right side motor
    digitalWrite(R_FORWARD, LOW); //Backwrd mode on right side wheels
    digitalWrite(R_BACKWARD, HIGH);
    Serial.println("Right");
}

//Setup right rotation speed
void rightSpeed(int speed) {
  carSpeed = speed;
  right();
}

//Setup left rotation speed
void leftSpeed(int speed) {
  carSpeed = speed;
  left();
}

//Stop
void stop() {
    digitalWrite(L_MOTOR, LOW); //Turn off left side motors
    digitalWrite(R_MOTOR, LOW); //Turn off right side motors
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

/**========================================================================
 *!                           Car oriented functions
 *========================================================================**/

//*********** CAR COORDONEES ****************\\


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
    // float speedScalar = 1.32;
    float speedScalar = 1.12;
    // float speedScalar = 1.22;

    // pour combien de temps?
    float turnTime = __degrees / angVelRight250; // give us the time it takes to rotate __degrees in seconds
    float waitTime = 0;

    float surgeTime = 6.9f / angVelRight250; // overcome friction

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
    stop();

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

    turn_left(80); // this function also adjusts the variable g_car_angle
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

    turn_right(85);
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

const float carSpeedForward250 = 61.919504; // car speed in cm/s
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
int getDistance() {

   int d = sonar.ping_cm();
   if(d == 0 || d > MAX_DISTANCE)
   { return -1; } //When out of range always equal to 0 bcs when smth is close, never returns 0
   else
   { return d; }
}

/**
 * @brief Tourner 360 degrees en __n it??rations
 *
 */
void scan360(size_t __n) {

    int distance_cm = 0;
    const int delayTime = 200;
    const int stepSize = 360 / __n;

    for (size_t i = 0; i < __n; i++) {

        turn_right(stepSize);
        distance_cm = getDistance();
        delay(delayTime);

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

    }

}

/**
 * @brief Afficher a l'??cran un pi??ce dans sa repr??sentation matricielle des uint8_t
 *
 * @param __p Pi??ce a afficher
 */
void Serial_print_piece(const Piece *__p) {

    for (size_t i = 0; i < __p->nrows; i++) {

        for (size_t j = 0; j < __p->ncols; j++) {

            //  Serial.print(Piece_get(__p, i, j));
           Serial.print(Matrix_at(__p, i, j));
            Serial.print(" ");
        }

        Serial.print("\n");
    }
}

const int g_targetDistanceAdjacent = 25;

// Implement function that will follow the walls
// - start heading forward until we are ~10 cm from the wall
// - then turn 90 degrees right/left
// - turn sensor perpindicular to the wall
// - ride a long wall
void startFollowWalls() {

    // start heading north
    int distance_cm = 0;
    int n_cases = 0;

    // start with the car in middle of the matrix
    g_car_i = 39;
    g_car_j = 39;

    g_myservo.write(90);

    // FIRST STEP
    // get distance

    while (true) {

        distance_cm = getDistance();
        delay(200);

        if (distance_cm == -1) { // wall is too far away

            car_advance(10);
            delay(100);
            continue;

        } else if (distance_cm < g_targetDistanceAdjacent + 5) {

            // we are close enough to the wall
            stop();
            delay(200);
            car_turn_right();
            g_myservo.write(180);
            break;

        } else {

            // convert distance to cases
            n_cases = distance_cm / 5;
            car_advance(1);
            stop();

        }
    }
}

void followRightWall() {

    int distanceAdjacent = 0;
    int prevDistanceAdjacent = 0;
    int distanceFace = 0;

    // int minDistanceFace = ;

    while (true) {

        prevDistanceAdjacent = distanceAdjacent;

        g_myservo.write(180);
        delay(400);
        distanceAdjacent = getDistance();
        car_advance(3);

        addAdjacentWall(distanceAdjacent);

        delay(100);

        if ( distanceAdjacent < prevDistanceAdjacent ) { // la on approche le mur

            if ( distanceAdjacent < g_targetDistanceAdjacent - 15) {
                turn_right(15);
            } else if ( distanceAdjacent < g_targetDistanceAdjacent - 10) {
                turn_right(10);
            } else if ( distanceAdjacent < g_targetDistanceAdjacent - 5) {
                turn_right(5);
            } else if ( distanceAdjacent < g_targetDistanceAdjacent ) {
                turn_right(2);
            } else {
                // continue to approach the wall
            }

            // if ( distanceAdjacent < g_targetDistanceAdjacent - 15) {
            //     turn_right(10);
            // } else if ( distanceAdjacent < g_targetDistanceAdjacent - 10) {
            //     turn_right(5);
            // } else if ( distanceAdjacent < g_targetDistanceAdjacent - 5) {
            //     turn_right(2);
            // } else {
            //     // continue to approach the wall
            // }

        } else if ( distanceAdjacent > prevDistanceAdjacent ) { //la on s'??loigne du mur

            if ( distanceAdjacent > g_targetDistanceAdjacent + 15 ) {
                turn_left(10);
            } else if ( distanceAdjacent > g_targetDistanceAdjacent + 10 ) { // si l'??cart avec le mur est trop grand, corriger plus
                turn_left(5);
            } else if ( distanceAdjacent > g_targetDistanceAdjacent + 5) {
                turn_left(2);
            } else {
                // continue to approach the wall
            }
        }

        g_myservo.write(90); // look at the wall in front of us
        delay(200);

        distanceFace = getDistance();

        // process forward distance
        if ( distanceFace < g_targetDistanceAdjacent + 5) {
            stop();
            car_turn_right();
            distanceAdjacent = distanceFace;
            break;
        }
    }
}

// WALL IS ON THE LEFT SIDE OF US!!!!!
void addAdjacentWall(int distanceAdjacent) {

    if (g_car_direction == EAST) { //si la voiture longe le mur d'en haut

        Piece_set( g_piece, g_car_i - (g_targetDistanceAdjacent / 5), g_car_j, true);
        Piece_set( g_piece, g_car_i - (g_targetDistanceAdjacent / 5), g_car_j +1, true);
        Piece_set( g_piece, g_car_i - (g_targetDistanceAdjacent / 5), g_car_j +2, true);

        // Piece_set( g_piece, g_car_i - (distanceAdjacent / 5), g_car_j, true);
        // Piece_set( g_piece, g_car_i - (distanceAdjacent / 5), g_car_j +1, true);
        // Piece_set( g_piece, g_car_i - (distanceAdjacent / 5), g_car_j +2, true);

        // g_car_i =
        // g_car_j += 3;

    } else if (g_car_direction == SOUTH) { //si la voiture longe le mur de gauche
        Piece_set( g_piece, g_car_i, g_car_j + (g_targetDistanceAdjacent / 5), true);
        Piece_set( g_piece, g_car_i+1, g_car_j + (g_targetDistanceAdjacent / 5), true);
        Piece_set( g_piece, g_car_i+2, g_car_j + (g_targetDistanceAdjacent / 5), true);

        // g_car_i -= 3;
        // g_car_j = 80 - (distanceAdjacent / 5);

    } else if(g_car_direction == WEST) {  //si la voiture longe le mur du bas

        Piece_set( g_piece, g_car_i + (g_targetDistanceAdjacent / 5), g_car_j, true);
        Piece_set( g_piece, g_car_i + (g_targetDistanceAdjacent / 5), g_car_j - 1, true);
        Piece_set( g_piece, g_car_i + (g_targetDistanceAdjacent / 5), g_car_j - 2, true);

        // g_car_i = 80 - (distanceAdjacent / 5);
        // g_car_j += 3;

    } else if (g_car_direction == NORTH) { //si la voiture longe le mur de gauche

        Piece_set( g_piece, g_car_i, g_car_j - (g_targetDistanceAdjacent / 5), true); //rempli ?? la coordonn?? i et j - distance avec le mur
        Piece_set( g_piece, g_car_i-1, g_car_j - (g_targetDistanceAdjacent / 5), true); //rempli ?? la coordonn?? i - 1 et j - distance avec le mur
        Piece_set( g_piece, g_car_i-2, g_car_j - (g_targetDistanceAdjacent / 5), true);

        // g_car_i = distanceAdjacent / 5;
        // g_car_j -= 3;

    }
}