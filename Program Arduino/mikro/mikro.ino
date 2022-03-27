#include <Wire.h>
#include "RTClib.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

WiFiClient client;
RTC_DS1307 RTC;
const char* host = "192.168.43.10"; 
HTTPClient http;
String url = "GET /rula/tambah.php?";

String range_update;
String awal_update;
String akhir_update;
int selisih,wal,wak;
int asal;
int pertama=1;
int buff=0;

//waktu update 1,2,3,4 (Berdasarkan type RULA)
int waktu_awal[5];
int waktu_akhir[5];
int jam_awal[5];
int jam_akhir[5];
int menit_awal[5];
int detik_awal[5];
int menit_akhir[5];
int detik_akhir[5];
int delta[5];

double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
int16_t tempRaw;
//double gyroXangle;
int skor, batas;
double compAngleX,sudut;

int led = D6;     // LED pin
int button = D5; // push button is connected
int temp = 0;   
int buf=0;

uint32_t timer;
uint8_t i2cData[14];

void setup() {
  pinMode(led, OUTPUT);  
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600);
  WiFi.disconnect();
  WiFi.begin("gateway","ripazhakjkszwj"); 
  
  while ((!(WiFi.status() == WL_CONNECTED))){
  digitalWrite(led,HIGH);
  delay(150);
  digitalWrite(led,LOW);
  delay(150);
  Serial.print(".");
  }
  
  Wire.begin();
  RTC.begin();
  i2cData[0] = 7; 
  i2cData[1] = 0x00; 
  i2cData[2] = 0x00; 
  i2cData[3] = 0x00; 
  while (i2cWrite(0x19, i2cData, 4, false)); 
  while (i2cWrite(0x6B, 0x01, true)); 

  while (i2cRead(0x75, i2cData, 1));
  if (i2cData[0] != 0x68) {
    Serial.print(F("Error reading sensor"));
    while (1);
  }
  
  delay(100); // Wait for sensor to stabilize
  while (i2cRead(0x3B, i2cData, 6));
  accX = (int16_t)((i2cData[0] << 8) | i2cData[1]);
  accY = (int16_t)((i2cData[2] << 8) | i2cData[3]);
  accZ = (int16_t)((i2cData[4] << 8) | i2cData[5]);
  double roll  = atan2(accY, accZ) * RAD_TO_DEG;
  //gyroXangle = roll;
  compAngleX = roll;
  timer = micros();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void kirim(String range, int skor, String awal, String akhir, int dt_rula, int asal)
{
  if (dt_rula>1)
  {
  Serial.print("Range:"); Serial.print(range); Serial.print("\t");
  Serial.print("skor:"); Serial.print(skor); Serial.print("\t");
  Serial.print("waktu awal:"); Serial.print(awal); Serial.print("\t");
  Serial.print("waktu akhir:"); Serial.print(akhir); Serial.print("\t");
  Serial.print("dt:"); Serial.print(dt_rula); Serial.print("\t");
  client.print( url + "range=" + range + "&" + "skor=" + skor + "&" + "awal=" + awal+ "&" + "akhir=" + akhir + "&" + "dt=" + dt_rula);
  client.print(" HTTP/1.1\r\nHost: host\r\nConnection:keep-alive\r\nAccept: */*\r\n\r\n");
  Serial.print("Terkirim");
  }
  for (int a=0;a<5;a++)
  {
    waktu_awal[a]=0;
    waktu_akhir[a]=0;
    jam_awal[a]=0;
    jam_akhir[a]=0;
    menit_awal[a]=0;
    detik_awal[a]=0;
    menit_akhir[a]=0;
    detik_akhir[a]=0;
    delta[a]=0;
  }
  pertama=1;
  wak=0;
  wal=0;
  selisih=0;
}


