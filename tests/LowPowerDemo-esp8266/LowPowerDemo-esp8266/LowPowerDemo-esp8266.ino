/* This example demonstrates the different low-power modes of the ESP8266

  The initial setup was a WeMos D1 Mini with 3.3V connected to the 3V3 pin through a meter
  so that it bypassed the on-board voltage regulator and USB chip.  There's still about
  0.3 mA worth of leakage amperage due to the unpowered chips.  These tests should work with
  any module, although on-board components will affect the actual current measurement.
  While the modem is turned on the amperage is > 67 mA or changing with a minimum value.
  To verify the 20 uA Deep Sleep amperage the voltage regulator and USB chip were removed.

  This test series requires an active WiFi connection to illustrate two tests.  If you
  have problems with WiFi, uncomment the #define DEBUG for additional WiFi error messages.
  The test requires a pushbutton switch connected between D3 and GND to advance the tests.
  You'll also need to connect D0/GPIO16 to RST for the Deep Sleep tests.  If you forget to
  connect D0 to RST it will hang after the first Deep Sleep test. D0 is driven high during
  Deep Sleep, so you should use a Schottky diode between D0 and RST if you want to use a
  reset switch; connect the anode of the diode to RST, and the cathode to D0.

  Additionally, you can connect an LED from any free pin through a 1K ohm resistor to the
  3.3V supply, though preferably not the 3V3 pin on the module or it adds to the measured
  amperage.  When the LED blinks you can proceed to the next test.  When the LED is lit
  continuously it's connecting WiFi, and when it's off the CPU is asleep.  The LED blinks
  slowly when the tests are complete.  Test progress can also be shown on the serial monitor.

  WiFi connections will be made over twice as fast if you can use a static IP address.

  This example is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This example is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this example; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA  */

#include <ESP8266WiFi.h>

void setup() {
}  // end of setup()

void loop() {

  WiFi.setAutoReconnect(true);
}
