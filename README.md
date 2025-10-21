# 🤖 Autonomous-Water-Cleaning-Robot

An **autonomous microcontroller-based robot** designed to collect floating trash from water surfaces.  
The system operates according to preset working hours, automatically starting and stopping based on the user's defined schedule, cleaning trash from water surfaces using a conveyor system, obstacle avoidance, and solar power based.


---

## 🧠 Overview

This project integrates mechanical, electronic, and embedded systems to create a **time-controlled water cleaning robot**.  
The robot navigates water surfaces, detects obstacles, and collects floating trash using a conveyor mechanism.  

---

## ⚙️ System Architecture

**Main Controller:** ESP8266 NodeMCU  
**Operation Mode:** Fully autonomous (time-triggered)  
**Control Logic:** RTC-based activation → Trash collection → Obstacle avoidance → Auto stop  

---

## 🔩 Components and Specifications

| Component | Specification | Function |
|------------|---------------|-----------|
| **ESP8266 NodeMCU** | 80 MHz CPU, 11 GPIO pins, Wi-Fi capable | Main controller unit for the system |
| **RTC DS3231** | ±2ppm accuracy, I2C interface | Real-time clock to control working time |
| **Ultrasonic Sensor (HC-SR04)** | 2–400 cm range, 15° angle | Detects obstacles for navigation |
| **Servo Motor (SG90)** | 0–180°, 5V DC | Rotates the ultrasonic sensor for scanning |
| **L298N Motor Driver** | Dual H-Bridge, up to 2A per channel | Drives propulsion DC motors |
| **DC Motors (x2)** | 5–12V, 300 RPM | Propulsion system for forward motion |
| **Conveyor Belt Motor** | 5–12V, geared | Drives the trash collection conveyor |
| **LCD 16x2 (I2C)** | 5V, SDA/SCL interface | Displays status and real-time data |
| **Keypad (4x4 I2C)** | Digital matrix input | User interface for setting time and parameters |
| **Buzzer** | 5V DC | Audio alert at end of operation |
| **Solar Panel** | 12V 5W | Charges battery through DC-DC converter |
| **Li-Po Battery** | 7.4V, 2200 mAh | Power supply for motors and controller |

---

## 🔌 Connection Summary

| Module | Connection to NodeMCU | Notes |
|---------|------------------------|-------|
| **RTC DS3231** | SDA → D2, SCL → D1 | I2C communication |
| **LCD (I2C)** | SDA → D2, SCL → D1 | Shared I2C bus with RTC |
| **Ultrasonic Sensor** | Trig → D5, Echo → D6 | Distance measurement |
| **Servo Motor** | Signal → D7 | Controls sensor rotation |
| **L298N Motor Driver** | IN1 → D3, IN2 → D4, IN3 → D8, IN4 → D0 | Motor control |
| **Conveyor Motor** | Through Relay → D9 | Controlled by relay output |
| **Buzzer** | → D10 | Alert output |
| **Keypad (I2C)** | SDA → D2, SCL → D1 | For time input |
| **Solar Panel** | → Buck Converter → Li-Po Battery | Power management |

---

## 🚀 How to Run the Project

Follow these steps to upload the code and run the project on your **ESP8266 NodeMCU**.

### Prerequisites
* Arduino IDE installed.
* ESP8266 board support package installed in the Arduino IDE.

### Installation and Setup

1.  Open the project file, `src/main_code.ino`, in the **Arduino IDE**.
2.  **Install Required Libraries:** All necessary libraries are listed below. For libraries like `Wire.h` and `Servo.h` which are usually built-in, no action is needed. For the custom libraries, you must install them via the Arduino Library Manager or by placing the files from the `libraries/` folder into your Arduino sketchbook's `libraries` folder:
    * `LiquidCrystal_I2C`
    * `RTClib`
    * `Keypad_I2C`
    * `Servo` (Included as it is necessary for control).
3.  In the Arduino IDE, go to **Tools** > **Board** > **ESP8266 Boards** and select **NodeMCU 1.0 (ESP-12E Module)** (or the correct NodeMCU board for your setup).
4.  **Upload the Code:** Click the **Upload** button in the Arduino IDE.
5.  **Set Operating Time:** Once uploaded, use the connected **keypad** to configure the desired operating hours for the cleaning robot.

---

## 📚 Libraries Used

| Library | Function |
| :--- | :--- |
| `Wire.h` | Handles **I²C communication** for the LCD and Keypad. |
| `LiquidCrystal_I2C.h` | Driver for the **I²C-enabled LCD display**. |
| `RTClib.h` | Interface library for the **Real-Time Clock (e.g., DS3231)**. |
| `Keypad_I2C.h` | Manages input from the **I²C Matrix Keypad**. |
| `Servo.h` | Control library for the **Servo Motor**. |

---

## 📜 License
This project is licensed under the MIT License for open-source use. It is intended for educational and research purposes.

© 2025 La Min Maung

---

## 👤 Author & Contributor
La Min Maung

Affiliation: King Mongkut’s Institute of Technology Ladkrabang (KMITL), Thailand

Expertise: Embedded & Mechatronics Enthusiast | PCB & Robotics Developer

---
