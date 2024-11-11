#include"GSM.h"

phone::phone(uint8_t gsmRx,uint8_t gsmTx,unsigned long baud){
  Serial2.begin(baud, SERIAL_8N1, gsmRx,gsmTx);  
}

void phone::cmd(String command){
  Serial2.println(command);
  Serial.println(getResponse());
}

void phone::loop(){
  notifier=getResponse();
  if(notifier.indexOf("Ring")!=-1)Serial.println("Incoming call......");
  if(notifier.length()>0){ Serial.println(notifier);notifier="";}
}

String phone::getResponse(){
  String response = "";
  unsigned long startTime = millis(); // Record the start time for timeout
  while ((millis() - startTime) < 1000){
    if(Serial2.available())response = Serial2.readString();
    }
  return response;
}

int phone::batteryPercentage(){
    Serial2.println("AT+CBC;");
  String response = getResponse();
  int batteryPercentage = -1; // Default value in case parsing fails
  // Example response format: "+CBC: 0,50,4211"
  int commaIndex = response.indexOf(',');
  if (commaIndex != -1) {
    int secondCommaIndex = response.indexOf(',', commaIndex + 1);
    if (secondCommaIndex != -1) {
      String batteryLevelStr = response.substring(commaIndex + 1, secondCommaIndex);
      batteryPercentage = batteryLevelStr.toInt();
    }
  }
  return batteryPercentage;
}
bool phone::isOK(){
  Serial2.println("AT");
  if((getResponse()).indexOf("OK")!=-1)return true;
  else return false;
}

void phone::call(String number){
  Serial2.println("ATD"+number+";");
}

void phone::cut_call(){
  Serial2.println("ATH");
}
void phone::accept_call(){
  Serial2.println("ATA");
}

void phone::msg(String number, String text){
  Serial2.println("AT+CMGF=1");
  Serial.println(getResponse());
  Serial2.println("AT+CNMI=1,2,0,0,0");
  Serial.println(getResponse());
  Serial2.print("AT+CMGS=\"" + number + "\"\r");
  Serial.println(getResponse());
  Serial2.print(text);
  Serial2.write(0x1A);
  Serial.println(getResponse());
}


String phone::getTime(){
  Serial2.println("AT+CCLK?");
  String time=getResponse();
  return time.substring(time.indexOf(',')+1,time.length()-12);
}