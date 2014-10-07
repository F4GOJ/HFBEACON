/********************************************************************************************
 * HF multimodes beacon Arduino library
 * Created 04/10/2014
 * Christophe Caiveau f4goj@free.fr 
 * 
 * Use this library freely
 *
 * Instance :
 *
 * Functions :
 *
 *******************************************************************************************/
 
 
#ifndef HFBEACON_H
#define HFBEACON_H
#include <Arduino.h>

class HFBEACON
{
  public:
    HFBEACON();
        
   void sendRsid(long freqRsid, int modeRsid);
   void cwTx(long freqCw, char * stringCw, int cwWpm);
   void pskTx(long freqPsk, char * stringPsk, int modePsk, int baudsPsk);
   void rttyTx(long freqRtty, char * stringRtty);
   void hellTx(long freqHell, char * stringHell);
   void wsprTx(long freqWspr, char * callWsprTx, char * locWsprTx, char * powWsprTx);
   void wsprProcess(char * callWsprProc, char * locWsprProc, char * powWsprProc);
   byte Symbol[163];
   int wsprSymbGen;
			
  private:
   void pskIdle(long freqIdle, int baudsIdle);
   void rttyTxByte (long freqRttyTxbyte, char c);
   byte parity(unsigned long tempo);
   byte Symbol[163];
   int wsprSymbGen;
};

extern HFBEACON Beacon;

#endif
