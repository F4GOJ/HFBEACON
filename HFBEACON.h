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
   
   void rsidToggle(boolean rsidEnable);
   void cwTx(long freqCw, char * stringCw, int cwWpm);
   void bpsk31Tx(long freqBpsk31, char * stringBpsk31);
   void qpsk31Tx(long freqQpsk31, char * stringQpsk31);
   void bpsk63Tx(long freqBpsk63, char * stringBpsk63);
   void qpsk63Tx(long freqQpsk63, char * stringQpsk63);
   void bpsk125Tx(long freqBpsk125, char * stringBpsk125);
   void qpsk125Tx(long freqQpsk125, char * stringQpsk125);
   void rttyTx(long freqRtty, char * stringRtty);
   void hellTx(long freqHell, char * stringHell);
   void wsprTx(long freqWspr, char * callWsprTx, char * locWsprTx, char * powWsprTx);
   void wsprProcess(char * callWsprProc, char * locWsprProc, char * powWsprProc);
   byte Symbol[162];
   int wsprSymbGen;
			
  private:
   void rsidTx(long freqRsid, int modeRsid);
   void pskIdle(long freqIdle, int baudsIdle);
   void rttyTxByte (long freqRttyTxbyte, char c);
   void pskTx(long freqPsk, char * stringPsk, int modePsk, int baudsPsk);
   byte parity(unsigned long tempo);
   byte rsidTxEnable = 1;
};

extern HFBEACON Beacon;

#endif
