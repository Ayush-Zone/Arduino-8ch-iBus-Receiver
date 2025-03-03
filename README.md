# Arduino-8ch-iBus-Receiver
# NRF24L01 iBUS Receiver

This project implements an **NRF24L01 iBUS Receiver** using an Arduino. It receives signals from a transmitter, processes them, and converts them into the **iBUS protocol** for communication with flight controllers.

## Features
- Uses **NRF24L01** to receive signals.
- Converts received signals to the **iBUS protocol**.
- LED status indication for connection status.
- Automatically stops sending packets when the signal is lost.

## Components Required
- **Arduino (Uno/Nano/Pro Mini)**
- **NRF24L01+ Module**
- **LED** (for connection status indication)
- **Jumper Wires**

## Circuit Diagram
### Wiring between **Arduino** and **NRF24L01+**
| Arduino | NRF24L01+ |
|---------|----------|
| 3.3V    | VCC      |
| GND     | GND      |
| 9       | CE       |
| 10      | CSN      |
| 11      | MOSI     |
| 12      | MISO     |
| 13      | SCK      |

### LED Connection
| Arduino | Component |
|---------|----------|
| 8       | LED (+)  |
| GND     | LED (-)  |

## Installation
1. **Connect the hardware** as per the circuit diagram.
2. **Install Required Libraries**:
   - [RF24 Library](https://github.com/nRF24/RF24)
3. **Upload the Code** to the Arduino.

## Code Explanation
- **recvData()**: Receives data packets from the NRF24L01 module.
- **sendIBUS()**: Converts received data to the iBUS protocol and sends it via Serial.
- **iBUS_CRC()**: Calculates the CRC checksum for iBUS data.
- **LED Indication**:
  - **ON**: Signal received successfully.
  - **OFF**: Signal lost for more than 1 second.

## Usage
1. Power on the transmitter and receiver.
2. The LED on the receiver will turn **ON** if a signal is detected.
3. The receiver will send the processed signal in **iBUS format** via Serial.
4. If the signal is lost, the LED will turn **OFF** and iBUS output will stop.

## License
This project is open-source under the **MIT License**. Feel free to modify and use it in your projects.

## Author
[(https://github.com/Ayush-Zone)] 
---
Feel free to contribute, report issues, or suggest improvements!
