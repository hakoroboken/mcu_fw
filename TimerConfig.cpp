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

#include "TimerConfig.h"
#include <EthernetUdp.h>
#include "EthernetConfig.h"

struct repeating_timer st_timer;
bool timer_flag = false;

bool Timer(struct repeating_timer *t) {
  timer_flag = true;
  return true;
}

void setupTimer() {
  add_repeating_timer_ms(1000, Timer, NULL, &st_timer);
}

void processTimer() {
  if (timer_flag) {
    timer_flag = false;
    Udp.beginPacket(broadcast_ip, 64203);
    String hello_str = Ethernet.localIP().toString() + "," + DeviceName;
    Udp.write(hello_str.c_str());
    Udp.endPacket();
  }
}
