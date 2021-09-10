/*
  Team 14
  by Arnav Choudury
  This program regcognizes a password and opens the lock with a key.
*/
#include<LiquidCrystal.h>
#include<IRremote.h>
#include<Servo.h>



// all the button codes from the elegoo remote
#define NUM0  0xFF6897
#define NUM1  0xFF30CF
#define NUM2  0xFF18E7
#define NUM3  0xFF7A85
#define NUM4  0xFF10EF
#define NUM5  0xFF38C7
#define NUM6  0xFF5AA5
#define NUM7  0xFF42BD
#define NUM8  0xFF4AB5
#define NUM9  0xFF52AD
#define REST  0xFFB04F
#define HELD  0xFFFFFF

#define SERVO_PIN 8

#define IR_PIN 9
//uses the active buzzer
#define P_PIN 11

//pins for ultra sonic
/*
  #define PING 12
  #define ECHO 13
*/
IRrecv irrecv(IR_PIN);
decode_results results;

Servo servo;
LiquidCrystal lcd = LiquidCrystal(2, 3, 4, 5, 6, 7);
int password[] = {0, 1, 2, 3};
int entered[] = {-1, -1, -1, -1};

void setup() {
  //serial setup
  Serial.begin(9600);
  //servo setup
  servo.attach(SERVO_PIN);
  servo.write(0);
  //screen set up
  lcd.begin(16, 2);
  //set up ir sensor
  irrecv.enableIRIn();
  irrecv.blink13(true);

  //sound setup
  pinMode(P_PIN, OUTPUT);

  //ultrasonic sound setup
  /*
    pinMode(PING,OUTPUT);
    pinMode(ECHO,INPUT);
  */
}

int i = 0;
int wrong = 0;
void loop()
{ /*
    digitalWrite(PING, LOW);
    delayMicroseconds(2);
    digitalWrite(PING, HIGH);
    delayMicroseconds(10);
    digitalWrite(PING, LOW);

    if(msTI(pulseIn(ECHO,HIGH))>3){
    alarm();
    }
  */
  //Serial.println(NUM0,HEX);
  lcd.setCursor(2, 0);
  lcd.write("Enter Code");
  if (irrecv.decode(&results))// Returns 0 if no data ready, 1 if data ready.
  {
    Serial.println("RECIEVED Button");
    if (nextPin(results.value) > -1) {
      Serial.println("Got a number");
      entered[i] = nextPin(results.value);
      i++;
      delay(100);
    }
    else if (nextPin(results.value) == -2) {
      servo.write(0);
      entered[0] = -1;
      entered[1] = -1;
      entered[2] = -1;
      entered[3] = -1;
      i = 0;
      wrong = 0;
    }
    irrecv.resume();

  }
  if (i == 4) { //length of password has to equal currentConditon
    if (passWordCheck(entered, password)) {
      servo.write(100);
      lcd.setCursor(2, 1);
      lcd.print("Correct Code!");
      entered[0] = -1;
      entered[1] = -1;
      entered[2] = -1;
      entered[3] = -1;
      wrong = 0;
      i = 0;
      delay(1500);
      lcd.clear();
    }
    else {
      lcd.setCursor(2, 1);
      lcd.print("Wrong Code");
      entered[0] = -1;
      entered[1] = -1;
      entered[2] = -1;
      entered[3] = -1;
      if (wrong == 3) {
        wrong = 0;
        alarm();
      }
      i = 0;
      delay(1500);
      wrong++;

      delay(500);
      lcd.clear();
    }
  }
}
bool passWordCheck(int currentCondition[], int password[]) {
  //checks if the password matches
  for (int i = 0; i < 4; i++) {
    if (currentCondition[i] != password[i]) {
      return false;
    }

  }
  return true;
}
int nextPin(int nextEntry) {
  //matches the ircodes with the number for a pin
  switch (nextEntry) {
    case NUM0:
      Serial.println(0);
      return 0;
      break;
    case NUM1:
      Serial.println(1);
      return 1;
      break;
    case NUM2:
      Serial.println(2);
      return 2;
      break;
    case NUM3:
      Serial.println(3);
      return 3;
      break;
    case NUM4:
      Serial.println(4);
      return 4;
      break;
    case NUM5:
      Serial.println(5);
      return 5;
      break;
    case NUM6:
      Serial.println(6);
      return 6;
      break;
    case NUM7:
      Serial.println(7);
      return 7;
      break;
    case NUM8:
      Serial.println(8);
      return 8;
      break;
    case NUM9:
      Serial.println(9);
      return 9;
      break;
    case HELD:
      Serial.println("HELD");
      return -1;
      break;
    case REST:
      Serial.println("Reset");
      return -2;
      break;
    default:
      Serial.println("Not a button");
      return -1;
      break;
  }
}
void alarm() {
  int j;
  for (j = 0; j < 1000; j++) {
    digitalWrite(P_PIN, LOW);
    delay(1);//wait for 1ms
    digitalWrite(P_PIN, HIGH);
    delay(1);//wait for 1ms

  }
  delay(100);
  digitalWrite(P_PIN, LOW);

}
long msTI(long microseconds) {
  return microseconds / 74 / 2;
}