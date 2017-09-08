#include <SoftwareSerial.h>
#include <Wire.h> //I2C Arduino Library

#define BT_RX 10
#define BT_TX 11

#define address 0x1E // HMC5883 の I2C 7bit address

SoftwareSerial android(BT_RX, BT_TX);

void setup(){
  // Bluetooth用のシリアルのポートを設定
  android.begin(115200);
  // arduinoのシリアルモニタ用
  Serial.begin(19200);
  // I2Cの通信用
  Wire.begin();
      
  //HMC5883のモード設定
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
}

void loop(){
  // 加速度の値取得
  String accele_x = String(analogRead(0));
  String accele_y = String(analogRead(1));
  String accele_z = String(analogRead(2));

  // 地磁気の値取得
  Wire.beginTransmission(address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
  Wire.requestFrom(address, 6);
  int x,y,z;
  String geomag_x, geomag_z, geomag_y;  
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    geomag_x = String(x);
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    geomag_z = String(z);
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
    geomag_y = String(y);
  }

  // androidへ値の送信
  char buffer[accele_x.length() + accele_y.length() + accele_z.length() + geomag_x.length() + geomag_z.length() + geomag_y.length() + 5] = { 0 };
  String data = String(accele_x + "," + accele_y + "," + accele_z + "," + geomag_x +  ","  + geomag_z +  ","  + geomag_y + "\n");
  data.toCharArray(buffer, data.length() + 1);
  android.write(buffer);

  delay(100);
}
