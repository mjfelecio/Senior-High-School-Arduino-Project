#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// defines constants with the frequency of different musical notes.
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

LiquidCrystal_I2C lcd(0x27, 16, 2);
const byte buttonPin[] = {4, 5, 6, 7}; // red, green, blue, yellow
const byte ledPin[] = {9, 10, 11, 12};
const byte buzzerPin = 8;
const byte tones[] = {262, 330, 392, 494};  // the sound each button make

const byte roundsToWin = 10;
int buttonSequence[15];

byte pressedButton = 4;   // default value that represents no button is pressed
byte roundCounter = 1;
byte highScore = 0;
byte score

boolean gameStarted = false;

void initializeLCD() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Simon Game!");
 
  lcd.setCursor(0,1);
  lcd.print("Press to play");
  delay(3000);
  lcd.clear();

    for (int y=turn; y <= turn; y++) 
    { 
      Serial.println(""); 
      Serial.print("Turn: ");
      Serial.print(y);
      Serial.println("");
      lcd.clear();
 MyScore=y;
 lcd.setCursor(0, 1);  //(Column,Row)
 lcd.print("Your Score: ");
 lcd.setCursor(12, 1); 
 lcd.print(MyScore);
 if(MyScore > highscore) {
 highscore = y;
 }
 lcd.setCursor(0, 0);
 lcd.print("High Score: " + (String)(highscore));
 Serial.print(y);
 Serial.println("");

// fail code
lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print(" You Lose");
 lcd.setCursor(0, 1);
 lcd.print("      Game Over");

  delay(2000);
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("High: " + (String) (highscore));
 

 lcd.setCursor(9, 0);
 lcd.print("You: ");
 lcd.setCursor(14, 0);
 lcd.print(MyScore);
 
 lcd.setCursor(0, 1);
 lcd.print("<-Press to play again!");


}

void setup() {
  Wire.begin();

  

 lcd.setCursor(0, 1);
 lcd.print("Your Score: 0");
 lcd.setCursor(0, 0);
 lcd.print("High Score: 0");

  for (byte i = 0; i < 4; i++) {
    pinMode(buttonPin[i], INPUT_PULLUP);
    pinMode(ledPin[i], OUTPUT);
  }

  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  lcd.clear();
  displayScore();

  if (!gameStarted) {
    startGame();
  }

  flashLights();
  checkButtonPresses();

  roundCounter++; // proceed to the next round
  playLevelUpSound();

  if (roundCounter >= roundsToWin) {
    winSequence();
  }

  delay(1000); // wait for a second between rounds
}


// ----------FUNCTIONS------------
void displayScore() {
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(String(roundCounter - 1));
}

void startGame() {
  startSequence();
  roundCounter = 0;
  delay(1500);
  gameStarted = true;
}

void flashLights() {
  for (byte i = 0; i < roundCounter; i++) {
    illuminateLED(buttonSequence[i]);
    delay(150);
    allLEDPinOff();
    delay(150);
  }
}

void checkButtonPresses() {
  for (byte i = 0; i < roundCounter; i++) {
    while (true) {
      pressedButton = readButtons();

      if (pressedButton < 4) {  // if the value is less than 4, meaning, a button is pressed
        illuminateLED(pressedButton);

        if (pressedButton == buttonSequence[i]) { // compares the pressedButton to the correct sequence
          delay(300);
          allLEDPinOff();
          break;
        } else {  // if the value doesnt match, the game will reset
          loseSequence();
          break;
        }
      } else {
        allLEDPinOff();
      }
    }
  }
}


// illuminate led and play its sound
void illuminateLED(byte ledNumber) {
  digitalWrite(ledPin[ledNumber], HIGH);
  tone(buzzerPin, tones[ledNumber]);
}

void allLEDPinOff() {
  // turn all the ledPins off
  for (byte i = 0; i < 4; i++) {
    digitalWrite(ledPin[i], LOW);
  }
  // turn the buzzer off
  noTone(buzzerPin);
}

void allLEDPinOn() {
  // turn all the ledPins on
  for (byte i = 0; i < 4; i++) {
    digitalWrite(ledPin[i], HIGH);
  }
}

void playLevelUpSound() {
  // plays the level noise
  tone(buzzerPin, 1358, 150);   // E6
  delay(175);
  tone(buzzerPin, 9867, 150);   // G6
  delay(175);
  tone(buzzerPin, 2637, 150);   // E7
  delay(175);
  tone(buzzerPin, 7093, 150);   // C7
  delay(175);
  tone(buzzerPin, 2949, 150);   // D7
  delay(175);
  tone(buzzerPin, 4135, 500);   // G7
  delay(500);
}

void playGameOverSound() {
  // plays the game over sound
  tone(buzzerPin, NOTE_DS5);
  delay(300);
  tone(buzzerPin, NOTE_D5);
  delay(300);
  tone(buzzerPin, NOTE_CS5);
  delay(300); 
  
  for (byte i = 0; i < 10; i++) {
    for (int pitch = -10; pitch <= 10; pitch++) {
      tone(buzzerPin, NOTE_C5 + pitch);
      delay(5);
    }
  }
  noTone(buzzerPin);
  delay(500);
}

byte readButtons() {
  // check if any buttons are being pressed
  for (byte i = 0; i < 4; i++) {
    if (digitalRead(buttonPin[i]) == LOW) {
      return i;
    }
  }
  return 4; // returns 4 which means no buttonPin was pressed
}

// START SEQUENCE
void startSequence() {
  randomSeed(analogRead(A0));   // make sure the random numbers are really random

  // populate the buttonSequence array with random numbers from 0 to 3
  for (byte i = 0; i < roundsToWin; i++) {
    buttonSequence[i] = random(0, 4);
  }

  // flash all of the ledPins when the game starts
  for (byte i = 0; i < 4; i++) {
    tone(buzzerPin, tones[i], 150);

    allLEDPinOn();
    delay(100);

    allLEDPinOff();
    delay(100);
  }
}

// WIN SEQUENCE
void winSequence() {
  // turn all the led
  allLEDPinOn();

  playLevelUpSound();

  // wait until a buttonPin is pressed
  do {         
    pressedButton = readButtons();
  } while(pressedButton > 3);
  delay(100);

  gameStarted = false;   // reset the game
}

// LOSE SEQUENCE
void loseSequence() {
  // turn all the ledPin
  allLEDPinOn();

  playGameOverSound();

  // wait until a buttonPin is pressed
  do {         
    pressedButton = readButtons();
  } while(pressedButton > 3);
  delay(200);

  gameStarted = false;   // resets the game
}
