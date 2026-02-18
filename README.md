<img src="./doc_images/header.png" alt="" style="width: 100%;">

<div align="center">
  <img alt="Static Badge" src="https://img.shields.io/badge/React-5ED2F3">
  <img alt="Static Badge" src="https://img.shields.io/badge/Tailwind-CSS-3BBBF7">
  <img src="https://deploy-badge.vercel.app/vercel/deploy-badge"/>
</div>

# GLOFLEX

GLOFLEX is an assistive technology device designed to bridge communication barriers between deaf or mute individuals and the wider community. By converting predefined sign language gestures into audible speech, the system aims to reduce social isolation and promote inclusive communication.

## Installation

Clone the repository:

    git clone https://github.com/francogabrieloliveros/gloflex.git

make sure you have the [**Arduino Software**](https://www.arduino.cc/en/software/) to be able to run the program.

## Usage

### Materials

1. 5 Flex sensors
2. MPU6050 Triple Axis Accelerometer and Gyro Breakout
3. Mini MP3 Player Module
4. Speaker
5. Arduino Nano board
6. Breadboard
7. 1k Ohm Resistors
8. Jumper Wires

### Connections

Follow the schematic diagram below. After that, you can connect the Arduino Nano

<img src="./doc_images/Schematic.png" alt="" style="width: 100%;">

## Additional Notes

- Read our [reserach paper](https://drive.google.com/file/d/1E5YhzoAbpzOS5He5SWwR0dnKx_ctYTIx/view?usp=sharing) for more information.
- The threshold for each sensor may vary, feel free to modify the code.
- The folder for the audio for each letter is not in this repository, no audio will be played for each hand sign. Instead, you can try to print the letter in the console if to replace the playing of letter audios.
