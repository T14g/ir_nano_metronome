#define IR_USE_AVR_TIMER1

#include <IRremote.h>
IRrecv IR(2);

// Tempo variables
int tempo = 0;
int previousTempo = 0;
unsigned long beatInterval;
unsigned long previousMillis = 0;

int modeSelected = 0;

// Leds
const int LED_1 = 5;
const int LED_2 = 8;
const int LED_3 = 7;
const int LED_4 = 6;
const int BUZZER_PIN = 3;

// 4/4 Time Signature
const int beatsPerMeasure = 4;
int beatCounter = 1;

void calculateBeatInterval() {
  beatInterval = 60000 / tempo; // Convert tempo to beat interval in milliseconds
}

void turnLedsOFF() {
    int ledState1 = digitalRead(LED_1);
    int ledState2 = digitalRead(LED_2);
    int ledState3 = digitalRead(LED_3);
    int ledState4 = digitalRead(LED_4);

    if(ledState1 == HIGH || ledState2 == HIGH || ledState3 == HIGH || ledState4 == HIGH) {
      digitalWrite(LED_1, LOW);    // First LED off
      digitalWrite(LED_2, LOW);  // Second LED on
      digitalWrite(LED_3, LOW);   // Third LED off
      digitalWrite(LED_4, LOW);   // Fourth LED off
    }
}

void metronome() {
  // Calculate the beat interval
  if (tempo != previousTempo) {
    // Calculate the beat interval
    calculateBeatInterval();
    // Update the previous tempo value
    previousTempo = tempo;
  }
  // Check if it's time for a beat
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= beatInterval) {
    // Toggle LED state for the current beat
    if (beatCounter == 1) {
      digitalWrite(LED_1, HIGH);   // First LED on
      digitalWrite(LED_2, LOW);   // Second LED off
      digitalWrite(LED_3, LOW);   // Third LED off
      digitalWrite(LED_4, LOW);   // Fourth LED off
    } else if (beatCounter == 2) {
      digitalWrite(LED_1, LOW);    // First LED off
      digitalWrite(LED_2, HIGH);  // Second LED on
      digitalWrite(LED_3, LOW);   // Third LED off
      digitalWrite(LED_4, LOW);   // Fourth LED off
    } else if (beatCounter == 3) {
      digitalWrite(LED_1, LOW);    // First LED off
      digitalWrite(LED_2, LOW);   // Second LED off
      digitalWrite(LED_3, HIGH);  // Third LED on
      digitalWrite(LED_4, LOW);   // Fourth LED off
    } else if (beatCounter == 4) {
      digitalWrite(LED_1, LOW);    // First LED off
      digitalWrite(LED_2, LOW);   // Second LED off
      digitalWrite(LED_3, LOW);   // Third LED off
      digitalWrite(LED_4, HIGH);  // Fourth LED on
    }

    // Increment beat counter
    beatCounter++;

    // Play the metronome sound
    if (beatCounter == 1 || beatCounter == 2) {
      tone(BUZZER_PIN, 1000, 100); // Play a click sound for the first and third beat
    } else {
      tone(BUZZER_PIN, 800, 100); // Play a softer click sound for the second and fourth beat
    }

    // Check if a measure is completed
    if (beatCounter > beatsPerMeasure) {
      beatCounter = 1; // Reset the beat counter for a new measure
    }

    // Store current time
    previousMillis = currentMillis;
  }
}

void setup() {
  // put your setup code here, to run once:
  // Serial.begin(9600);
  IR.enableIRIn();
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Calculate beat interval based on tempo
  calculateBeatInterval();
}

void loop() {
  if (IR.decode()) {
    IR.resume();
    if(IR.decodedIRData.command == 0x10) {
      tempo = 100;
    }else if(IR.decodedIRData.command == 0x11) {
      tempo = 200;
    }else if(IR.decodedIRData.command == 0x12) {
      tempo = 0;
      turnLedsOFF();
    }
    delay(1000);
  }
  if(tempo > 0) {
    metronome();
  }
}
