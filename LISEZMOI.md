# Librairie arduino balise multimodes HF#
F4GOJ Christophe f4goj@free.fr<br>
F4GOH Anthony f4goh@orange.fr

Octobre 2014

Utilisez cette librairie librement.

## Installation ##
Pour utiliser la librairie **HFBEACON** :
- Allez à https://github.com/F4GOJ/HFBEACON, cliquez le bouton [Download ZIP](https://github.com/F4GOJ/HFBEACON/archive/master.zip) et enregistrez le fichier ZIP à l'endroit de votre convenance.
- Décompressez le fichier. Vous obtiendrez un répertoire contenant tous les fichiers de la librairie avec un nom comprenant le nom de branche, typiquement **HFBEACON-master**.
- Renommez le répertoire en **HFBEACON**.
- Copiez le répertoire renommé dans le répertoire Arduino \libraries.

## Notes d'utilisation##

La librairie **HFBEACON** crée une instance de l'objet **Beacon**, l'utilisateur n'a pas pas besoin de le faire.

Pour utiliser la librairie **HFBEACON**, les librairies AD9850SPI et SPI doivent être incluses.

```c++
#include <AD9850SPI.h> //http://github.com/F4GOJ/HFBEACON
#include <AD9850SPI.h> // http://github.com/F4GOJ/AD9850SPI
#include <SPI.h>       //http://arduino.cc/en/Reference/SPI (incluse dans l'IDE Arduino)
```

## Fonctions : ##
###rsidToggle(boolean rsidEnable)
#####Description
Active/désactive le RSiD en modes PSK, RTTY et Hell.
#####Syntaxe
`Beacon.rsidToggle(rsidEnable);`
#####Paramètres
**rsidEnable :** true pour activer ou false pour désactiver l'envoi du RSiD (par défaut à true) *(boolean)*
#####Retourne
Rien.
#####Exemple
```c++
void setup(){
 Beacon.rsidToggle(false);
}
```
###cwTx(long freqCw, char * stringCw, int cwWpm)
#####Description
Transmet la chaine en code morse.
#####Syntaxe
`Beacon.cwTx(freqCw, stringCw, cwWpm);`
#####Paramètres
**freqCw :** Frequence d'émission. *(long)*<br>
**stringCw :** Chaine à transmettre (n'est pas sensible à la casse) *(char array)*<br>
**cwWpm :** Vitesse de transmission en mots par minute.  *(int)*
#####Retourne
Rien.
#####Exemple
```c++
void loop(){
 Beacon.cwTx(10141000, "f4goj 30m bcn test f4goj", 20);
 while(1);
}
```
###pskTx(long freq, char * stringPsk, int modePsk, int baudsPsk)
#####Description
Transmet la chaine en BPSK 31.
#####Syntaxe
`Beacon.pskTx(freq, stringPsk, modePsk, baudsPsk);`
#####Paramètres
**freq :** Frequence d'émission. *(long)*<br>
**stringPsk :** Chaine à transmettre (n'est pas sensible à la casse) *(char array)*<br>
**modePsk :** Mode (code ascii de B pour BPSK et Q pour QPSK) *(integer)*<br>
**baudsPsk :** Baud rate (31, 63, 125) *(integer)*
#####Retourne
Rien.
#####Exemple
```c++
void loop(){
 Beacon.pskTx(14070000, "f4goj 20m beacon test f4goj", 'B', 31);
 while(1);
}
```
###rttyTx(long freqRtty, char * stringRtty)
#####Description
Transmet la chaine en RTTY 45.
#####Syntaxe
`Beacon.rttyTx(freqRtty, stringRtty);`
#####Paramètres
**freqRtty :** Frequence d'émission. *(long)*<br>
**stringRtty :** Chaine à transmettre (n'est pas sensible à la casse) *(char array)*
#####Retourne
Rien.
#####Exemple
```c++
void loop(){
 Beacon.rttyTx(14070000, "f4goj 20m beacon test f4goj");
 while(1);
```
###wsprTx(long freqWspr, char * callWsprTx, char * locWsprTx, char * powWsprTx)
#####Description
Encode l'indicatif, le locator, la puissance d'émission si nécessaire et les transmet en WSPR-2.
#####Syntaxe
`Beacon.wsprTx(freqWspr, callWsprTx, locWsprTx, powWsprTx);`
#####Paramètres
**freqWspr :** Frequence d'émission. *(long)*<br>
**callWsprTx :** Indicatif (n'est pas sensible à la casse) *(char array)*<br>
**locWsprTx :** Locator (sur 4 caractères et n'est pas sensible à la casse) *(char array)*<br>
**powWsprTx :** Puissance d'émission (en dBm) *(char array)*
#####Retourne
Rien.
#####Exemple
```c++
void loop(){
 Beacon.wsprTx(10140131, "f4goj", "in97", "33");
 while(1);
```
###wsprEncode(char * callsign, char * locator, char * power)
#####Description
Encode l'indicatif, le locator, la puissance d'émission.
#####Syntaxe
`Beacon.wsprEncode(callsign, locator, power);`
#####Paramètres
**callsign :** Indicatif (n'est pas sensible à la casse) *(long)*<br>
**locator :** Locator (sur 4 caractères et n'est pas sensible à la casse) *(char array)*<br>
**power :** Puissance d'émission (en dBm) *(char array)*
#####Retourne
Mets à jour les variables wsprSymb[] and wsprSymbGen
#####Exemple
```c++
 Beacon.wsprEncode("f4goj", "in97", "33");
```
