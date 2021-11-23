#include <Servo.h>
#include <LiquidCrystal.h>

#define RIGHT_ANGLE 170
#define LEFT_ANGLE 0
#define REST_ANGLE 90

#define MAX_DIST 100

//
////Servo declarationss
  int pos = 0;
//
 Servo servo_9;

 // initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 13, en = 10, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
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
    lcd.print("moving right");
    for(double i = curr_ang; i > new_ang; i--) {
      servo_9.write(i);
      delay(25);
    }
  }
  else if(curr_ang < new_ang){
    Serial.println("moving left");
    lcd.print("moving left");
    for(double i = curr_ang; i < new_ang; i++) {
      servo_9.write(i);
      delay(25);
    }
  }
  else {
    lcd.print("at rest");
    return;
  }
}

void setup() {
  Serial.begin(9600);
  
  //begin motor setup
  servo_9.attach(9);
  
  //begin LCD setup
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
 
}
void loop() {
  
  //Set LCD cursor to the next line
  lcd.setCursor(0, 1);
  
  //US sensor code
  // put your main code here, to run repeatedly:
  //meausre ping time in cm
  cm_1 = 0.01723 * readUltrasonicDistance(7,6);
  cm_2 = 0.01723 * readUltrasonicDistance(12,11);
  cm_3 = 0.01723 * readUltrasonicDistance(2,3);
  distance_arr[0] = cm_1;
  distance_arr[1] = cm_2;
  distance_arr[2] = cm_3;

  //[cm1, cm2. cm3]
  
  min_val = cm_1;
  my_indx = 0;

  for(int i = 0; i < 3; i++){
    if ( distance_arr[i] < min_val && distance_arr[i] < MAX_DIST){
      min_val = distance_arr[i];
      my_indx = i;
    }
  }
  if(min_val < MAX_DIST){
    
ang = servo_9.read();
if( my_indx == 0){          //right
  calc_angle(ang, REST_ANGLE);
}  else if  (my_indx == 1){ //center
  calc_angle(ang, LEFT_ANGLE);
}else {                     //left
  calc_angle(ang, RIGHT_ANGLE);
}

//if( my_indx == 0){
//  servo_9.write(90);
//}  else if  (my_indx == 1){
//  servo_9.write(0);
//}else {
//  servo_9.write(170); 
//}

 //(my_indx == 2)
  
    //Serial.print("min value is");
    //Serial.println(min_val);
//    Serial.print("minimum index is");
//    Serial.println(my_indx);
    
  
  //convert to inches 
  //inches = (cm/2.54);

}else{
  lcd.print("cannot find you!");
  servo_9.write(ang);
  //to add blinking lights if all sensors out of maximum set range
  }

  //Servo Code

  Serial.print(cm_1);
  Serial.print(",");
  Serial.print(cm_2);
  Serial.print(",");
  Serial.println(cm_3);
  Serial.println(cm_3);

  delay(500);
}
