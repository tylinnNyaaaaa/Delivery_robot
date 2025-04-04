/*
  Software serial multiple serial test
 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
    10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
    8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
 */

/*  used port:
Motor 4, 5, 6, 7
Servo 3, 9
BlueTooth 10, 11
LCD a4, a5
*/

#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define motorIn1_A  4    
#define motorIn1_B  5    
#define motorIn2_A  6    
#define motorIn2_B  7  

const int DELAY = 10;
LiquidCrystal_I2C lcd(0x27,20,4); 
SoftwareSerial mySerial(10, 11); // RX, TX
Servo myservo1;  // create Servo object to control a servo
Servo myservo2;
int pos = 0; 
String receivedText = "";
int PR;
bool ade_pressure = false;

void setup() {

  //Serial Connection Area
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //BlueTooth Area
  Serial.println("Goodnight moon!");
  mySerial.begin(9600);
  mySerial.println("Hello, world?");

  //LCD Area
  lcd.init();  

  //Servo Area
  myservo1.attach(9);  // attaches the servo on pin 9 to the Servo object
  myservo2.attach(3); 

  //Motor Area
  pinMode(motorIn1_A, OUTPUT);
  pinMode(motorIn1_B, OUTPUT);
  pinMode(motorIn2_A, OUTPUT);
  pinMode(motorIn2_B, OUTPUT);  

  //Order Info (LCD monitor)
  //LCD Area
  lcd.backlight();

  lcd.setCursor(3,0);
  lcd.print("Hi, I'm here to  get food delivery!");

}

  void loop() {

  //BlueTooth, Motor, LCD text display
   if (mySerial.available()) {
        char receivedChar = mySerial.read(); // read a char
        Serial.write(receivedChar); //test
        
        // if receive #, a sentence end
        if (receivedChar == '#') {
            // if receive motor control command
            if (receivedText.length() == 1) {
                char command = receivedText[0]; // take the first char, emit #
                if (command == 's') {
                  Serial.write("s");
                  forward();
                }
                else if (command == 'b') {
                  Serial.write("b");
                  backward();
                }
                else if (command == 'l') {
                  Serial.write("l");
                  left();
                }
                else if (command == 'r') {
                  Serial.write("r");
                  right();
                }
                else if (command == 'x') {
                  Serial.write("x");
                  stop();
                }
                else if (command == 'o') {
                  Serial.write("o");
                  openlid();
                }
                else if (command == 'c') {
                  Serial.write("c");
                  closelid();
                }
            }
            else
            {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print(receivedText);
            }

            receivedText = ""; // clear the port
        }  
        // **普通字符则存储**
        else {
            receivedText += receivedChar;
        }
    }

    PR = analogRead(A0); //0-1023
    Serial.println(PR);
    delay(300);

    //if detect pressure, open the lid
    if(PR>100 && ade_pressure == false)
    {
      ade_pressure = true;
      openlid();
    }
    
}

//motor control functions:
void stop()
{
  digitalWrite(motorIn1_A, LOW);
  digitalWrite(motorIn1_B, LOW);
  digitalWrite(motorIn2_A, LOW);
  digitalWrite(motorIn2_B, LOW);
}

//前進
void forward()
{
  digitalWrite(motorIn1_A, HIGH);
  digitalWrite(motorIn1_B, LOW);
  digitalWrite(motorIn2_A, HIGH);
  digitalWrite(motorIn2_B, LOW);
}

//後退
void backward()
{
  digitalWrite(motorIn1_A, LOW);
  digitalWrite(motorIn1_B, HIGH);
  digitalWrite(motorIn2_A, LOW);
  digitalWrite(motorIn2_B, HIGH);
}

//右轉
void right()
{
  digitalWrite(motorIn1_A, LOW);
  digitalWrite(motorIn1_B, LOW);
  digitalWrite(motorIn2_A, HIGH);
  digitalWrite(motorIn2_B, LOW);
}

// 左轉
void left()
{
  digitalWrite(motorIn1_A, HIGH);
  digitalWrite(motorIn1_B, LOW);
  digitalWrite(motorIn2_A, LOW);
  digitalWrite(motorIn2_B, LOW);
}

// money case's lid control
void openlid()
{
  for (pos = 30; pos <= 120; pos += 1) {
    myservo1.write(pos);             
    delay(15);                      
    myservo2.write(120-pos);              
    delay(15); 
  }
}

void closelid()
{
  for (pos = 120; pos >= 30; pos -= 1) { 
    myservo1.write(pos);              
    delay(15);                    
    myservo2.write(120-pos);              
    delay(15);                      
  }
  ade_pressure = false;
}