#include <SoftwareSerial.h>

#define BT_RX 10
#define BT_TX 11

SoftwareSerial android(BT_RX, BT_TX);

void setup(){
  // Bluetooth用のシリアルのポートを設定
  //android.begin(9600);
  android.begin(115200);
  // arduinoのシリアルモニタ用
  Serial.begin(19200);
      
  Serial.write("init");
}

void loop(){
  String x = String(analogRead(3));
  String y = String(analogRead(4));
  String z = String(analogRead(5));
  char buffer[x.length() + y.length() + z.length() + 5] = { 0 };
  String data = String(x + "," + y + "," + z + "\n");
  data.toCharArray(buffer, data.length() + 1);
  android.write(buffer);

  delay(100);
}
