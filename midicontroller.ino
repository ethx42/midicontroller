/*
 * Basic MIDI Controller
 * This code transforms a Teensy microcontroller into a MIDI controller with 10 potentiometers.
 * Each potentiometer can send MIDI Control Change messages to any MIDI-compatible software or hardware.
 * Designed for real-time video/music performance and production.
 * 
 * Author: Santiago Torres - https://github.com/ethx42
 */


const int numPots = 10; // Total number of potentiometers.
int potPin[numPots] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};
int potCState[numPots] = {0}; // Holds current potentiometer readings.
int potPState[numPots] = {0}; // Holds previous potentiometer readings.
int potVar = 5; // Sensitivity for detecting potentiometer changes.

// MIDI Control Change (CC) numbers for each potentiometer.
int midiCC[numPots] = {1, 7, 10, 11, 71, 74, 77, 80, 83, 86};

const int channel = 5; // MIDI channel for sending messages.

// LED Blink variables
const int ledPin = LED_BUILTIN; // Use Teensy's built-in LED
const long ledOnDuration = 10; // Duration LED stays on (milliseconds)
unsigned long previousBlinkMillis = 0; // Stores last time LED was updated
const long blinkInterval = 2500; // Interval at which to blink (milliseconds)

void setup() {
  for (int i = 0; i < numPots; i++) {
    pinMode(potPin[i], INPUT);
  }
  pinMode(ledPin, OUTPUT); // Initialize the LED pin as an output
  Serial.begin(9600); // For debugging.
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Blink LED for a brief moment every 750ms
  if (currentMillis - previousBlinkMillis >= blinkInterval) {
    previousBlinkMillis = currentMillis; // Save the last time you blinked the LED
    digitalWrite(ledPin, HIGH);
  } else if (currentMillis - previousBlinkMillis >= ledOnDuration) {
    digitalWrite(ledPin, LOW);
  }

  for (int i = 0; i < numPots; i++) {
    potCState[i] = analogRead(potPin[i]) >> 3; // Convert 0-1023 to 0-127 range.

    if (abs(potCState[i] - potPState[i]) > potVar) {
      usbMIDI.sendControlChange(midiCC[i], potCState[i], channel); // Send CC message.

      // Debugging output: Uncomment to track potentiometer changes.
      // Serial.print("Pot ");
      // Serial.print(i);
      // Serial.print(": ");
      // Serial.println(potCState[i]);

      potPState[i] = potCState[i]; // Update previous state.
    }
  }
  delay(10); // Maintain MIDI communication stability.

  // Clear the USB MIDI read buffer.
  while (usbMIDI.read());
}
