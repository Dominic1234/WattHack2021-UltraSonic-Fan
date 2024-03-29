#include <Servo.h>
#include <LiquidCrystal.h>

#define LEFT_ANGLE 145
#define RIGHT_ANGLE 35
#define REST_ANGLE 90

#define MIN_DIST 2
#define MAX_DIST 100

#define RIGHT_LED 8
#define LEFT_LED 4
#define MIDDLE_LED 5

//
////Servo declarations
  int pos = 0;
//
 Servo servo_9;

 // initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 13, en = 10, d4 = A5, d5 = A4, d6 = A3, d7 = A2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//US sensor declarations
int inches = 0;
int cm_1 = 0;
int cm_2 = 0;
int cm_3 = 0;
double distance_arr[3];
int min_val = 0;
int my_indx = 0;

double ang;
long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT); // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  //Sets the trigger pin to HIGH state for 10 ms.

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin,LOW);
  pinMode(echoPin, INPUT);
  //Reads echopin and returns the sound wave travel

  return pulseIn(echoPin,HIGH);
  
}

void calc_angle(double curr_ang, double new_ang) { 
  if(curr_ang > new_ang) {
    Serial.println("moving right");
    for(double i = curr_ang; i > new_ang; i--) {
      servo_9.write(i);
      delay(12);
    }
  }
  else if(curr_ang < new_ang){
    Serial.println("moving left");
    for(double i = curr_ang; i < new_ang; i++) {
      servo_9.write(i);
      delay(12);
    }
  }
  else {
    return;
  }
}

void setup() {
  Serial.begin(9600);
  
  //begin motor setup
  servo_9.attach(9);
  servo_9.write(REST_ANGLE);
  
  //begin LCD setup
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);

  //setup LED
  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
  pinMode(MIDDLE_LED, OUTPUT);
 
}
void loop() {
  
  //US sensor code
  // put your main code here, to run repeatedly:
  //meausre ping time in cm
  cm_1 = 0.01723 * readUltrasonicDistance(7,6); //middle
  cm_2 = 0.01723 * readUltrasonicDistance(12,11); //right
  cm_3 = 0.01723 * readUltrasonicDistance(2,3); // left
  distance_arr[0] = cm_1;
  distance_arr[1] = cm_2;
  distance_arr[2] = cm_3;

  //[cm1, cm2. cm3]
  min_val = cm_1;
  my_indx = 0;

  for(int i = 0; i < 3; i++){
      if ( distance_arr[i] < min_val){
        min_val = distance_arr[i];
        my_indx = i;
      }
  }

  if(min_val < MAX_DIST){
    ang = servo_9.read();
    if( my_indx == 0){                          //center
        lcd.clear();
        lcd.print("centering");
        digitalWrite(LEFT_LED, LOW);
        digitalWrite(RIGHT_LED, LOW);
        digitalWrite(MIDDLE_LED, HIGH);
        calc_angle(ang, REST_ANGLE);
    }  else if  (my_indx == 1){                 //right
        lcd.clear();
        lcd.print("moving right");
        digitalWrite(LEFT_LED, LOW);
        digitalWrite(RIGHT_LED, HIGH);
        digitalWrite(MIDDLE_LED, LOW);
        calc_angle(ang, RIGHT_ANGLE);
    }else if (my_indx == 2){                     //left
      lcd.clear();
      lcd.print("moving left");
      digitalWrite(LEFT_LED, HIGH);
      digitalWrite(RIGHT_LED, LOW);
      digitalWrite(MIDDLE_LED, LOW);
      calc_angle(ang, LEFT_ANGLE);
    }
    }else{
      lcd.print("cannot find");
      servo_9.write(ang);
      //to add blinking lights if all sensors out of maximum set range
      }
    
      //Servo Code
      Serial.print(cm_2);
      Serial.print(",");
      Serial.print(cm_1);
      Serial.print(",");
      Serial.println(cm_3);
    
      delay(250);
}
