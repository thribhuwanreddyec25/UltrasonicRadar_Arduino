# UltrasonicRadar_Arduino

This project implements a complete radar-style scanning system using an **Arduino**, **HC-SR04 ultrasonic sensor**, **servo motor**, and a **Processing radar interface**.
The radar sweeps across a **150° arc**, measures distance at every degree, and displays objects in real time on a radar screen.

If an object gets **closer than 10 cm**, the radar **pauses**, and a **buzzer + red LED warning** activates.

---

## Index
* [What it does / Features](https://github.com/thribhuwanreddyec25/UltrasonicRadar_Arduino/tree/main?tab=readme-ov-file#what-it-does--features)
---

## What it does / Features

* 150° scanning sweep (15° → 165° → 15°)
* Real-time ultrasonic distance measurement
* Clean radar UI using Processing
* Animated sweep line + object plotting
* Auto-stop when object is very close (<10 cm)
* Red/Green LED indication
* Buzzer alarm
* Smooth serial communication (angle,distance.)

---

## Hardware Required

| Component                 | Quantity |
| ------------------------- | -------- |
| Arduino Uno/Nano          | 1        |
| HC-SR04 Ultrasonic Sensor | 1        |
| SG90/MG90 Servo Motor     | 1        |
| Breadboard                | 1        |
| Red LED                   | 1        |
| Green LED                 | 1        |
| Buzzer                    | 1        |
| 220Ω Resistors            | 2        |
| Jumper Wires              | —        |

---

## Circuit Wiring

### HC-SR04 Sensor

| HC-SR04 Pin | Arduino Pin |
| ----------- | ----------- |
| VCC         | 5V          |
| GND         | GND         |
| TRIG        | D8          |
| ECHO        | D9          |

### Servo Motor

| Servo Pin | Arduino Pin |
| --------- | ----------- |
| Signal    | D10         |
| VCC       | 5V          |
| GND       | GND         |

### LEDs & Buzzer

| Component | Arduino Pin |
| --------- | ----------- |
| Red LED   | D7          |
| Green LED | D6          |
| Buzzer    | D11         |

---

## How the System Works

### 1. Servo Radar Sweep

The servo gradually rotates between **15° and 165°**.
At each step, it measures the distance using the ultrasonic sensor.


### 2. Data Format Sent to Processing

For each angle, Arduino sends:

angle,distance.

Example:

45,22.

Meaning:
Angle = 45°
Distance = 22 cm

Processing reads this and updates the radar.


### 3. Processing Radar UI

Processing animates:

* Radar rings (distance zones)
* Sweeping green line (servo angle)
* Object markers in red
* Text labels (angle + distance)
* Status (In Range / Out of Range)


### 4. Object Detection Alert

If distance < **10 cm**:

* Servo **freezes**
* Buzzer turns ON
* Red LED turns ON
* Green LED turns OFF
* Radar still displays object
* System resumes when object moves away

---

## How to Run

### Step 1 — Upload Arduino Code

Open the  [Radar_Arduino.ino](https://github.com/thribhuwanreddyec25/UltrasonicRadar_Arduino/blob/main/Radar_Arduino.ino)  file in Arduino IDE and upload it.


### Step 2 — Open Processing Sketch

You can download it here <https://processing.org/download>

Open  [Radar_UI.pde](https://github.com/thribhuwanreddyec25/UltrasonicRadar_Arduino/blob/main/Radar_UI.pde)  in Processing.


Update the COM port:

myPort = new Serial(this, "COM3", 9600);

(Use whatever COM your Arduino shows.)

### Step 3 — Run Processing

Press **Run** in Processing.
A radar window will open.

### Step 4 — Test

Move your hand in front of the sensor.
If you go < 10 cm near it, the system alerts.

---

## Probable problems

### Processing window blank?

* Arduino Serial Monitor **must be closed**
* Check COM port
* Re-open Processing after uploading Arduino code

### Servo jittery or stuck?

* Power insufficient → Use external 5V supply with shared GND
* Object might be < 10 cm (this is expected behavior)

### Distance always 0?

* TRIG/ECHO wiring reversed
* Loose ground connection
