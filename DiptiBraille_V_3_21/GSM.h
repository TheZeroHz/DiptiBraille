#ifndef GSM_H
#define GSM_H
#include <Arduino.h>  // Include this if you are using Arduino platforms

class phone{
  private:
  int ok_count=0;
  String notifier;
  public:
  phone(uint8_t gsmRx,uint8_t gsmTx,unsigned long baud);
  void cmd(String command);
  void loop();
  bool isOK();
  String getName(String contact_no);
  String getNumber(String name);
  String getResponse();
  int batteryPercentage();
  void call(String number);
  void cut_call();
  void accept_call();
  void msg(String number,String text);
  void deep_sleep();
  void wake_up();
  String getTime();
  String getDate();
  
};
#endif // GSM_H