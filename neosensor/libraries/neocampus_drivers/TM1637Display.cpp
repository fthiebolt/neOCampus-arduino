//  Author: avishorp@gmail.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//  2017  - mostly rewritten for neOCampus


/*
 * Includes
 */
extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

#include <Arduino.h>
#include <Ticker.h>

#include "TM1637Display.h"


/*
 * Definitions & constants
 */

#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const uint8_t TM1637Display::digitToSegment[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
};

// erase display's content
const uint8_t TM1637Display::dispOFF[] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

// WiFi scan mode animation
//   __  __  __  __
//  |              |
//  |__  __  __  __|
//
const uint8_t TM1637Display::animMode1[] = { 0b00111001, 0b00001001, 0b00001001, 0b00001111, 0x00 };



/*
 * Constructor
 */
TM1637Display::TM1637Display( uint8_t pinClk, uint8_t pinDIO, uint16_t usBitDelay )
{
  // Copy the pin numbers
  m_pinClk = pinClk;
  m_pinDIO = pinDIO;
  // register bit delay (us)
  _usBitDelay = ( usBitDelay<TM1637_MIN_BITDELAY_US ? TM1637_MIN_BITDELAY_US : usBitDelay );

  // enable dots blinking as a default
  _enabledDotsBlinking = true;
  
  // disable animations
  _animMode = displayAnimate_t::stop;
  
  // Set the pin direction and default value.
  // Both pins are set as inputs, allowing the pull-up resistors to pull them up
  pinMode(m_pinClk, INPUT);
  pinMode(m_pinDIO,INPUT);
  digitalWrite(m_pinClk, LOW);
  digitalWrite(m_pinDIO, LOW);
  
   // powermode OFF as a default
  _powerON = false;
  setBrightness( 0, false );
  
  // currently displayed HOURS:MINUTES
  _curHour  = -1;
  _curMin   = -1;
}

// ---
// Brightness control
void TM1637Display::setBrightness(uint8_t brightness, bool on)
{
	m_brightness = (brightness & 0x7) | (on? 0x08 : 0x00);
}

void TM1637Display::setSegments(const uint8_t segments[], uint8_t length, uint8_t pos)
{
    // Write COMM1
	start();
	writeByte(TM1637_I2C_COMM1);
	stop();

	// Write COMM2 + first digit address
	start();
	writeByte(TM1637_I2C_COMM2 + (pos & 0x07));  // 0x07 instead of 0x03 because we use GRID5 for central dots and others stuffs

	// Write the data bytes
	for (uint8_t k=0; k < length; k++)
	  writeByte(segments[k]);

	stop();

	// Write COMM3 + brightness
	start();
	writeByte(TM1637_I2C_COMM3 + (m_brightness & 0x0f));
	stop();
}

void TM1637Display::showNumberDec(int num, bool leading_zero, uint8_t length, uint8_t pos)
{
  showNumberDecEx(num, 0, leading_zero, length, pos);
}

void TM1637Display::showNumberDecEx(int num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos)
{
    uint8_t digits[4];
	const static int divisors[] = { 1, 10, 100, 1000 };
	bool leading = true;

	for(int8_t k = 0; k < 4; k++) {
	    int divisor = divisors[4 - 1 - k];
		int d = num / divisor;
    uint8_t digit = 0;

		if (d == 0) {
		  if (leading_zero || !leading || (k == 3))
		      digit = encodeDigit(d);
	      else
		      digit = 0;
		}
		else {
			digit = encodeDigit(d);
			num -= d * divisor;
			leading = false;
		}
    
    // Add the decimal point/colon to the digit
    digit |= (dots & 0x80); 
    dots <<= 1;
    
    digits[k] = digit;
	}

	setSegments(digits + (4 - length), length, pos);
}


void TM1637Display::bitDelay()
{
	delayMicroseconds(_usBitDelay);
}

void TM1637Display::start()
{
  pinMode(m_pinDIO, OUTPUT);
  bitDelay();
}

void TM1637Display::stop()
{
	pinMode(m_pinDIO, OUTPUT);
	bitDelay();
	pinMode(m_pinClk, INPUT);
	bitDelay();
	pinMode(m_pinDIO, INPUT);
	bitDelay();
}

bool TM1637Display::writeByte(uint8_t b)
{
  uint8_t data = b;

  // 8 Data Bits
  for(uint8_t i = 0; i < 8; i++) {
    // CLK low
    pinMode(m_pinClk, OUTPUT);
    bitDelay();

	// Set data bit
    if (data & 0x01)
      pinMode(m_pinDIO, INPUT);
    else
      pinMode(m_pinDIO, OUTPUT);

    bitDelay();

	// CLK high
    pinMode(m_pinClk, INPUT);
    bitDelay();
    data = data >> 1;
  }

  // Wait for acknowledge
  // CLK to zero
  pinMode(m_pinClk, OUTPUT);
  pinMode(m_pinDIO, INPUT);
  bitDelay();

  // CLK to high
  pinMode(m_pinClk, INPUT);
  bitDelay();
  uint8_t ack = digitalRead(m_pinDIO);
  if (ack == 0)
    pinMode(m_pinDIO, OUTPUT);


  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  return ack;
}

uint8_t TM1637Display::encodeDigit(uint8_t digit)
{
	return digitToSegment[digit & 0x0f];
}



/* ------------------------------------------------------------------
 * Driver display overloads
 */

