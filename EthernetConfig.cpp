// Copyright 2024 Hakoroboken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "EthernetConfig.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x05 };
String DeviceName = "mcu-rm-05";

EthernetUDP Udp;

IPAddress default_ip(192, 168, 0, 100);
IPAddress default_myDns(192, 168, 0, 1);
IPAddress broadcast_ip(255, 255, 255, 255);

IPAddress device_ip;
bool isGetDeviceIp = false;

unsigned int localPort = 64202;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

void initializeEthernet() {
  Ethernet.init(17);
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found. Sorry, can't run without hardware. :(");
      while (true) { delay(1); }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    Ethernet.begin(mac, default_ip, default_myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  Udp.begin(localPort);
}

void processUdpPacket() {
  int packetSize = Udp.parsePacket();
  if (packetSize > 0) {
    memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);

    String data_str(packetBuffer);
    data_str.trim();

    if (data_str.indexOf("INFOREQ") != -1) {
      device_ip = Udp.remoteIP();
      isGetDeviceIp = true;
      return;
    }

    int power_int = data_str.toInt();
    if (data_str != "0" && power_int == 0) {
      Serial.println("Unknown data");
      return;
    }

    digitalWrite(3, power_int > 0 ? HIGH : LOW);

    uint8_t pwm_power = min(abs(power_int) * 2.55, 255.0);
    analogWrite(2, pwm_power);

    uint8_t rm_power = min(abs(power_int), 255);
    Serial1.write(rm_power);
  }

  if (Serial1.available() > 0 && isGetDeviceIp) {
    String motor_power_str = Serial1.readStringUntil('\n');
    motor_power_str.trim();
    Udp.beginPacket(device_ip, 64201);
    Udp.write(motor_power_str.c_str());
    Udp.endPacket();
  }
}
