#include "SoftwareSerial.h"         // Include the SoftwareSerial library for serial communication
#include "DFRobotDFPlayerMini.h"    // Include the DFRobotDFPlayerMini library for the DFPlayer Mini module

SoftwareSerial mySoftwareSerial(27, 26); // Create a software serial connection on pins 27 (RX) and 26 (TX)
DFRobotDFPlayerMini myDFPlayer;          // Create a DFPlayerMini object

const int FlameSensor = D7;             // Define the pin number for the push button
bool isPlaying = false;                   // Flag to indicate if the track is playing
unsigned long lastPlayTime = 0;           // Store the last time the track was played
const unsigned long playDuration = 3800;  // Set the duration for which the track will play

void setup() {
    mySoftwareSerial.begin(9600);          // Start software serial communication at 9600 baud rate
    Serial.begin(115200);                  // Start serial communication at 115200 baud rate
    pinMode(FlameSensor, INPUT);  // Set push button pin as input with internal pull-up resistor

    if (!myDFPlayer.begin(mySoftwareSerial)) { // Initialize the DFPlayer Mini module
        Serial.println(F("Not initialized:"));
        Serial.println(F("1. Check the DFPlayer Mini connections"));
        Serial.println(F("2. Insert an SD card"));
        while (true);                        // If initialization fails, print error messages and halt the program
    }

    Serial.println();
    Serial.println(F("DFPlayer Mini module initialized!")); // Print initialization success message
    myDFPlayer.setTimeOut(500);            // Set the timeout value for serial communication
    myDFPlayer.volume(60);                 // Set the volume level (0 to 30)
    myDFPlayer.EQ(0);                      // Set the equalizer setting (0: Normal, 1: Pop, 2: Rock, etc.)
}

void loop() {
    int buttonState = digitalRead(FlameSensor);  // Read the current button state

    // If the button is pressed, check if the song should be played or restarted
    if (buttonState == LOW) {                      // Button pressed (LOW with INPUT_PULLUP)
        if (!isPlaying) {                          // If no track is currently playing
            playSong(5);                           // Play the third audio file on the SD card
        } else if (millis() - lastPlayTime >= playDuration) {
            playSong(5);                           // Restart the song if play duration has passed
        }
    } else if (isPlaying && (millis() - lastPlayTime >= playDuration)) {
        stopSong();                                // Stop the song if button is released and play duration has passed
    }
}

// Function to play a song on DFPlayer
void playSong(uint8_t song) {
    myDFPlayer.play(song);                   // Play the specified audio file
    lastPlayTime = millis();                 // Record the time the track started playing
    isPlaying = true;                        // Set the flag to indicate the track is playing
    Serial.println("Playing song " + String(song)); // Log the song playing
}

// Function to stop the song
void stopSong() {
    myDFPlayer.stop();                       // Stop the song
    isPlaying = false;                       // Reset the flag to allow the next button press to play the track again
    Serial.println("Track finished playing"); // Log track finish
}