void loop() {
  temp = digitalRead(button);
  if ((temp == LOW)&&(buf==0)) 
     {buf=1;
     delay(500);}
     else if((temp == LOW)&&(buf==1))
     {buf=0;
     delay(500);}
  if(buf==1)
  {
  if (!client.connect(host,80)) {
  Serial.println("Gagal Konek");
  digitalWrite(led,HIGH);
  delay(150);
  digitalWrite(led,LOW);
  delay(150);
  return;
  }
  digitalWrite(led, HIGH);
  DateTime now = RTC.now(); 
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(" = ");
    
  while (i2cRead(0x3B, i2cData, 14));
  accX = (int16_t)((i2cData[0] << 8) | i2cData[1]);
  accY = (int16_t)((i2cData[2] << 8) | i2cData[3]);
  accZ = (int16_t)((i2cData[4] << 8) | i2cData[5]);
  tempRaw = (int16_t)((i2cData[6] << 8) | i2cData[7]);
  gyroX = (int16_t)((i2cData[8] << 8) | i2cData[9]);
  gyroY = (int16_t)((i2cData[10] << 8) | i2cData[11]);
  gyroZ = (int16_t)((i2cData[12] << 8) | i2cData[13]);

  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();
  
  double roll  = atan2(accY, accZ) * RAD_TO_DEG;
  double gyroXrate = gyroX / 131.0; 
  compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll; 
  
  sudut = map(compAngleX, 90, 0, 0, 90);
  Serial.print(sudut);
  if (sudut>=0&&sudut<1)
  {
    if((menit_awal[1]+detik_awal[1])==0)
      {
        if(pertama!=1)
        {kirim(range_update, skor, awal_update, akhir_update, selisih, asal);}
        asal=1;
        skor=1;
        jam_awal[1]=now.hour();
        menit_awal[1]=now.minute();
        detik_awal[1]=now.second();
        waktu_awal[1]=(menit_awal[1]*60)+(detik_awal[1]);
        wal=waktu_awal[1];
        awal_update=String(jam_awal[1])+":"+String(menit_awal[1])+":"+String(detik_awal[1]);
        range_update="0";
        pertama=0;
      }
      else
      {
        skor=1;
        jam_akhir[1]=now.hour();
        menit_akhir[1]=now.minute();
        detik_akhir[1]=now.second();
        waktu_akhir[1]=(menit_akhir[1]*60)+(detik_akhir[1]);
        wak=waktu_akhir[1];
        akhir_update=String(jam_akhir[1])+":"+String(menit_akhir[1])+":"+String(detik_akhir[1]);
        delta[1]=wak-wal;
        range_update="0";
        selisih=delta[1];
        pertama=0;
      }
  }
  
  else if (sudut>=1&&sudut<20)
  {
    
    if((menit_awal[2]+detik_awal[2])==0)
      {
        if(pertama!=1)
        {kirim(range_update, skor, awal_update, akhir_update, selisih, asal);}
        asal=2;
        skor=2;
        jam_awal[2]=now.hour();
        menit_awal[2]=now.minute();
        detik_awal[2]=now.second();
        waktu_awal[2]=(menit_awal[2]*60)+(detik_awal[2]);
        wal=waktu_awal[2];
        range_update="1-20";
        awal_update=String(jam_awal[2])+":"+String(menit_awal[2])+":"+String(detik_awal[2]);
        pertama=0;
      }
      else
      {
        skor=2;
        jam_akhir[2]=now.hour();
        menit_akhir[2]=now.minute();
        detik_akhir[2]=now.second();
        waktu_akhir[2]=(menit_akhir[2]*60)+(detik_akhir[2]);
        wak=waktu_akhir[2];
        akhir_update=String(jam_akhir[2])+":"+String(menit_akhir[2])+":"+String(detik_akhir[2]);
        delta[2]=wak-wal;
        range_update="1-20";
        selisih=delta[2];
       
      }
  }
  
  else if (sudut>=20&&sudut<60)
  {
    if((menit_awal[3]+detik_awal[3])==0)
      {
        if(pertama!=1)
        {kirim(range_update, skor, awal_update, akhir_update, selisih, asal);}
        asal=3;
        skor=3;
        jam_awal[3]=now.hour();
        menit_awal[3]=now.minute();
        detik_awal[3]=now.second();
        waktu_awal[3]=(menit_awal[3]*60)+(detik_awal[3]);
        wal= waktu_awal[3];
        range_update="20-60";
        awal_update=String(jam_awal[3])+":"+String(menit_awal[3])+":"+String(detik_awal[3]);
        pertama=0;
      }
      else
      {
        skor=3;
        jam_akhir[3]=now.hour();
        menit_akhir[3]=now.minute();
        detik_akhir[3]=now.second();
        waktu_akhir[3]=(menit_akhir[3]*60)+(detik_akhir[3]);
        wak=waktu_akhir[3];
        akhir_update=String(jam_akhir[3])+":"+String(menit_akhir[3])+":"+String(detik_akhir[3]);
        delta[3]=wak-wal;
        range_update="20-60";
        selisih=delta[3];
       
      }
  }
  else if (sudut>=60)
  {
    if((menit_awal[4]+detik_awal[4])==0)
      {
        if(pertama!=1)
        {kirim(range_update, skor, awal_update, akhir_update, selisih, asal);}
        asal=4;
        skor=4;
        jam_awal[4]=now.hour();
        menit_awal[4]=now.minute();
        detik_awal[4]=now.second();
        waktu_awal[4]=(menit_awal[4]*60)+(detik_awal[4]);
        wal=waktu_awal[4];
        range_update=">60";
        awal_update=String(jam_awal[4])+":"+String(menit_awal[4])+":"+String(detik_awal[4]);
        pertama=0;
      }
      else
      {
        skor=4;
        jam_akhir[4]=now.hour();
        menit_akhir[4]=now.minute();
        detik_akhir[4]=now.second();
        waktu_akhir[4]=menit_akhir[4]*60+detik_akhir[4];
        akhir_update=String(jam_akhir[4])+":"+String(menit_akhir[4])+":"+String(detik_akhir[4]);
        wak=waktu_akhir[4];
        delta[4]=wak-wal;
        range_update=">60";
        selisih=delta[4];
       
      } 
  }
  
  else if (sudut<0)
  {
    if((menit_awal[0]+detik_awal[0])==0)
      {
        if(pertama!=1)
        {kirim(range_update, skor, awal_update, akhir_update, selisih, asal);}
        asal=0;
        skor=0;
        jam_awal[0]=now.hour();
        menit_awal[0]=now.minute();
        detik_awal[0]=now.second();
        waktu_awal[0]=menit_awal[0]*60+detik_awal[0];
        range_update="<0";
        awal_update=String(jam_awal[0])+":"+String(menit_awal[0])+":"+String(detik_awal[0]);
        pertama=0;
      }
      else
      {
        skor=0;
        jam_akhir[0]=now.hour();
        menit_akhir[0]=now.minute();
        detik_akhir[0]=now.second();
        waktu_akhir[0]=menit_akhir[0]*60+detik_akhir[0];
        akhir_update=String(jam_akhir[0])+":"+String(menit_akhir[0])+":"+String(detik_akhir[0]);
        delta[0]=waktu_akhir[0]-waktu_awal[0];
        range_update="<0";
        selisih=delta[0];
      } 
  }
  Serial.print("\r\n");
  delay(2);
  }
  else
  {
    digitalWrite(led, LOW);
    Serial.println("Tekan Tombol Start");}
}
