#define VERSION "UEBELSTGEILON_1"

#include <Wire.h> 
#include <LiquidCrystal_I2C.h> //Display
#include <DistanceGP2Y0A21YK.h>

//Settings
#define DEBUG
#define LINE_FOLLOWING
#define DRIVE 200
#define OBSTACLE
#define DISPLAY_OUT
#define ULTRASONIC
//#define SHARP_DIS

//Spezifische Fuxnktionen
//#define TOUCH_RESCUE
//#define SEARCH 

#define LED 13
#define Off() OnFwd(FORWARD,0,0) //Konstante für Off = Ausschalten der Motoren
#define WHITE digitalRead(light[1]) == 1 && digitalRead(light[2]) == 1 && digitalRead(light[3]) == 1  && digitalRead(light[4]) == 1 && digitalRead(light[5]) == 1  //Konstante für alle Lichtsensoren Weiss
//Greenshield
#define GR_INPUT 19
#define GR_RESET 46


//Sharp - Ultraschallsensoren
#define SHARP1 A1 
#define SHARP2 A2
#define SHARP3 A3 // HINTEN

//Ultraschallsensoren
#define trigL 50
#define pwmL 51

#define trigR 52
#define pwmR 53

//Touch Sensoren
#define TOUCH1 30
#define TOUCH2 31

//Motoren
#define M1 7
#define E1 6
#define E2 5                     
#define M2 4

#define ODOML 00
#define ODOMR 00


//OnFwd
const int FORWARD = 1;  
const int BACKWARD = 2;
const int RIGHT = 3;                         
const int LEFT = 4;
const int OFF = 0;

//Servos
#define SERVO1 46
#define SERVO2 22
#define SERVO3 28

#ifdef DRIVE
int dspeed = DRIVE;
#else
int dspeed = 190;
#endif

//Odometer
int counterLeft = 0;
int counterRight = 0;

int counter = 0;

int oldcounter = 0;

//Lichtarray
int light[] = {
  0,14,15,16,17,18};
  
//Display Initialisierung 
LiquidCrystal_I2C lcd(0x27,16,2); //0x27

void loop(){};



