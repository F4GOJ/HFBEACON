# Arduino HF beacon library for AD9850 #
F4GOJ Christophe f4goj@free.fr<br>
F4GOH Anthony f4goh@orange.fr

October 2014

Use this library freely.

Ce projet est basé sur les réflexions d'Anthony F4GOH :<br>
*Cela fait un moment que je m'intéresse au codage des modes numériques et j'ai cherché un moyen de les programmer facilement.<br>
J'ai trouvé très intéressant les travaux de M0XPD sur l'oscillateur programmable AD9850. Aussi, je me suis lancé dans l'aventure et décidé de faire mon propre montage en commençant par une balise HF.<br>
Un ATMEGA 328P d'un arduino a été extrait afin de réduire le circuit imprimé. Grâce à cette carte j'ai pu programmer les modes numériques suivants :<br>
BPSK, QPSK, RTTY, Hellschreiber et WSPR<br>
J'ai aussi ajouté le code RSiD et plus tard la SSTV (Martin M1)<br><br>
La balise est restée en fonction pendant une semaine 24h/24 sur les trois bandes 40, 30 et 20m en BPSK31 avec une puissance de 1W sur un dipôle.<br>
Le message indiquait de m'envoyer un report par mail et j'en ai reçu plus de 80. En regardant sur PSK reporter, il y a eu au total 250 reports avec une distance max de 7500km.<br>
Avec WSPR ça marche encore mieux évidemment...<br>
Aussi devant le succès du montage je me décidais à fabriquer un montage équivalent pouvant faire de la VHF.<br>
Je me suis donc inspiré de la balise de F1LVT utilisant un ICS511 (multiplicateur de fréquence) car l'AD9850 est limité à 40MHz.<br>
Le circuit imprimé VHF est quasiment identique à la version HF. J'ai commencé à générer facilement un bruit "won won" sur 121.375MHz (balise ADRASEC)<br>
Puis l'APRS me tenant à coeur, je me suis dit pourquoi ne pas créer une balise météo sur 144.800MHz avec des capteurs de température et pression I2C.<br>
Finalement cela marche très bien et F1PPH relaie mes trames F4GOH-11. Le montage fonctionnant sous 5V l'ajout d'une cellule photovoltaïque à permis d'éviter de changer les accus tous les 4 jours.<br>
Depuis, F4GOJ (Christophe) m'a rejoint dans l'aventure et nous avons décidé de faire évoluer l'ensemble en faisant un regroupement des deux montages HF et VHF, tout en réalisant un logiciel plus adapté.*<br>

## Installation ##
To use the **HFBEACON** library:  
- Go to https://github.com/F4GOJ/HFBEACON, click the [Download ZIP](https://github.com/F4GOJ/HFBEACON/archive/master.zip) button and save the ZIP file to a convenient location on your PC.
- Uncompress the downloaded file.  This will result in a folder containing all the files for the library, that has a name that includes the branch name, usually **HFBEACON-master**.
- Rename the folder to  **HFBEACON**.
- Copy the renamed folder to the Arduino sketchbook\libraries folder.


## Usage notes ##

The **HFBEACON** library instantiates a **Beacon** object, the user does not need to do this.

To use the **HFBEACON** library, the AD9850SPI and SPI library must also be included.


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
**freqBpsk31 :** Frequency of transmission *(long)*<br>
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
**freqQpsk31 :** Frequency of transmission *(long)*<br>
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
**freqBpsk63 :** Frequency of transmission *(long)*<br>
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
**freqQpsk63 :** Frequency of transmission *(long)*<br>
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
**freqBpsk125 :** Frequency of transmission *(long)*<br>
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
**freqQpsk125 :** Frequency of transmission *(long)*<br>
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
**freqRtty :** Frequency of transmission *(long)*<br>
**stringRtty :** String to transmit (not case sensitive) *(char array)*
#####Returns
None.
#####Example
```c++
void loop(){
 Beacon.rttyTx(14070000, "f4goj 20m beacon test f4goj");
 while(1);
```