// ---
// PowerON display
void TM1637Display::powerON( void ) {
  // if( _powerON ) return; // [jun.18] to ease integration with animations modes

  setBrightness( TM1637_MAX_BRIGHTNESS, true );
  
  if( _enabledDotsBlinking and _animMode==displayAnimate_t::stop ) {
    _dots = false;
    _blinkTimer.detach();   // in case something already active (like an animation)
    _blinkTimer.attach( 0.5, timerHandler, this );
  }
  
  _powerON = true;
}

// ---
// PowerOFF display
void TM1637Display::powerOFF( void ) {
  // if( !_powerON ) return;  // [jun.18] to enable to clear the segments at startup
  
  setBrightness( 0, false );
  setSegments( dispOFF, sizeof(dispOFF) );
  
  _blinkTimer.detach();
  
  _powerON = false;
}

// ---
// Set brightness in percent 0-->100%
uint8_t TM1637Display::setPercentBrightness( uint8_t percent ) {
  // translate percent to range 0 to TM1637_MAX_BRIGHTNESS
  uint8_t val = _computePercentBrightness( percent );
  
  setBrightness( val, true );
  return val;
}

// ---
// toggle central dots blinking ON/OFF
bool TM1637Display::setDotsBlinking( bool val ) {
  _enabledDotsBlinking = val;
  
  // TODO;
  // launch timer if active
  // otherwise stop it!
  
  return _enabledDotsBlinking;
}

// ---
// Display a text message (i.e without 'dots')
uint8_t TM1637Display::dispMsg( const char *msg ) {

  // TODO
  
  return 0;
}

// ---
// Display time
//  return number ob bytes written
uint8_t TM1637Display::dispTime( uint8_t hours, uint8_t minutes, uint8_t seconds ) {

  int val = 0;

  if( hours != _curHour ) {
    
    _curHour  = hours;
    _curMin   = minutes;
    val = (hours * 100) + minutes;

    showNumberDec( val );
    return 4;
  }
  else if( minutes != _curMin ) {
    
    _curMin   = minutes;
    val = minutes;
    
    showNumberDec( val, true, 2, 2 );
    return 2;
  }

  return 0;
}


// ---
/* Send back identity
uint8_t TM1637Display::subID( void ) {
  return -1;
}
*/


// ---
// display some animations
// - wifi scanning
// - demo etc
bool TM1637Display::animate( displayAnimate_t mode ) {
  
  if( mode!=displayAnimate_t::stop ) {
    // start animation mode
    _animMode = mode;
    
    powerON();
    
    _blinkTimer.detach();
    _blinkTimer.attach( 0.5, timerHandler, this );
    //_blinkTimer.attach_ms( 250, timerHandler, this );
  }
  else {
    // stop animation mode
    _blinkTimer.detach();
    _animMode = displayAnimate_t::stop;
    
    powerOFF();    
  }

  return true;
}



/* ------------------------------------------------------------------
 * Protected'n Private methods
 */

/*
 * process current animation
 * [jun.18] only one animation available --> WiFi connect
 */
uint8_t TM1637Display::_processAnim( void ) {

  static bool toggle = false;
  static uint8_t curBrightness = TM1637_MAX_BRIGHTNESS;
  
  if( _animMode==displayAnimate_t::network_connect ) {
    
    setBrightness( TM1637_MAX_BRIGHTNESS, true );
    
    if( toggle )
      setSegments( animMode1, sizeof(animMode1) );
    else
      setSegments( dispOFF, sizeof(dispOFF) );
    
    // toggle brightness
    toggle = !toggle;
    
  }
  else if( _animMode==displayAnimate_t::network_connect_alt ) {
    
    static bool increaseBrightness = true;

    setBrightness( curBrightness, true );
    setSegments( animMode1, sizeof(animMode1) );
    
    // manage brightness evolution
    curBrightness = ( increaseBrightness ? curBrightness+1 : curBrightness-1 );
    setBrightness( curBrightness, true );
    
    // revert brightness direction
    if( curBrightness == TM1637_MAX_BRIGHTNESS ) increaseBrightness=false;
    else if( curBrightness==0 ) increaseBrightness=true;
  }
  
  return true;
}


/*
 * convert percent Brightness 0->100% to 0->7
 * Note: find a proper math way
 */
uint8_t TM1637Display::_computePercentBrightness( uint8_t percent ) {
  if( percent < 12 ) return 0;
  else if( percent <= 25 ) return 1;
  else if( percent < 37 ) return 2;
  else if( percent <= 50 ) return 3;
  else if( percent < 62 ) return 4;
  else if( percent <= 75 ) return 5;
  else if( percent < 87 ) return 6;
  else return 7;
}

/*
 * 0.5s timer handler to manage central dots blinking
 */
void IRAM_ATTR TM1637Display::timerHandler( TM1637Display *p ) {

  // animation mode is exclusive with normal mode (i.e central leds blinking)
  if( p->_animMode != displayAnimate_t::stop ) {
    p->_processAnim();
    return;
  }

  // dots blinking ...
  if( p->_enabledDotsBlinking ) {
    uint8_t _dot;
    if( p->_dots ) {
      _dot=SEG_DOT;
      p->setSegments( &_dot, 1, 4 ); // write 1 byte in pos 4 (central dots)
    }
    else {
      _dot='\0';
      p->setSegments( &_dot, 1, 4 ); // write 1 byte in pos 4 (central dots)
    }
    
    p->_dots = !p->_dots;
  }
}
