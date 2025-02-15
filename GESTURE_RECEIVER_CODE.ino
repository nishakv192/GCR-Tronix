// Arduino Gesture Control Robot
// Receiver Circuit 
// Created BY DIY Builder
// Ensure required libraries are installed before uploading.
// To install libraries: Download from https://github.com/nRF24/RF24, then go to Sketch > Include Library > Add .ZIP Library > Select the downloaded ZIP file.
// For more details, visit www.instagram.com/diy.builder

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Motor Control Pins
int ENA = 6;      // Changed from 3 to 2 to avoid conflict with RF24 CE
int ENB = 5;      // Changed from 9 to 8 to avoid conflict with RF24 CE
int MotorA1 = 3;
int MotorA2 = 2;
int MotorB1 = 7;
int MotorB2 = 4;

// RF24 Object (CE=9, CSN=10)
RF24 radio(9, 10);

// Communication Address
const byte address[6] = "00001";

// Data Structure
struct data {
  int xAxis;
  int yAxis;
};
data receive_data;

// LED Indicator Pin (Optional)
const int ledPin = 3; // Using pin 3 for LED to avoid conflict

void setup() {
  Serial.begin(9600);               // Initialize serial communication
  pinMode(ENA, OUTPUT);             // Set ENA as OUTPUT
  pinMode(ENB, OUTPUT);             // Set ENB as OUTPUT
  pinMode(MotorA1, OUTPUT);         // Set MotorA1 as OUTPUT
  pinMode(MotorA2, OUTPUT);         // Set MotorA2 as OUTPUT
  pinMode(MotorB1, OUTPUT);         // Set MotorB1 as OUTPUT
  pinMode(MotorB2, OUTPUT);         // Set MotorB2 as OUTPUT
  pinMode(ledPin, OUTPUT);          // Set LED pin as OUTPUT (Optional)

  // Initialize RF24 Module
  if (!radio.begin()) {
    Serial.println("Radio initialization failed!");
    while (1); // Halt if initialization fails
  }

  radio.openReadingPipe(0, address);  // Open reading pipe on specified address
  radio.setPALevel(RF24_PA_HIGH);     // Set Power Amplifier level to high for better range
  radio.setDataRate(RF24_250KBPS);    // Set data rate to 250 Kbps
  radio.startListening();              // Put module into listening mode

  Serial.println("Receiver ready.");
}

void loop() {
  if (radio.available()) { // Check if data is available
    radio.read(&receive_data, sizeof(data)); // Read incoming data

    // Debugging: Print received data
    Serial.print("Received X: ");
    Serial.println(receive_data.xAxis);
    Serial.print("Received Y: ");
    Serial.println(receive_data.yAxis);

    // Control Motors Based on Received Data
    if (receive_data.yAxis > 400) {
      // Move Forward
      digitalWrite(MotorA1, LOW);
      digitalWrite(MotorA2, HIGH);
      digitalWrite(MotorB1, HIGH);
      digitalWrite(MotorB2, LOW);
      analogWrite(ENA, 150);
      analogWrite(ENB, 150);
      digitalWrite(ledPin, HIGH); // Optional: Turn LED on
    } 
    else if (receive_data.yAxis < 320) {
      // Move Backward
      digitalWrite(MotorA1, HIGH);
      digitalWrite(MotorA2, LOW);
      digitalWrite(MotorB1, LOW);
      digitalWrite(MotorB2, HIGH);
      analogWrite(ENA, 150);
      analogWrite(ENB, 150);
      digitalWrite(ledPin, HIGH); // Optional: Turn LED on
    } 
    else if (receive_data.xAxis < 320){
      // Turn Left
      digitalWrite(MotorA1, HIGH);
      digitalWrite(MotorA2, LOW);
      digitalWrite(MotorB1, HIGH);
      digitalWrite(MotorB2, LOW);
      analogWrite(ENA, 150);
      analogWrite(ENB, 150);
      digitalWrite(ledPin, HIGH); // Optional: Turn LED on
    }
    else if (receive_data.xAxis > 400){
      // Turn Right
      digitalWrite(MotorA1, LOW);
      digitalWrite(MotorA2, HIGH);
      digitalWrite(MotorB1, LOW);
      digitalWrite(MotorB2, HIGH);
      analogWrite(ENA, 150);
      analogWrite(ENB, 150);
      digitalWrite(ledPin, HIGH); // Optional: Turn LED on
    }
    else {
      // Stop Movement
      digitalWrite(MotorA1, LOW);
      digitalWrite(MotorA2, LOW);
      digitalWrite(MotorB1, LOW);
      digitalWrite(MotorB2, LOW);
      analogWrite(ENA, 0);
      analogWrite(ENB, 0);
      digitalWrite(ledPin, LOW);  // Optional: Turn LED off
    }

    delay(100); // Short delay for stability
  }
}
