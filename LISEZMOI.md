# Librairie arduino balise multimodes HF#
F4GOJ Christophe f4goj@free.fr<br>
F4GOH Anthony f4goh@orange.fr

Octobre 2014

Utilisez cette librairie librement.

http://hamprojects.wordpress.com/2014/10/10/balise-multimodes-hfvhf-2/

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
###bpsk31Tx(long freqBpsk31, char * stringBpsk31)
#####Description
Transmet la chaine en BPSK 31.
#####Syntaxe
`Beacon.bpsk31Tx(freqBpsk31, stringBpsk31);`
#####Paramètres
**freqBpsk31 :** Frequence d'émission. *(long)*<br>
**stringBpsk31 :** Chaine à transmettre (n'est pas sensible à la casse) *(char array)*
#####Retourne
Rien.
#####Exemple
```c++
void loop(){
 Beacon.bpsk31Tx(14070000, "f4goj 20m beacon test f4goj");
 while(1);
}
```
###qpsk31Tx(long freqQpsk31, char * stringQpsk31)
#####Description
Transmet la chaine en QPSK 31.
#####Syntaxe
`Beacon.qpsk31Tx(freqQpsk31, stringQpsk31);`
#####Paramètres
**freqQpsk31 :** Frequence d'émission. *(long)*<br>
**stringQpsk31 :** Chaine à transmettre (n'est pas sensible à la casse) *(char array)*
#####Retourne
Rien.
#####Exemple
```c++
void loop(){
 Beacon.qpsk31Tx(14070000, "f4goj 20m beacon test f4goj");
 while(1);
}
```
###bpsk63Tx(long freqBpsk63, char * stringBpsk63)
#####Description
Transmet la chaine en BPSK 63.
#####Syntaxe
`Beacon.bpsk63Tx(freqBpsk63, stringBpsk63);`
#####Paramètres
**freqBpsk63 :** Frequence d'émission. *(long)*<br>
**stringBpsk63 :** Chaine à transmettre (n'est pas sensible à la casse) *(char array)*
#####Retourne
Rien.
#####Exemple
```c++
void loop(){
 Beacon.bpsk63Tx(14070000, "f4goj 20m beacon test f4goj");
 while(1);
}
```
###qpsk63Tx(long freqQpsk63, char * stringQpsk63)
#####Description
Transmet la chaine en QPSK 63.
#####Syntaxe
`Beacon.qpsk63Tx(freqQpsk63, stringQpsk63);`
#####Paramètres
**freqQpsk63 :** Frequence d'émission. *(long)*<br>
**stringQpsk63 :** Chaine à transmettre (n'est pas sensible à la casse) *(char array)*
#####Retourne
Rien.
#####Exemple
```c++
void loop(){
 Beacon.qpsk63Tx(14070000, "f4goj 20m beacon test f4goj");
 while(1);
}
```
###bpsk125Tx(long freqBpsk125, char * stringBpsk125)
#####Description
Transmet la chaine en BPSK 125.
#####Syntaxe
`Beacon.bpsk125Tx(freqBpsk125, stringBpsk125);`
#####Paramètres
**freqBpsk125 :** Frequence d'émission. *(long)*<br>
**stringBpsk125 :** Chaine à transmettre (n'est pas sensible à la casse) *(char array)*
#####Retourne
Rien.
#####Exemple
```c++
void loop(){
 Beacon.bpsk125Tx(14070000, "f4goj 20m beacon test f4goj");
 while(1);
}
```
###qpsk125Tx(long freqQpsk125, char * stringQpsk125)
#####Description
Transmet la chaine en QPSK 125.
#####Syntaxe
`Beacon.qpsk125Tx(freqQpsk125, stringQpsk125);`
#####Paramètres
**freqQpsk125 :** Frequence d'émission. *(long)*<br>
**stringQpsk125 :** Chaine à transmettre (n'est pas sensible à la casse) *(char array)*
#####Retourne
Rien.
#####Exemple
```c++
void loop(){
 Beacon.qpsk125Tx(14070000, "f4goj 20m beacon test f4goj");
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
Encode l'indicatif, le locator, la puissance d'émission et les transmet en WSPR-2.
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
