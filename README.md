# CS120B
Contains completed labs from Cs120B: Introduction to Embedded Systems


# SoundBooth Program(Final Project)
# Overview
- The SoundBooth program is an interactive sound player designed for Arduino platforms, capable of playing multiple pre-programmed songs. 
- It utilizes digital input (such as a joystick or buttons) to allow users to select and control the playback of songs.
- The program also incorporates an LCD display to provide visual feedback on the current operation, such as song selection and playback status.

# Features
- Multiple Songs: Comes preloaded with a selection of songs defined in arrays, utilizing pitch definitions from pitches.h for melody playback.
- Interactive Controls: Uses a digital input device (joystick or button) to select and control song playback. Users can play, pause, and switch between songs.
- LCD Feedback: Displays current song and playback status on an LCD, enhancing the user interface.
- Modular Design: Code structured with task scheduling in mind, making it easy to extend or modify for additional functionalities.
# Hardware Requirements
- Arduino board (Uno, Mega, etc.)
- Buzzer or speaker for sound output
- Joystick or buttons for control input
- LCD display (16x2 or compatible)
# Software Dependencies
- Timer.h: A custom timer library for task scheduling, ensuring precise control over song playback and UI updates.
- pitches.h: A header file containing definitions for musical note frequencies, enabling melody composition.
- LiquidCrystal.h: (Assumed) A library for controlling LCD displays, used for outputting song names and statuses.
# Setup and Installation
Connect the Hardware:

- Attach the buzzer to pin 8 (or as defined in your code).
- Connect the joystick/button to pin 10 for control input.
- Wire the LCD according to the pin definitions in the code (rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2).
# Software Preparation:

- Ensure Timer.h, pitches.h, and the LiquidCrystal library are included in your Arduino IDE environment.
- Upload the soundbooth.ino sketch to your Arduino board.
# Operation Instructions
- Selecting Songs: Use the joystick/button to navigate through the song list. The current selection is displayed on the LCD.
- Playing/Pausing: A specific action (e.g., button press) plays or pauses the currently selected song. Playback status is reflected on the LCD.


This README provides a comprehensive overview of the SoundBooth program, including its features, hardware and software requirements, and how to get started with the project. Adjustments might be necessary based on the actual hardware setup and specific functionalities of the LiquidCrystal.h library. ​
