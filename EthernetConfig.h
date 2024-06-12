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

#ifndef ETHERNET_CONFIG_H
#define ETHERNET_CONFIG_H

#include <Ethernet.h>
#include <EthernetUdp.h>

// USER SETTINGS 
extern byte mac[];
extern String DeviceName;
// USER END 

extern EthernetUDP Udp;

extern IPAddress default_ip;
extern IPAddress default_myDns;
extern IPAddress broadcast_ip;

extern IPAddress device_ip;
extern bool isGetDeviceIp;

extern unsigned int localPort;
extern char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

void initializeEthernet();
void processUdpPacket();

#endif
