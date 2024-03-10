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

#include <Ethernet.h>
#include <EthernetUdp.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
EthernetUDP Udp;

IPAddress default_ip(192, 168, 0, 100);
IPAddress default_myDns(192, 168, 0, 1);

IPAddress device_ip;

unsigned int localPort = 64202;             // local port to listen on
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,

void setup() {
  // USB Serial Port Setup
  Serial.begin(9600);
  Serial1.begin(115200);

  // GPIO Setting
  pinMode(3, OUTPUT);  
  
  // Ethernet Setup
  Ethernet.init(17);
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
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

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize > 0) {

    // read the packet into packetBufffer
    for(auto i = 0; i < UDP_TX_PACKET_MAX_SIZE; i++){
      packetBuffer[i] = 0;
    }
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);

    String data_str(packetBuffer);
    Serial.println(packetBuffer);
    if(data_str.indexOf("INFOREQ") != -1){
      device_ip = Udp.remoteIP();
      Serial.print("INFOREQ get");
      return;
    }

    int power_int = data_str.toInt();

    if(power_int > 0){
      digitalWrite(3,HIGH);
    }else{
      digitalWrite(3,LOW);
    }

    uint pwm_power = abs(power_int);
    pwm_power = (int)((float)(pwm_power) * 2.55);
    if(pwm_power > 255){
      pwm_power = 255;
    }
    analogWrite(2,pwm_power);

    uint rm_power = abs(power_int);
    if(rm_power > 255){
      rm_power = 255;
    }
    Serial1.write((uint8_t)(rm_power));

  }

  if(Serial1.available() > 0){
    String motor_power_str = Serial1.readStringUntil(0x0a);
    motor_power_str.trim();
    // Serial.println(motor_power_str.toInt());

    Udp.beginPacket(device_ip, 64201);
    Udp.write(motor_power_str.c_str());
    Udp.endPacket();
    
  }
}
