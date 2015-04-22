#include <AD9850SPI.h> // https://github.com/F4GOJ/AD9850SPI/archive/master.zip
#include <HFBEACON.h> // https://github.com/F4GOJ/HFBEACON/archive/master.zip
#include <SPI.h>
#include <DS3232RTC.h> //http://github.com/JChristensen/DS3232RTC
#include <Time.h>
#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h> // https://github.com/jenschr/Arduino-libraries/tree/master/LiquidCrystal

#define atu_eeprom_base 400
#define atu_swr_min 30
#define consigneSWR 3
#define CRAdr 0x21
#define dds_on 7
#define EcartdBmPont 29.6
#define gain 6
#define kp 3
#define led_sync A1
#define LRAdr 0x20
#define onewire 4
#define sqw 2
#define vf A6
#define vr A7

typedef struct  {
  byte confDef;         // B
  byte modeBcn;         // 1:bpsk ou qpsk 2:hell 3:cw 4:rtty 5:wspr 
  byte seqBcn;          // 1:continue 2:every 3:hour 4:sweep  5:debug
  byte minBcn;          // minutes between transmissions for every and hour sequences
  byte hourBcn[3];      // Hour 
  byte pwm[3];          // pwm value (0 - 255) for each freqBcn
  byte power[3];        // index to dbm array (power in dbm)
  byte speedBcn;        // index to pskBaudRate array (PSK baud rate 31 - 63 - 125)
  byte wpm;             // CW words per minute
  byte rsid_toggle;     // 0 - 1
  byte swr_enable;      // 0 - 1
  byte lcd_enable;      // 0 - 1
  byte serial_enable;   // 0 - 1
  byte bandfreq1select;
  byte bandfreq2select;
  byte bandfreq3select;
  long trim;
  long freqBcn[3];
  char callsign[7];
  char locator[7];
  char email[41];
  char texte[258];
 }BeaconStruct;
 BeaconStruct beacon;

static time_t tLast;
time_t t;
tmElements_t tm;
  
byte atu_data[8][3];        //bandfreqselect , swr correct , valeur de L valeur de C+HZLZ
byte car0[8] = {B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00000}; // Special character for ATU bargraph
byte car1[8] = {B10000,B10000,B10000,B10000,B10000,B10000,B10000,B10000}; // Special character for ATU bargraph
byte car2[8] = {B11000,B11000,B11000,B11000,B11000,B11000,B11000,B11000}; // Special character for ATU bargraph
byte car3[8] = {B11100,B11100,B11100,B11100,B11100,B11100,B11100,B11100}; // Special character for ATU bargraph
byte car4[8] = {B11110,B11110,B11110,B11110,B11110,B11110,B11110,B11110}; // Special character for ATU bargraph
byte car5[8] = {B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111}; // Special character for ATU bargraph
byte cval=0;
byte Data;
byte hand_return;
byte hzlz=0;
byte lval=0;
byte pskBaudRate[] = {31, 63, 125};
byte relay[] = {5, A3, A2};
byte wpm[] = {5, 10, 15, 20, 25, 30};

const char* lcd_mode[] = {"", "PSK", "HELL     ", "CW", "RTTY     ", "WSPR     "};
const char* lcd_rsid[] = {"    ", "RSID"};
const char* lcd_seq[] = {"", "Cont. ", "Every ", "Hour  ", "Sweep ", "Debug "};
const char* lcd_tx[] = {"  ", "Tx"};
const char* serial_mode[] = {"", "PSK", "HELL", "CW", "RTTY", "WSPR"};
const char* serial_rsid[] = {"RSID disabled", "RSID enabled"};
const char* serial_seq[] = {"", "Continue", "Every ", "Hour", "Sweep", "Debug"};
const char* dbm[] = {"20", "23", "27", "30", "33", "37"};

int band = 0;
int calibre_ptr = 1;
int swr_result = 1;
int val_gain = 0;
int wsprTime;
int wsprTimeSec;

