/*
 * xPL.Arduino v0.1, xPL Implementation for Arduino
 *
 * This code is parsing a xPL message stored in 'received' buffer
 * - isolate and store in 'line' buffer each part of the message -> detection of EOL character (DEC 10)
 * - analyse 'line', function of its number and store information in xpl_header memory
 * - check for each step if the message respect xPL protocol
 * - parse each command line
 *
 * Copyright (C) 2012 johan@pirlouit.ch, olivier.lebrun@gmail.com
 * Original version by Gromain59@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <xPL.h>
#include <EtherCard.h>

byte Ethernet::buffer[XPL_MESSAGE_BUFFER_MAX];
xPL xpl;

// MAC & IP init
uint8_t mymac[6] = { 0x54,0x55,0x58,0x10,0x00,0x11 };
uint8_t myip[4] = { 192,168,4,133 };
uint8_t gwip[4] = { 192,168,4,4};

// Ethercard only sends broadcast on this addr
uint8_t broadcast[4] = { 255,255,255,255};

// Input status
int A2current = -1 , A3current = -1, A4current = -1, A5current = -1;

void setup()
{
  Serial.begin(115200);
      
  ether.begin(sizeof Ethernet::buffer, mymac,10);  // non standard PIN for KMTronic board
  ether.staticSetup(myip, gwip);

  xpl.SendExternal = &SendUdPMessage;  // pointer to the send callback
  xpl.AfterParseAction = &AfterParseAction;  // pointer to a post parsing action callback 
  xpl.SetSource_P(PSTR("kmtronic"), PSTR("dino"), PSTR("boiler")); // parameters for hearbeat message

  pinMode(5, OUTPUT); digitalWrite(5, 0);
  pinMode(6, OUTPUT); digitalWrite(6, 0);
  pinMode(7, OUTPUT); digitalWrite(7, 0);
  pinMode(8, OUTPUT); digitalWrite(8, 0);
}

void loop()
{
   int reading;
   static bool enabled = false;
   
   if (millis() > 3000) enabled = true;
  
   xpl.Process();  // heartbeat management
  
   word len = ether.packetReceive();
   if( len > 0 ) 
   {
     // Check if Xpl UDP packet
     if( isXpl( Ethernet::buffer ) )
     {
       char xPLMessageBuff[XPL_MESSAGE_BUFFER_MAX];
       ByteToChar(Ethernet::buffer, xPLMessageBuff, len);
   
       // parse message
       xpl.ParseInputMessage(xPLMessageBuff);
       //Serial.println(xPLMessageBuff);
     }
   }
   
   if (enabled) {
     if ((reading = digitalRead(A5)) != A5current)
       SensorChange(reading, &A5current, "1");
     if ((reading = digitalRead(A4)) != A4current)
       SensorChange(reading, &A4current, "2");
     if ((reading = digitalRead(A3)) != A3current)
       SensorChange(reading, &A3current, "3");
     if ((reading = digitalRead(A2)) != A2current)
       SensorChange(reading, &A2current, "4");
   }
}

