/*
 *  Author: Jose Antonio Luceño Castilla
 *  Date  : Septempber 2013
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <SoftwareSerial.h>

#define RxD 10
#define TxD 11
#define RST 5
#define KEY 4

SoftwareSerial BTSerial(RxD, TxD);

void setup()
{
  
  pinMode(RST, OUTPUT);
  pinMode(KEY, OUTPUT);
  digitalWrite(RST, LOW);
  digitalWrite(KEY, HIGH);
  digitalWrite(RST, HIGH);
  
  delay(500);
  
  BTSerial.flush();
  delay(500);
  BTSerial.begin(38400);
  Serial.begin(9600);
  Serial.println("Enter AT commands:");

  BTSerial.print("AT\r\n");
  delay(100);

}

void loop()
{

  if (BTSerial.available())
    Serial.write(BTSerial.read());

  if (Serial.available())
    BTSerial.write(Serial.read());

}