long calibre = 125000000L;
long debug_freq;
unsigned long previousMillis = 0;
unsigned long interval = 1000;
unsigned long currentMillis;
const unsigned long calibre_freq[]={1,10,100,1000,10000,100000};

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup(){
 
 Serial.begin(57600);
 DDS.begin(13,9,8);
 Beacon.ddsPower(0);
 pinMode(relay[0], OUTPUT);
 pinMode(relay[1], OUTPUT);
 pinMode(relay[2], OUTPUT);
 pinMode(led_sync,OUTPUT);
 digitalWrite(led_sync, LOW);
 digitalWrite(relay[0], LOW);
 digitalWrite(relay[1], LOW);  
 digitalWrite(relay[2], LOW);
 analogReference(EXTERNAL);
 analogWrite(gain, 0);

 send_gpio(LRAdr,0x00,0x00);  //init gpio L to output
 send_gpio(CRAdr,0x00,0x00);  //init gpio C to output

 send_gpio(LRAdr,0x09,0xff);  //all gpio L at level 0
 send_gpio(CRAdr,0x09,0xff);  //all gpio C at level 0

 init_beacon();
 
 DDS.calibrate(beacon.trim);
 
 if(beacon.lcd_enable == 1){
  lcd.begin(4, 20);
  lcd.setBacklight(HIGH);
  lcd.createChar(0, car0);
  lcd.createChar(1, car1);
  lcd.createChar(2, car2);
  lcd.createChar(3, car3);
  lcd.createChar(4, car4);
  lcd.createChar(5, car5);
 }
 
 Serial.write('A');
 hand_return = handshake(0);
 if (hand_return == 'V'){
  command();
 }
 
 if(beacon.lcd_enable == 1){
  lcd.clear();
  lcd.print(F("   HF BEACON v1.0"));
  lcd.setCursor(0, 2);
  lcd.print(F(" F4GOH  F4GOJ  2015"));
  delay(4000);
  lcd.clear();
 }

if(beacon.serial_enable == 1){
 Serial.println("");
 serial_print();
}

if(beacon.lcd_enable == 1){
  lcd.clear();
  lcd_print();
 }

if(beacon.seqBcn == 5){
 print_beacon();
}

while(beacon.seqBcn == 5){
 debug();
}

 if((beacon.swr_enable & 2) == 2){   // beacon.swr_enable bit 1 = 2 ATU enabled
  if(beacon.serial_enable == 1){
   Serial.println(F("Lecture eeprom :"));
  }
  for(int ptr=0; ptr < 8; ptr++){  //charger l'eeprom (toutes les bandes L et C seulement)
   atu_data[ptr][1] = EEPROM.read(ptr * 2 + atu_eeprom_base);
   atu_data[ptr][2] = EEPROM.read(ptr * 2 + atu_eeprom_base + 1);
   if(beacon.serial_enable == 1){
    Serial.print(F("ptr :"));
    Serial.print(ptr);
    Serial.print(F("L :"));
    Serial.print(atu_data[ptr][1]);
    Serial.print(F(", C :"));
    Serial.println(atu_data[ptr][2]);
   }
  }
  
  test_atu(beacon.bandfreq1select, 0); 
  if ((beacon.seqBcn > 1) && (beacon.seqBcn < 5)) {
   test_atu(beacon.bandfreq2select, 1);
   test_atu(beacon.bandfreq3select, 2);
  }
 }
} // End of setup()


void loop(){
 currentMillis = millis();
 if(currentMillis - previousMillis > 500){
  RTC.read(tm);
  wsprTimeSec = tm.Second;
  
  if(beacon.lcd_enable == 1){
   lcd_print();
  }

  switch (beacon.seqBcn)
  {
   case 1 :
   if (beacon.modeBcn == 5){
    wsprTime = tm.Minute & 0x01;
    if(wsprTime == 0 && wsprTimeSec == 0){
     transmit(0);
    }
   }
   else{
    transmit(0);
    delay(2000);
   }
   break;
   
   case 2 :
    wsprTime = tm.Minute % beacon.minBcn;
    if(wsprTime == 0 && wsprTimeSec == 0){
     transmit(0);
    }
   break;
   
   case 3 :
   if(beacon.hourBcn[0] > beacon.hourBcn[1]){
    if(tm.Hour >= beacon.hourBcn[0] || tm.Hour < beacon.hourBcn[1]){
      band = 0;
    }
   }
   else if(tm.Hour >= beacon.hourBcn[0] && tm.Hour < beacon.hourBcn[1]){
    band = 0;
   }
   
   if(beacon.hourBcn[1] > beacon.hourBcn[2]){
    if(tm.Hour >= beacon.hourBcn[1] || tm.Hour < beacon.hourBcn[2]){
     band = 1;
    }
   }
   else if(tm.Hour >= beacon.hourBcn[1] && tm.Hour < beacon.hourBcn[2]){
     band = 1;
   }
   
   if(beacon.hourBcn[2] > beacon.hourBcn[0]){
    if(tm.Hour >= beacon.hourBcn[2] || tm.Hour < beacon.hourBcn[0]){
     band = 2;
    }
   }
   else if(tm.Hour >= beacon.hourBcn[2] && tm.Hour < beacon.hourBcn[0]){
     band = 2;
   }
       
   wsprTime = tm.Minute % beacon.minBcn;
   if(wsprTime == 0 && wsprTimeSec == 0){
    transmit(band);
   }
   break;
   
   case 4 :
    if(beacon.modeBcn==5){
     wsprTime = tm.Minute & 0x01;
     if(wsprTime == 0 && wsprTimeSec == 0){
      transmit(band++);
     }
    }
    else{
     transmit(band++);
    }
    if(band == 3){
     band = 0;
    }
   break;
  }
  previousMillis = currentMillis;
 }
} // End of loop()

