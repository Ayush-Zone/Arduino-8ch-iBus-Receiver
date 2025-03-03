#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define LED_PIN 8  // LED to indicate connection

const uint64_t pipeIn = 0xE9E8F0F0E1LL;
RF24 radio(7, 10);

struct Signal {
  byte throttle;
  byte yaw;
  byte pitch;
  byte roll;
  byte aux1;
  byte aux2;
  byte aux3;
  byte aux4;
};

Signal data;
unsigned long lastRecvTime = 0;
bool signalLost = true;  // Track signal status

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.startListening();
  radio.openReadingPipe(1, pipeIn);
  radio.setAutoAck(false);
  radio.setCRCLength(RF24_CRC_8);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(108);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Default: Disconnected
}

void recvData() {
  if (radio.available()) {
    radio.read(&data, sizeof(Signal));
    lastRecvTime = millis();
    signalLost = false;
    digitalWrite(LED_PIN, HIGH);  // LED ON (Connected)
  }
}

void sendIBUS() {
  if (signalLost) return;  // Stop sending packets when out of range

  uint8_t buffer[32] = { 0x20, 0x40 };  // iBUS Header

  uint16_t channels[8] = {
    map(data.roll, 0, 255, 1000, 2000),
    map(data.pitch, 0, 255, 1000, 2000),
    map(data.throttle, 0, 255, 1000, 2000),
    map(data.yaw, 0, 255, 1000, 2000),
    map(data.aux1, 0, 1, 1000, 2000),
    map(data.aux2, 0, 1, 1000, 2000),
    map(data.aux3, 0, 1, 1000, 2000),
    map(data.aux4, 0, 1, 1000, 2000)
  };

  for (int i = 0; i < 8; i++) {
    buffer[2 + (i * 2)] = channels[i] & 0xFF;
    buffer[3 + (i * 2)] = (channels[i] >> 8) & 0xFF;
  }

  uint16_t crc = iBUS_CRC(buffer, 30);
  buffer[30] = crc & 0xFF;
  buffer[31] = (crc >> 8) & 0xFF;

  Serial.write(buffer, 32);
}

uint16_t iBUS_CRC(uint8_t *buffer, uint8_t length) {
  uint16_t crc = 0xFFFF;
  for (uint8_t i = 0; i < length; i++) {
    crc -= buffer[i];
  }
  return crc;
}

void loop() {
  recvData();
  unsigned long now = millis();

  if (now - lastRecvTime > 1000) {  // No signal for 1 second
    signalLost = true;              // Stop sending packets
    digitalWrite(LED_PIN, LOW);     // LED OFF (Disconnected)
  }

  sendIBUS();
  delay(7);  // iBUS updates every ~7ms
}
