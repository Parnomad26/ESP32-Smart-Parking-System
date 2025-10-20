# 🚗 Smart Parking System (ESP32 & Web Server)

This project demonstrates a fully functional Smart Parking System using an ESP32 microcontroller, featuring real-time parking slot availability, vehicle counting, and a local Wi-Fi web dashboard.

## ✨ Features
* **Real-time Slot Monitoring:** Uses Ultrasonic sensors (HC-SR04) to check if a parking spot is **Available (🟢)** or **Occupied (🔴)**.
* **Vehicle Counting:** Uses IR sensors at the gate to track the total number of vehicles entering and exiting the facility.
* **Web Dashboard:** Hosts a simple, responsive web page on the ESP32's local IP address, updated automatically every 3 seconds.
* **LED Indicators:** Physical Red/Green LEDs provide immediate visual feedback for each parking slot.

## ⚙️ Hardware Components
| Component | Quantity | Purpose |
| :--- | :--- | :--- |
| ESP32 Dev Board | 1 | Main Microcontroller & Web Host |
| HC-SR04 Ultrasonic Sensor | 2 | Slot Occupancy Detection |
| IR Proximity Sensor | 2 | Gate Entry/Exit Counting |
| Red/Green LEDs | 4 (2 pairs) | Slot Status Indicators |
| Resistors (220 $\Omega$) | 4 | LED Current Limiting |

## 📌 Wiring Diagram (Pinout)
| Component | ESP32 Pin |
| :--- | :--- |
| **Ultrasonic 1** (TRIG/ECHO) | 13 / 12 |
| **Ultrasonic 2** (TRIG/ECHO) | 14 / 27 |
| **IR Sensor (Entry)** | 33 (INPUT_PULLUP) |
| **IR Sensor (Exit)** | 32 (INPUT_PULLUP) |
| **LED Green 1** | 18 |
| **LED Red 1** | 19 |
| **LED Green 2** | 21 |
| **LED Red 2** | 22 |
*Note: All sensors and the ESP32 should share a common GND and be powered by 5V (or the ESP32's Vin).*

## 🚀 Setup and Installation

1.  **Arduino IDE Setup:**
    * Install the **ESP32 board package** in your Arduino IDE (File > Preferences > Additional Boards Manager URLs).
    * Install the required libraries: `WiFi.h` and `WebServer.h` (usually pre-installed with the ESP32 board package).
2.  **Configure Code:**
    * Open the provided `.ino` file.
    * **Crucially, update your Wi-Fi credentials** in the code:
        ```cpp
        const char* ssid = "YourWiFiName";
        const char* password = "YourWiFiPassword";
        ```
    * Adjust the `slotDistance` constant if your prototype size requires it (e.g., if cars are larger/smaller).
3.  **Upload:** Select the correct ESP32 board and COM Port, then upload the code.
4.  **Access Web Server:**
    * Open the Serial Monitor (115200 baud).
    * Wait for the "WiFi Connected!" message. The ESP32 will print its **local IP address** (e.g., `192.168.1.100`).
    * Enter this IP address into any browser on the same Wi-Fi network to view the Smart Parking Dashboard.
