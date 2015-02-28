# Arduino HF multimodes beacon library#
F4GOJ Christophe f4goj@free.fr<br>
F4GOH Anthony f4goh@orange.fr

October 2014

Use this library freely.

## Installation ##
To use the **HFBEACON** library:  
- Go to https://github.com/F4GOJ/HFBEACON, click the [Download ZIP](https://github.com/F4GOJ/HFBEACON/archive/master.zip) button and save the ZIP file to a convenient location on your PC.
- Uncompress the downloaded file.  This will result in a folder containing all the files for the library, that has a name that includes the branch name, usually **HFBEACON-master**.
- Rename the folder to  **HFBEACON**.
- Copy the renamed folder to the Arduino sketchbook\libraries folder.


## Usage notes ##

The **HFBEACON** library instantiates a **Beacon** object, the user does not need to do this.

To use the **HFBEACON** library, the AD9850SPI and SPI libraries must also be included.


```c++
#include <HFBEACON.h>  // http://github.com/F4GOJ/HFBEACON
#include <AD9850SPI.h> // http://github.com/F4GOJ/AD9850SPI
#include <SPI.h>       // http://arduino.cc/en/Reference/SPI (included with Arduino IDE)
```

## Functions : ##
###rsidToggle(boolean rsidEnable)
#####Description
Enable RSiD in PSK, RTTY and Hell modes.
#####Syntax
`Beacon.rsidToggle(rsidEnable);`
#####Parameters
**rsidEnable :** true to enable or false to disable RSiD transmission (default is true) *(boolean)*
#####Returns
None.
#####Example
```c++
void setup(){
 Beacon.rsidToggle(false);
}
```
###cwTx(long freqCw, char * stringCw, int cwWpm)
#####Description
Transmit a string in morse code.
#####Syntax
`Beacon.cwTx(freqCw, stringCw, cwWpm);`
#####Parameters
**freqCw :** Frequency of transmission. *(long)*<br>
**stringCw :** String to transmit (not case sensitive) *(char array)*<br>
**cwWpm :** Speed of transmission in words per minute.  *(int)*
#####Returns
None.
#####Example
```c++
void loop(){
 Beacon.cwTx(10141000, "f4goj 30m bcn test f4goj", 20);
 while(1);
}
```
###pskTx(long freq, char * stringPsk, int modePsk, int baudsPsk)
#####Description
Transmit a string in BPSK 31 mode.
#####Syntax
`Beacon.bpsk31Tx(freq, stringPsk, modePsk, baudsPsk);`
#####Parameters
**freq :** Frequency of transmission. *(long)*<br>
**stringPsk :** String to transmit (not case sensitive) *(char array)*<br>
**modePsk :** Mode (ascii code of B for BPSK and Q for QPSK) *(integer)*<br>
**baudsPsk :** Baud rate (31, 63, 125) *(integer)*
#####Returns
None.
#####Example
```c++
void loop(){
 Beacon.pskTx(14070000, "f4goj 20m beacon test f4goj", 'B', 31);
 while(1);
}
```

###rttyTx(long freqRtty, char * stringRtty)
#####Description
Transmit a string in RTTY 45 mode.
#####Syntax
`Beacon.rttyTx(freqRtty, stringRtty);`
#####Parameters
**freqRtty :** Frequency of transmission. *(long)*<br>
**stringRtty :** String to transmit (not case sensitive) *(char array)*
#####Returns
None.
#####Example
```c++
void loop(){
 Beacon.rttyTx(14070000, "f4goj 20m beacon test f4goj");
 while(1);
```
###wsprTx(long freqWspr, char * callWsprTx, char * locWsprTx, char * powWsprTx)
#####Description
Encode callsign, locator, transmit power if not done and transmit them in WSPR-2.
#####Syntax
`Beacon.wsprTx(freqWspr, callWsprTx, locWsprTx, powWsprTx);`
#####Parameters
**freqWspr :** Frequency of transmission. *(long)*<br>
**callWsprTx :** Callsign (not case sensitive) *(char array)*<br>
**locWsprTx :** Locator (4 characters long, not case sensitive) *(char array)*<br>
**powWsprTx :** Transmit power (in dBm) *(char array)*
#####Returns
None.
#####Example
```c++
void loop(){
 Beacon.wsprTx(10140131, "f4goj", "in97", "33");
 while(1);
```
###wsprEncode(char * callsign, char * locator, char * power)
#####Description
Encode callsign, locator, transmit power.
#####Syntax
`Beacon.wsprEncode(callsign, locator, power);`
#####Parameters
**callsign :** Callsign (not case sensitive) *(long)*<br>
**locator :** Locator (4 characters long, not case sensitive) *(char array)*<br>
**power :** Transmit power (in dBm) *(char array)*
#####Returns
Update wsprSymb[] and wsprSymbGen variables
#####Example
```c++
void loop(){
 Beacon.wsprEncode("f4goj", "in97", "33");
 while(1);
```
