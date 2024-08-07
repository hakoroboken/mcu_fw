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

#include <Arduino.h>
#include "EthernetConfig.h"
#include "TimerConfig.h"
#include "IOConfig.h"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(DEBUG_BUTTON_PIN, INPUT_PULLUP);
  pinMode(FOR_F303K8_CW_CCW, OUTPUT);

  Serial.begin(9600);
  Serial1.begin(115200);

  initializeEthernet();
  setupTimer();
  
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  processUdpPacket();
  processTimer();

  if (digitalRead(4) == LOW) {
    Serial1.write((uint8_t)50);
    delay(50);
  }
}