void test_atu(byte bandfreq,byte bandTransmit){
 Wire.beginTransmission(LRAdr);
 if (Wire.endTransmission =! 0){
  if(beacon.serial_enable == 1){
    Serial.println(F("ATU not found !"));
   }
   return;
 }
 else{
  Wire.beginTransmission(CRAdr);
  if (Wire.endTransmission =! 0){
   if(beacon.serial_enable == 1){
    Serial.println(F("ATU not found !"));
   }
   return;
  }
  else{
   if(beacon.serial_enable == 1){
    Serial.print(F("test atu, bandfreq :"));
    Serial.print(bandfreq);
    Serial.print(F(", bandTransmit :"));
    Serial.println(bandTransmit);
   }
 
  send_gpio(LRAdr, 0x09, ~atu_data[bandfreq][1]); 
  send_gpio(CRAdr, 0x09, ~atu_data[bandfreq][2]);  

  Beacon.ddsPower(1);
  digitalWrite(relay[bandTransmit], HIGH);
  DDS.setfreq(beacon.freqBcn[bandTransmit], 0);
  analogWrite(gain, 150);                        //
  delay(50);
  if (mesureSWR() < atu_swr_min) {
   atu_data[bandfreq][0] = 1; // si swr < 3 alors flag ok dans le tableau
   if(beacon.serial_enable == 1){
    Serial.println(F("table eeprom ok"));
   }
  }
  else{ //   sinon recherche 
   if(beacon.serial_enable == 1){
    Serial.println(F("recherche..."));
   }
   if(beacon.lcd_enable == 1){
    lcd.clear();
    lcd.print(F("Searching ATU "));
    lcd.print(beacon.freqBcn[bandTransmit]/1000000);
    lcd.print(F(" Mhz "));                 
    lcd.setCursor(0, 1);
    lcd.print(F("Coarse:"));
    lcd.setCursor(0, 2);
    lcd.print(F("Fine :"));
    lcd.setCursor(0, 3);
    lcd.print(F("Result:"));
   }
   if (search_atu() < atu_swr_min){
    atu_data[bandfreq][0] = 1; //  si swr < 3 memorise le resultat dans le tableau  et flag=1
    atu_data[bandfreq][1] = lval;
    atu_data[bandfreq][2] = cval | (hzlz * 128);
    EEPROM.write(atu_eeprom_base + bandfreq * 2, lval); // ecriture dans eeprom
    EEPROM.write(atu_eeprom_base + bandfreq * 2 + 1, cval | (hzlz * 128));
    if(beacon.serial_enable == 1){
     Serial.print(F("memorisation L :"));
     Serial.print(lval);
     Serial.print(F(", C :"));
     Serial.println(cval | (hzlz * 128));
    }
   }
   else{
    atu_data[bandfreq][0]=0;        //pas d'accord trouvé flag =0
    delay(2000);      //le temps de lire le resultat
   }
   analogWrite(gain, 0);    
   Beacon.ddsPower(0);
   digitalWrite(relay[bandTransmit], LOW);
  }
 }
} // End of test_atu()

 
void transmit(byte bandTransmit){
 int tempe_rtc = RTC.temperature();
 float celsius = tempe_rtc / 4.0;
 if(beacon.lcd_enable == 1){
  lcd.setCursor(18, 1);
  lcd.print(lcd_tx[1]);
  lcd.setCursor(0,3);
  lcd.print(celsius);
  lcd.write(223);
  lcd.print(F("C "));
 }
 // ici positionner le code pour l'atu depuis le tableau 
 // si   sinon tx impossible
 byte tx_enable = 1;      //flag autorisation tx par defaut à 1

 if((beacon.swr_enable & 2)  == 2){      //si atu lire tableau et maj de tx_enable
  switch (bandTransmit){
   case 0 :
    tx_enable = atu_data[beacon.bandfreq1select][0];
    lval = atu_data[beacon.bandfreq1select][1]; 
    cval = atu_data[beacon.bandfreq1select][2];  
   break;
   case 1 :
    tx_enable = atu_data[beacon.bandfreq2select][0];
    lval = atu_data[beacon.bandfreq2select][1]; 
    cval = atu_data[beacon.bandfreq2select][2];  
   break;
   case 2 :
    tx_enable = atu_data[beacon.bandfreq3select][0];
    lval = atu_data[beacon.bandfreq3select][1]; 
    cval = atu_data[beacon.bandfreq3select][2];  
   break;
  }
  send_gpio(LRAdr, 0x09, ~lval);             //cde des relais
  send_gpio(CRAdr, 0x09, ~cval);  
 }

 if (tx_enable == 1) {                      //test de tx enable
  if(beacon.serial_enable == 1){
   serial_print();
   Serial.println(F("\r\nTransmitting"));
 }
 Beacon.ddsPower(1);
 // Serial.print(band);            //ajout cde boite de couplage
 digitalWrite(relay[bandTransmit], HIGH);
 DDS.setfreq(beacon.freqBcn[bandTransmit], 0);
 if((beacon.swr_enable & 1) == 1){
  analogWrite(gain, 150);    //150 doit suffir avant 250 a revoir eventuellement
  delay(2000);     
  swr_result = mesureSWR();
  if(beacon.lcd_enable == 1){
   lcd.setCursor(14, 2);
   lcd.print(F("SWR"));
   if (swr_result >= 100){
    lcd.print(F("++!"));  
   }
   else{
    lcd.print(swr_result / 10);
    lcd.print(".");
    lcd.print(swr_result % 10);
   }
  }
  if(beacon.serial_enable == 1){
   Serial.print(F("SWR : "));
   if(swr_result >= 100){
    Serial.println(F("Infini"));
   }
   else{
    Serial.println((float)swr_result / 10); 
   } 
  }
  else{
   delay(2000);
  }
  analogWrite(gain, (int)beacon.pwm[bandTransmit]);
  modeBeacon(beacon.freqBcn[bandTransmit]);
  analogWrite(gain, 0);
  Beacon.ddsPower(0);
  digitalWrite(relay[bandTransmit], LOW);
  if(beacon.lcd_enable == 1){
   lcd.setCursor(18, 1);
   lcd.print(lcd_tx[0]);
  }
  if(beacon.serial_enable == 1){
   Serial.println(F("End of Transmission\r\n"));
  }
 }
 else{    //inserer ici le code lcd et serial dans le cas ou on ne peut pas transmettre
 }
} // End of transmit()


int mesureSWR(void){
 int fwd_num =  analogRead(vf);  // puissance directe numérique
 int rev_num =  analogRead(vr);  // puissance reflechie numérique

 //Serial.println("num---");
 //Serial.println(fwd_num);
 //Serial.println(rev_num);

 float fwd_dbm =   NtodBm(fwd_num)+EcartdBmPont;  // puissance directe dbm
 float rev_dbm =   NtodBm(rev_num)+EcartdBmPont;  // puissance reflechie dbm

 float Rho=0;
 float swr=0; 

 //Serial.println("dbm---");
 //Serial.println(fwd_dbm);
 //Serial.println(rev_dbm);
 if(rev_dbm < fwd_dbm){
  Rho = (rev_dbm - fwd_dbm) / 20;
  Rho = pow(10, Rho);
 //          Serial.print("rho ");
 //          Serial.println(Rho);
 swr = (1 + Rho)/(1 - Rho);
 //          Serial.print("swr ");
 //          Serial.println(swr);
 }
 else{
  swr = 10; // pas de charge ou pas d'antenne
 }

 if (swr > consigneSWR){
  digitalWrite(led_sync, HIGH);
 }
 else{
  digitalWrite(led_sync, LOW);     //consigneSWR en define hi !
 }
 return (int)(swr * 10);
} // End of mesureSWR()


float NtodBm(int N){
 return (100 * (float)N) / 1023 - 90;
} // End of NtodBm()


void modeBeacon(long frequence){
 char txLoc[] = "xxxx";
 char txPow[] = "00";
 String wsprloc = "    ";
 
 switch (beacon.modeBcn){
   case 1 :
    Beacon.pskTx(frequence, beacon.texte, beacon.confDef, pskBaudRate[beacon.speedBcn]);
   break;
   case 2 :
    Beacon.hellTx(frequence,beacon.texte);
   break;
   case 3 :
    Beacon.cwTx(frequence, beacon.texte, beacon.wpm);
   break;
   case 4 : 
    Beacon.rttyTx(frequence, beacon.texte);
   break;
   case 5 :
    for(int i = 0; i < 4; i++){
     txLoc[i] = beacon.locator[i];
    }
    strcpy(txPow, dbm[beacon.power[band]]);
    Beacon.wsprTx(frequence, beacon.callsign, txLoc, txPow);
   break;
 }
} // End of modeBeacon()

void lcd_print(){
 lcd.setCursor(0, 0);
 lcd.print(beacon.callsign);
 lcd.setCursor(7, 0);
 lcd.print(beacon.locator);
 lcd.setCursor(14, 0);
 lcd.print(lcd_seq[beacon.seqBcn]);
 if(beacon.seqBcn == 2 || beacon.seqBcn == 3){
  lcd.setCursor(0, 3);
  digitalClockDisplay(); 
  lcd.setCursor(10, 3);
  lcd.print(F("every "));
  lcd.print(beacon.minBcn);
  lcd.print(F("mn"));
 }
 else{
  lcd.setCursor(12, 3);
  digitalClockDisplay(); 
 }
 lcd.setCursor(0, 1);
 if(beacon.freqBcn[band] < 10000000){
  lcd.print(F(" "));
 }
 lcd.print(beacon.freqBcn[band]);
 lcd.setCursor(8, 1);
 lcd.print(F("Hz"));
 lcd.setCursor(12, 1);
 switch(beacon.modeBcn){
  case 1 : case 2 : case 4 :
   lcd.print(lcd_rsid[beacon.rsid_toggle]);
  break;
 }
 lcd.setCursor(0, 2);
 switch(beacon.modeBcn){
   case 1:
   if(beacon.confDef == 'B'){
    lcd.print(F("BPSK"));
   }
   else{
    lcd.print(F("QPSK"));
   }
  lcd.print(pskBaudRate[beacon.speedBcn]);
  lcd.print(F("  "));
  break;
  case 3:
   lcd.print(F("CW"));
   lcd.print(wpm[beacon.wpm]);
   lcd.print(F("wpm "));
  break;
  default:
  lcd.print(lcd_mode[beacon.modeBcn]);
 }
 lcd.setCursor(8, 2);
 lcd.print(dbm[beacon.power[band]]);
 lcd.print(F("dBm"));
} // End of lcd_print()


void serial_print(){
 Serial.print(F("Callsign : "));
 Serial.println(beacon.callsign);
 Serial.print(F("Locator : "));
 Serial.println(beacon.locator);
 Serial.print(F("Sequence : "));
 Serial.print(serial_seq[beacon.seqBcn]);
 switch(beacon.seqBcn){
  case 2 :
   Serial.print(beacon.minBcn);
   Serial.println(F(" minutes"));
  break;
  case 3 :
   Serial.print(beacon.minBcn);
   Serial.println(F(" every minutes"));
  break;
  default :
  Serial.println(F(""));
 }
 Serial.print(F("Frequency : "));
 Serial.print(beacon.freqBcn[band]);
 Serial.println(F("Hz"));
 switch(beacon.modeBcn){
  case 1 : case 2 : case 4 :
   Serial.println(serial_rsid[beacon.rsid_toggle]);
  break;
 }
 Serial.print(F("Mode : "));
 switch(beacon.modeBcn){
  case 1:
   if(beacon.confDef == 'B'){
    Serial.print(F("BPSK"));
   }
   else{
    Serial.print(F("QPSK"));
   }
   Serial.println(pskBaudRate[beacon.speedBcn]);
   break;
  case 3:
   Serial.print(F("CW "));
   Serial.print(wpm[beacon.wpm]);
   Serial.println(F("wpm "));
  break;
  default:
  Serial.println(serial_mode[beacon.modeBcn]);
 }
 Serial.print(F("Power : "));
 Serial.print(dbm[beacon.power[band]]);
 Serial.println(F("dBm"));

 if(beacon.modeBcn != 5){
  Serial.print(F("Tx string : "));
  Serial.println(beacon.texte);
 }
 serialClockDisplay();
} // End of serial_print()


byte handshake(byte wait){
  int data = 0;
  int compteur = 0;
  if(wait == 0){
  while (compteur < 10000){
   if (Serial.available() > 0){
    data = Serial.read();
   }
   compteur++;
   }
  }
  else{
   while (data == 0){
    if (Serial.available() > 0){
     data = Serial.read();
    }
   }
  }
 return data;
} // End of handshake


void command(){
 byte com_data;
 if(beacon.lcd_enable == 1){
  lcd.clear();
 }
 while(1){
  if(beacon.lcd_enable == 1){
   lcd.setCursor(0, 0);
   lcd.print(F("   Connected to PC   "));
  }
  com_data = handshake(1);
  switch (com_data){
   case 0x53 :
    receiveData();
   break;
   case 0x52 :
    sendData();
   break;
   case 0x54 :
    syncRTC();
   break;
  }
 }
} // End of command()


void receiveData(){
 byte receivedData;
 int bytesReceived = 0;
 while(bytesReceived < 350){
   if(Serial.available() > 0){
    receivedData = Serial.read();
    *((char*)&beacon + bytesReceived) = receivedData;
    bytesReceived++;
   }
 }
 for(int i = 0; i < 350; i++){
   EEPROM.write(i, *((char*)&beacon + i));
 }
 if(beacon.lcd_enable == 1){
  lcd.setCursor(0, 2);
  lcd.print(F("   EEPROM Updated   "));
 }
} // End of receiveData()


void sendData(){
 for(int i = 0; i < 350; i++){
  Serial.write(*((char*)&beacon + i));
 }
 if(beacon.lcd_enable == 1){
  lcd.setCursor(0, 2);
  lcd.print(F(" Config transmitted "));
 }
} // End of sendData()


void syncRTC(){
 char message[7];
 if(Serial.readBytes(message, 7) > 0){
  Wire.beginTransmission(0x68);
  Wire.write(byte(0));
  for (int n = 6; n >= 0; n--){
   Wire.write(decToBcd(byte(message[n])));
  }
  Wire.write(byte(0));
  Wire.endTransmission();
  if(beacon.lcd_enable == 1){
  lcd.setCursor(0, 2);
  lcd.print(F("  RTC synchronised  "));
 }
 }
} // End of syncRTC()


byte decToBcd(byte val) {
 return ((val / 10 * 16) + (val % 10));
} // End of decToBcd()


byte bcdToDec(byte val) {
 return ((val / 16 * 10) + (val % 16));
} // End of bcdToDec()


void init_beacon(){
 if (EEPROM.read(0) == 'B' || EEPROM.read(0) == 'Q'){
  for(int i = 0; i < 350; i++){
   *((char*)&beacon + i) = EEPROM.read(i);
  }
 }
 else{
  beacon.confDef = 'B';            // B : BPSK - Q : QPSK
  beacon.modeBcn = 1;              // 1 : PSK - 2 : Hellschreiber - 3 : CW  - 4 : RTTY - 5 : WSPR 
  beacon.seqBcn = 5;               // 1 : continue - 2 : every - 3 : hour - 4 : sweep - 5 :debug
  beacon.minBcn = 2;               // Minutes between transmissions in hour and every modes
  beacon.hourBcn[0] = 0;           // Hour of transmission beginning for frequency 1
  beacon.hourBcn[1] = 0;           // Hour of transmission beginning for frequency 2
  beacon.hourBcn[2] = 0;           // Hour of transmission beginning for frequency 3
  beacon.pwm[0] = 0;               //
  beacon.pwm[1] = 0;               //
  beacon.pwm[2] = 0;               //
  beacon.power[0] = 0;             //power en dbm
  beacon.power[1] = 0;             //power en dbm
  beacon.power[2] = 0;             //power en dbm
  beacon.speedBcn = 0;             // Baud rate for PSK
  beacon.wpm = 0;                  // Words per minute for CW
  beacon.rsid_toggle = 0;          // 0 : RSID inactive - 1 : RSID active in PSK, HELL and RTTY
  beacon.swr_enable = 0;           // 
  beacon.lcd_enable = 0;           // 0 : LCD display disabled - 1 : LCD display enabled
  beacon.serial_enable = 0;        //
  beacon.bandfreq1select = 0;      //
  beacon.bandfreq2select = 0;      //
  beacon.bandfreq3select = 0;      //
  beacon.trim = 125000000L;        //
  beacon.freqBcn[0] = 10140000L;   //
  beacon.freqBcn[1] = 10140000L;
  beacon.freqBcn[2] = 10140000L;
  strcpy(beacon.callsign,"F7XYZ");
  strcpy(beacon.locator,"AA00AA");
  strcpy(beacon.email,"f7xyz@foo.foo");
  strcpy(beacon.texte," f7xyz f7xyz beacon test 0123456789 f7xyz f7xyz");
 }
} // End of init_beacon


// LCD time display
void digitalClockDisplay(){
 printDigits(tm.Hour);
 lcd.print(F(":"));
 printDigits(tm.Minute);
 lcd.print(F(":"));
 printDigits(tm.Second);
} // End of digitalClockDisplay()

// LCD format time 
void printDigits(int digits){
  if(digits < 10) lcd.print('0');
  lcd.print(digits);
} // End of printDigits()


// Serial time display
void serialClockDisplay(){
   serialDigits(tm.Hour);
   Serial.print(F(":"));
   serialDigits(tm.Minute);
   Serial.print(F(":"));
   serialDigits(tm.Second);
   Serial.println(F("Z "));
 } // End of serialClockDisplay()

 
// Serial format time 
void serialDigits(int digits){
  if(digits < 10) Serial.print('0');
  Serial.print(digits);
}
// End of serialDigits()


//--------------------------------------- routines i2c des drivers MCP23008
void send_gpio(byte adresse, byte registre, byte valeur){
 Wire.beginTransmission(adresse);      // start talking to the device
 Wire.write(registre);                   // select the IODIR register
 Wire.write(valeur);                   // set register value-all high, sets all pins as outputs on MCP23008
 Wire.endTransmission();            // stop talking to the devicevice
} // End of send_gpio()


void LCUpDown(byte lu, int pas){
 //Exemple : LCUpDown (1,-1) décremente C
 //Exemple : LCUpDown (0,10) incrémente L de 10

 if (lu == 0){
  lval += pas;
 }
 else {
  cval += pas;
 }
 cval = (cval & 0x7f) | (hzlz * 128); //securité hzlz pour le moment
 send_gpio(LRAdr, 0x09, ~lval); 
 send_gpio(CRAdr, 0x09, ~cval);
} // End of LCUpDown()


//--------------------------------------- essai de recherche rapide avec sortie immediate

#define delta_Lfine 8
#define delta_Cfine 16

int search_atu(){
 int swr_min = 100;
 int current_swr;
 int hz = 0;
 int Lvalue;
 int Cvalue;

do{
 Lvalue = 0;
 do{
  Cvalue = 0;
  do{
   send_gpio(LRAdr, 0x09, ~Lvalue); 
   send_gpio(CRAdr, 0x09, ~(Cvalue | (hz * 128)));  
   delay(20);
   current_swr = mesureSWR();
   if(beacon.lcd_enable == 1) Bargraph(current_swr, 1, 1);
    if (current_swr < swr_min){
     cval = Cvalue;
     lval = Lvalue;
     hzlz = hz;
     swr_min = current_swr;
     if(beacon.serial_enable == 1){    
      Serial.print(cval);
      Serial.print(", ");
      Serial.print(lval);
      Serial.print(", ");
      Serial.println(swr_min);
     }
    }
    Cvalue = Cvalue * 2 + 1;
   } while ((Cvalue <= 127) && (swr_min > 10));
   Lvalue = Lvalue * 2 + 1;
  } while ((Lvalue <= 255) && (swr_min > 10)); 
  hz++;  
 } while ((hz <= 1) && (swr_min > 10));
 if(beacon.lcd_enable == 1){
  Bargraph(swr_min,1,1);
 }

 send_gpio(LRAdr, 0x09, ~lval); 
 send_gpio(CRAdr, 0x09, ~(cval | (hzlz * 128)));  
 if(beacon.serial_enable == 1){
  Serial.print(F("C Val :"));
  Serial.print(cval);
  Serial.print(F(", L Val :"));
  Serial.print(lval);
  Serial.print(F(", HZLZ :"));
  Serial.print(hzlz);
  Serial.print(F(", SWR final large :"));
  Serial.println(swr_min);    
 }
 int Lstart;
 int Lstop;
 int Cstart;
 int Cstop;

 if (lval < delta_Lfine){
  Lstart = lval;
  Lstop = lval + delta_Lfine;
 }
 else if ((lval >= delta_Lfine) && (lval < 255)){
  Lstart = lval - delta_Lfine;
  Lstop = lval + delta_Lfine;
 }
 else{
  Lstart = lval - delta_Lfine;
  Lstop = lval;
 }

 if (cval < delta_Cfine){
  Cstart = cval;
  Cstop = cval + delta_Cfine;
 }
 else if ((cval >= delta_Cfine) && (cval < 127)){
  Cstart = cval - delta_Cfine;
  Cstop = cval + delta_Cfine;
 }
 else{
  Cstart = cval - delta_Cfine;
  Cstop = cval;
 }
 
 Lvalue = Lstart;
 do{
  //for (int Cvalue=0; Cvalue<=consigne_cmax;Cvalue++)
  Cvalue = Cstart;
  do{
   send_gpio(LRAdr, 0x09, ~Lvalue); 
   send_gpio(CRAdr, 0x09, ~(Cvalue | (hzlz * 128)));  
   delay(20);
   current_swr = mesureSWR();
   if(beacon.lcd_enable == 1){
    Bargraph(current_swr, 2, 0);
   }
   if (current_swr < swr_min) {
    cval = Cvalue;
    lval = Lvalue;
    swr_min = current_swr;
    if(beacon.serial_enable == 1){             
     Serial.print(cval);
     Serial.print(", ");
     Serial.print(lval);
     Serial.print(", ");
     Serial.println(swr_min);
    }
   }
   Cvalue++;
  } while ((Cvalue <= Cstop) && (swr_min > 10));  
  Lvalue++;
 } while ((Lvalue <= Lstop) && (swr_min > 10)); 

 if(beacon.lcd_enable == 1){
  Bargraph(swr_min, 2, 0);
 }
 send_gpio(LRAdr, 0x09, ~lval); 
 send_gpio(CRAdr, 0x09, ~(cval | (hzlz * 128)));  
 if(beacon.serial_enable == 1){
  Serial.print(F("C Val :"));
  Serial.print(cval);
  Serial.print(F(", L Val :"));
  Serial.print(lval);
  Serial.print(F(", HZLZ :"));
  Serial.print(hzlz);
  Serial.print(F(", SWR final fine :"));
  Serial.println(swr_min);    
 }
 return swr_min;  
} // End of search_atu()


void Bargraph(int swr, int ligne_curseur, byte large_or_fine){
 int curseur_map;
 // curseur swr depart 6 fin 19 (1ong car 19-6+1=14) 5*14=70 avec -1 marge pour pas déborder
 if (large_or_fine == 1){
  if (swr >= 100){
   swr = 100; //changement d'échelle pour recherche large (swr max 10)
  }
  curseur_map = map(swr, 10, 100, 1, 70 - 1);    
 }
 else{
  if (swr >= 40){
   swr=40; //changement d'échelle pour recherche fine (swr max 4)
  }
  curseur_map = map(swr, 10, 40, 1, 70 - 1);
 }
 int curseur_entier = curseur_map / 5; //un caractère 5 de large
 int curseur_modulo = curseur_map % 5; //modulo
 int start_curseur = 6; //position de départ en colonne sur l'afficheur
 int end_curseur = 6 + curseur_entier; //position de fin en caractère plein
 while(start_curseur != end_curseur){ //remplissage
  lcd.setCursor(start_curseur, ligne_curseur);
  lcd.write(byte(5));
  start_curseur++;
 }
 lcd.setCursor(start_curseur, ligne_curseur);  //positionne le caractère modulo 5
 lcd.write(byte(curseur_modulo));
 start_curseur++;
 while(start_curseur != 20){ //complète jusqu'a 19 en caractère vide
  lcd.setCursor(start_curseur, ligne_curseur); 
  lcd.write(byte(0));
  start_curseur++;
 }
 lcd.setCursor(8,3);    //affiche le résulat courant
 lcd.print(swr / 10);
 lcd.print(F("."));
 lcd.print(swr % 10);
 lcd.print(F("  "));
} // End of Bargraph()


// Debug mode
void debug(){
 int cde; 
 if (Serial.available() > 0) {
  cde = Serial.read(); // get incoming byte
  switch (cde){
   case '1' :
    debug_freq = 3600000L;
    DDS.setfreq(debug_freq, 0);
    Serial.println(F("80m 3.600.000 Mhz"));
   break;
   case '2' :
    debug_freq = 7100000L;
    DDS.setfreq(debug_freq, 0);
    Serial.println(F("40m 7.100.000 Mhz"));
   break;
   case '3' :
    debug_freq = 10140000L;
    DDS.setfreq(debug_freq, 0);
    Serial.println(F("30m 10.140.000 Mhz"));
   break;
   case '4' :
    debug_freq = 14070000L;
    DDS.setfreq(debug_freq, 0);
    Serial.println(F("20m 14.070.000 Mhz"));
   break;
   case '5' :
    debug_freq = 21070000L;
    DDS.setfreq(debug_freq, 0);
    Serial.println(F("15m 21.070.000 Mhz"));
   break;
   case '6' :
    debug_freq = 28070000L;
    DDS.setfreq(debug_freq, 0);
    Serial.println(F("10m 28.070.000 Mhz"));
   break;
   case 'a' :
    Beacon.ddsPower(1);
    Serial.println(F("dds on"));
   break;
   case 'q' :
    val_gain = 0;
    analogWrite(gain, val_gain);
    Beacon.ddsPower(0);
    Serial.println(F("dds off"));
   break;
   case 'z' :
    analogWrite(gain, val_gain);
    Serial.println(F("Gain"));
    Serial.println(val_gain);
   break;
   case 's' :
    analogWrite(gain, 0);
    Serial.println(F("Gain: 0"));
   break;
   case 'e' :
    analogWrite(gain, gain_plus(10));
    print_gain();
   break;
   case 'd' :
    analogWrite(gain, gain_plus(-10));
    print_gain();
   break;
   case 'r' :
    analogWrite(gain, gain_plus(+1));
    print_gain();
   break;
   case 'f' :
    analogWrite(gain, gain_plus(-1));
    print_gain();
   break;
   case 't' :
    calibre += 50;
    DDS.calibrate(calibre);
    DDS.setfreq(debug_freq,0);
    Serial.println(F("Calibre"));
    Serial.println(calibre);
   break;
   case 'g' :
    calibre -= 50;
    DDS.calibrate(calibre);
    DDS.setfreq(debug_freq,0);
    Serial.println(F("Calibre"));
    Serial.println(calibre);
   break;
   case 'w' :
    digitalWrite(relay[0], LOW);
    digitalWrite(relay[1], LOW);
    digitalWrite(relay[2], LOW);
    Serial.println(F("relais off"));
   break;
   case 'x' :
    digitalWrite(relay[0], HIGH);
    digitalWrite(relay[1], LOW);
    digitalWrite(relay[2], LOW);
    Serial.println(F("relais 1 on"));
   break;
   case 'c' :
    digitalWrite(relay[0], LOW);
    digitalWrite(relay[1], HIGH);
    digitalWrite(relay[2], LOW);
    Serial.println(F("relais 2 on"));
   break;
   case 'v' :
    digitalWrite(relay[0], LOW);
    digitalWrite(relay[1], LOW);
    digitalWrite(relay[2], HIGH);
    Serial.println(F("relais 3 on"));
   break; 
   case 'h' :
    Serial.println(F("80m : 1 | 40m : 2 | 30m : 3 | 20m : 4 | 15m : 5 | 10m : 6"));
    Serial.println(F("RELAYS OFF : w | RELAY 1 ON : x | RELAY 2 ON : c  | RELAY 3 ON : v"));
    Serial.println(F("  DDS on : a | DDS off :  q"));
    Serial.println(F(" GAIN on : z | GAIN off : s"));
    Serial.println(F("GAIN +10 : e | GAIN -10 : d"));
    Serial.println(F(" GAIN +1 : r | GAIN -1 :  f"));
    Serial.println(F("Freq + : u   | Freq - : j | calibre UP : n"));        
    Serial.println(F("CALIBRATE +50 : t | CALIBRATE -50 : g"));
    Serial.println(F("SWR MEASURE : y"));
    Serial.println(F("L UP : i | L DOWN : k ")); 
    Serial.println(F("C UP : o | C DOWN : l ")); 
    Serial.println(F("HzLz toggle : p | Search : m ")); 
   break;
   case 'y' :
    swr_result = mesureSWR();
    Serial.print(F("SWR : "));
    if(swr_result >= 100){
	 Serial.println(F("Infini"));
	}
    else{
     Serial.println((float)swr_result / 10);
	}
   break;
   case 'u' :
    debug_freq += calibre_freq[calibre_ptr];
    DDS.setfreq(debug_freq, 0);
    Serial.print(F("Freq : "));
    Serial.println(debug_freq);
   break;    
   case 'j' :
    debug_freq -= calibre_freq[calibre_ptr];
    DDS.setfreq(debug_freq, 0);
    Serial.print(F("Freq : "));
    Serial.println(debug_freq);
   break;     
   case 'n' :
    calibre_ptr++;
    if (calibre_ptr == 6){
	 calibre_ptr = 0;
	}
    Serial.print(F("Calibre : "));
    Serial.println(calibre_freq[calibre_ptr]);
   break;    
   case 'i' :
    LCUpDown(0, 1);
    Serial.print(F("L UP Val :"));
    Serial.println(lval);
    break;    
   case 'k' :
    LCUpDown(0, -1);
    Serial.print(F("L DOWN Val :"));
    Serial.println(lval);
   break;    
   case 'o' :
    LCUpDown(1, 1);
    Serial.print(F("C UP Val :"));
    Serial.println(cval & 0x7f);
   break;    
   case 'l' :
    LCUpDown(1, -1);
    Serial.print(F("C DOWN Val :"));
    Serial.println(cval & 0x7f);
   break;    
   case 'p' :
    hzlz ^= 1;
    Serial.print(F("HzLz : "));
    Serial.println(hzlz);
    cval=(cval & 0x7f) | (hzlz * 128);
    send_gpio(CRAdr, 0x09, ~cval);
   break;    
   case 'm' :
    Serial.print(F("Searching..."));
    search_atu();
   break;
   default :
    Serial.println(F("key not recognized--> Press 'h' key for debug help"));        
  }
 }
} // End of debug mode


int gain_plus(int val){
 val_gain += val;
 if (val_gain > 255){
  val_gain = 255;
 }
 if (val_gain < 0){
  val_gain = 0;
 }
 return val_gain;
} // End of gain_plus()


void print_gain(){
 delay(300);
 Serial.print(F("Gain: "));
 Serial.print(val_gain);
 Serial.print(F(", VF: "));
 Serial.print(analogRead(vf));
 Serial.print(F(", VR: "));
 Serial.println(analogRead(vr)); 
} // End of print_gain


void print_beacon(){
 Serial.println(F("------------------"));  
 Serial.println(F("| HF Beacon V1.0  |"));  
 Serial.println(F("| f4goj@free.fr   |"));  
 Serial.println(F("| f4goh@orange.fr |"));  
 Serial.println(F("-------------------"));  
 Serial.println(beacon.confDef);
 Serial.println(beacon.modeBcn);
 Serial.println(beacon.seqBcn);
 Serial.println(beacon.minBcn);
 Serial.println(beacon.hourBcn[0]);
 Serial.println(beacon.hourBcn[1]);
 Serial.println(beacon.hourBcn[2]);
 Serial.println(beacon.pwm[0]);
 Serial.println(beacon.pwm[1]);
 Serial.println(beacon.pwm[2]);
 Serial.println(beacon.power[0]);
 Serial.println(beacon.power[1]);
 Serial.println(beacon.power[2]);
 Serial.println(beacon.speedBcn); 
 Serial.println(beacon.wpm);
 Serial.println(beacon.rsid_toggle);
 Serial.println(beacon.swr_enable);
 Serial.println(beacon.lcd_enable);
 Serial.println(beacon.serial_enable);
 Serial.println(beacon.bandfreq1select);
 Serial.println(beacon.bandfreq2select);
 Serial.println(beacon.bandfreq3select);
 Serial.println(beacon.trim);
 Serial.println(beacon.freqBcn[0]);
 Serial.println(beacon.freqBcn[1]);
 Serial.println(beacon.freqBcn[2]);
 Serial.println(beacon.callsign);
 Serial.println(beacon.locator);
 Serial.println(beacon.email);
 Serial.println(beacon.texte);
 Serial.println(F("----> Press 'h' key for debug help"));
} // End of print_beacon()
