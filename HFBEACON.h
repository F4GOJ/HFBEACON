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
   void bpsk31Tx(long freqPsk, char * stringPsk);
   void qpsk31Tx(long freqPsk, char * stringPsk);
   void bpsk63Tx(long freqPsk, char * stringPsk);
   void qpsk63Tx(long freqPsk, char * stringPsk);
   void bpsk125Tx(long freqPsk, char * stringPsk);
   void qpsk125Tx(long freqPsk, char * stringPsk);
   void rttyTx(long freqRtty, char * stringRtty);
   void hellTx(long freqHell, char * stringHell);
   void wsprTx(long freqWspr, char * callWsprTx, char * locWsprTx, char * powWsprTx);
   void wsprProcess(char * callWsprProc, char * locWsprProc, char * powWsprProc);
   byte Symbol[163];
   int wsprSymbGen;
			
  private:
   void pskIdle(long freqIdle, int baudsIdle);
   void rttyTxByte (long freqRttyTxbyte, char c);
   void pskTx(long freqPsk, char * stringPsk, int modePsk, int baudsPsk);
   byte parity(unsigned long tempo);
};

extern HFBEACON Beacon;

#endif
