#ifndef VIBRATOR_H
#define VIBRATOR_H

class vibrating_wave{
public:
int total_duration;
int *pattern;
void oscilate();
};

class vibrator{
public:
vibrating_wave waves[11];
enum notification_type{
  power_on,
  power_off,
  phone_call,
  phone_sms,
  alarm,
  battery_0,
  battery_25,
  battery_50,
  battery_75,
  battery_100,
  os_crashed,
  sd_fail,
  
};
public:
vibrator(int signal_pin);
void notify(notification_type notifier);

};
#endif