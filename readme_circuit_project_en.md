# Embedded Proximity Monitoring and Multimodal Alert System

An **Arduino Uno**-based prototype that detects whether a space is occupied by measuring distance with an ultrasonic sensor, and signals the result with lights and sound. It was designed, simulated in Tinkercad, and mounted on a **1:10 scale model** of a study cubicle at the Faculty of Engineering of Universidad de San Buenaventura, Bogotá campus.

Project for the Digital Circuits course.

![Tinkercad simulation](docs/simulacion_tinkercad.png)

## Problem Statement

In the study cubicles and laboratory areas of the Faculty of Engineering, space is oversaturated and poorly managed during peak hours: users have no signage telling them in real time whether a spot is available or occupied. Static posters and paper reservations do not adapt to the changing dynamics of the campus.

This system measures occupancy locally and continuously, and communicates it immediately with visual and acoustic indicators, at low cost and without relying on network infrastructure.

## How It Works

1. The **HC-SR04** sensor measures the distance to the nearest object or person.
2. If the distance is less than or equal to the threshold (**5 cm** on the scale model), the **red LED** turns on (occupied); otherwise, the **green LED** turns on (free).
3. Every time the state **changes** (enters or leaves the threshold), the **buzzer** sounds at 1 kHz. A **potentiometer** sets how long it sounds, from 0.3 to 3 s, and the buzzer is turned off without blocking the main loop (`millis()`).
4. A **pushbutton** mutes or re-enables the buzzer; the LEDs keep working.
5. In every cycle, a telemetry line is sent over **UART** (9600 bps) to the serial monitor.

## Components List

| Qty | Component | Function |
|:--:|---|---|
| 1 | Arduino Uno (ATmega328P) and USB cable | Microcontroller and power supply |
| 1 | HC-SR04 ultrasonic sensor | Distance measurement (time of flight) |
| 1 | Potentiometer | Alert duration adjustment (analog input) |
| 1 | Pushbutton | Mute / re-enable the buzzer |
| 1 | 10 kΩ resistor | External *pull-down* for the pushbutton |
| 1 | Green LED | Free space |
| 1 | Red LED | Occupied space |
| 2 | 220 Ω resistors | LED current limiting |
| 1 | Piezoelectric buzzer | Acoustic alert |
| 1 | Breadboard and wires | Connections |
| 1 | 1:10 scale model | Support for the sensor and LEDs |

## Wiring Diagram

![Schematic](docs/esquematico.png)

| Pin | Component | Type |
|:--:|---|---|
| A1 | HC-SR04 sensor (Trig pin) | Digital output (trigger) |
| A2 | HC-SR04 sensor (Echo pin) | Digital input (echo) |
| D2 | Green LED (with 220 Ω) | Digital output |
| D3 | Red LED (with 220 Ω) | Digital output |
| D4 | Piezoelectric buzzer | Output (`tone()`) |
| D5 | Pushbutton with 10 kΩ *pull-down* | Digital input |
| A0 | Potentiometer wiper | Analog input (10-bit ADC) |
| 5 V / GND | Power for the sensor and potentiometer; common ground | — |

The pushbutton connects input D5 to 5 V when pressed; the 10 kΩ resistor to ground keeps the input at a low level at rest. `INPUT_PULLUP` is not used.

## Usage Instructions

### Upload the Code

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Open `monitoreo_proximidad/monitoreo_proximidad.ino`.
3. Select the **Arduino Uno** board and the corresponding port, then click **Upload**.

### Test the System

1. Open the **Serial Monitor** at **9600 baud**. You will see lines like:

   ```
   Distance: 4 cm | Set Duration: 1.7 s | Buzzer: ENABLED -> RED LED
   ```

2. Turn the **potentiometer** to adjust the alert duration (`Set Duration`).
3. Bring an object within 5 cm of the sensor: the red LED turns on and the buzzer sounds once; when you move it away the green LED returns and the buzzer sounds again.
4. Press the **pushbutton** to mute the buzzer (`DISABLED`) and press it again to re-enable it (`ENABLED`).
5. If the sensor receives no echo, `Distance` is `999` and the LED stays green.

### Test in Simulation

The circuit can be simulated in Tinkercad Circuits. The simulation was made with an earlier firmware version: a PING))) sensor on D2, a threshold adjustable from 0 to 100 cm with the potentiometer, and a different pin assignment from the final build.

<!-- Add the Tinkercad project link here -->

## Repository Structure

```
.
├── README.md
├── monitoreo_proximidad/
│   └── monitoreo_proximidad.ino   # Commented source code
└── docs/
    ├── esquematico.png            # Wiring diagram
    └── simulacion_tinkercad.png   # Simulation screenshot
```

## Limitations and Improvements

- The threshold is fixed at 5 cm in the firmware, and the loop keeps a `delay(50)` per cycle. Making the threshold adjustable and replacing the remaining `delay()` with `millis()` is proposed.
- Sound-absorbing surfaces or surfaces with large incidence angles scatter the echo.
- The conversion to centimeters assumes a constant speed of sound (≈29 µs/cm per direction).
- The Arduino and breadboard sit outside the scale model box; housing them inside is proposed.
- Proposed improvements: temperature compensation, sensor array, OLED display, and sending data to the cloud (for example, with an ESP32).

## Authors

- Juan Sebastian Rios Socha
- Gustavo Pacheco
- Ivan Romero
- Santiago Cabrera

Universidad de San Buenaventura, Bogotá campus. Faculty of Engineering.
