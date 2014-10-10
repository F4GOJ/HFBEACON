# Arduino HF beacon library for AD9850 #
F4GOJ Christophe f4goj@free.fr<br>
F4GOH Anthony f4goh@orange.fr

October 2014

http://hamprojects.wordpress.com/2014/09/26/balise-multimodes-hfvhf/

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
## Hardware connections : ##

![ad9850](https://raw.githubusercontent.com/F4GOJ/AD9850SPI/master/images/AD9850.png)

- W_CLK -> D13 arduino UNO/NANO, D52 MEGA
- FQ_UD -> any pin except 10 and 12 UNO/NANO, 50 and 53 MEGA
- DATA/D7 -> D11 arduino UNO/NANO, D51 MEGA
- RESET -> any pin except 10 and 12 UNO/NANO, 50 and 53 MEGA

## Functions : ##

###cwTx(long freqCw, char * stringCw, int cwWpm);
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
###bpsk31Tx(long freqBpsk31, char * stringBpsk31);
#####Description
Transmit a string in BPSK 31 mode.
#####Syntax
`Beacon.bpsk31Tx(freqBpsk31, stringBpsk31);`
#####Parameters
**freqBpsk31 :** Frequency of transmission. *(long)*<br>
**stringBpsk31 :** String to transmit (not case sensitive) *(char array)*
#####Returns
None.
#####Example
```c++
void loop(){
 Beacon.bpsk31Tx(14070000, "f4goj 20m beacon test f4goj");
 while(1);
}
```
###qpsk31Tx(long freqQpsk31, char * stringQpsk31);
#####Description
Transmit a string in QPSK 31 mode.
#####Syntax
`Beacon.qpsk31Tx(freqQpsk31, stringQpsk31);`
#####Parameters
**freqQpsk31 :** Frequency of transmission. *(long)*<br>
**stringQpsk31 :** String to transmit (not case sensitive) *(char array)*
#####Returns
None.
#####Example
```c++
void loop(){
 Beacon.qpsk31Tx(14070000, "f4goj 20m beacon test f4goj");
 while(1);
}
```
###bpsk63Tx(long freqBpsk63, char * stringBpsk63);
#####Description
Transmit a string in BPSK 63 mode.
#####Syntax
`Beacon.bpsk63Tx(freqBpsk63, stringBpsk63);`
#####Parameters
**freqBpsk63 :** Frequency of transmission. *(long)*<br>
**stringBpsk63 :** String to transmit (not case sensitive) *(char array)*
#####Returns
None.
#####Example
```c++
void loop(){
 Beacon.bpsk63Tx(14070000, "f4goj 20m beacon test f4goj");
 while(1);
}
```
###qpsk63Tx(long freqQpsk63, char * stringQpsk63);
#####Description
Transmit a string in QPSK 63 mode.
#####Syntax
`Beacon.qpsk63Tx(freqQpsk63, stringQpsk63);`
#####Parameters
**freqQpsk63 :** Frequency of transmission. *(long)*<br>
**stringQpsk63 :** String to transmit (not case sensitive) *(char array)*
#####Returns
None.
#####Example
```c++
void loop(){
 Beacon.qpsk63Tx(14070000, "f4goj 20m beacon test f4goj");
 while(1);
}
```
###bpsk125Tx(long freqBpsk125, char * stringBpsk125);
#####Description
Transmit a string in BPSK 125 mode.
#####Syntax
`Beacon.bpsk125Tx(freqBpsk125, stringBpsk125);`
#####Parameters
**freqBpsk125 :** Frequency of transmission. *(long)*<br>
**stringBpsk125 :** String to transmit (not case sensitive) *(char array)*
#####Returns
None.
#####Example
```c++
void loop(){
 Beacon.bpsk125Tx(14070000, "f4goj 20m beacon test f4goj");
 while(1);
}
```
###qpsk125Tx(long freqQpsk125, char * stringQpsk125);
#####Description
Transmit a string in QPSK 125 mode.
#####Syntax
`Beacon.qpsk125Tx(freqQpsk125, stringQpsk125);`
#####Parameters
**freqQpsk125 :** Frequency of transmission. *(long)*<br>
**stringQpsk125 :** String to transmit (not case sensitive) *(char array)*
#####Returns
None.
#####Example
```c++
void loop(){
 Beacon.qpsk125Tx(14070000, "f4goj 20m beacon test f4goj");
 while(1);
}
```
###rttyTx(long freqRtty, char * stringRtty);
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
###wsprTx(long freqWspr, char * callWsprTx, char * locWsprTx, char * powWsprTx);
#####Description
Encode the callsign, the locator, the transmit power and transmit them in WSPR-2.
#####Syntaxe
`Beacon.wsprTx(freqWspr, callWsprTx, locWsprTx, powWsprTx);`
#####Paramètres
**freqWspr :** Frequency of transmission. *(long)*<br>
**callWsprTx :** Callsign (not case sensitive) *(char array)*<br>
**locWsprTx :** Locator (4 characters long, not case sensitive) *(char array)*<br>
**powWsprTx :** Transmit power (in dBm) *(char array)*
#####Retourne
Rien.
#####Exemple
```c++
void loop(){
 Beacon.wsprTx(10140131, "f4goj", "in97", "33");
 while(1);
```
