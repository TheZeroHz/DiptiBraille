#define DISPLAY_ENABLE true
#define DISPLAY_DISABLE false
#define DEFAULT_STATE DISPLAY_ENABLE
#define DEFAULT_FREQUENCY 30
#define DEFAULT_COOLINGFACTOR 4
#define DEFAULT_BURNINGFACTOR 0.5
#define RESET_CODE 128
#define IDLE_CODE 0
#define CMD_DEFAULT_BAUDRATE 9600
#define CMD_DEFAULT_CHANNEL(x) Serial.begin(x)
#define CMD_SECOND_CHANNEL(x) //Serial2.begin(x)

const int bufferSize = 64;
char buffer[bufferSize];
int bufferIndex = 0;
int DISP_DATA=0;

struct DisplaySettings{
  int STCP,DS,SHCP,frequency,quantity;
  bool state;
  float cooling_coefficient,burning_coefficient;
};

class Display{
  protected:
  float version=1.01;
  DisplaySettings Settings;
  void pushToRegister(int base_10);
  int binstrToint(String binary_string);
  
  public:
  void setPins(int dataPin,int clockPin,int latchPin,int numOFcells);
  void initDisplay(int refresh_rate);
  void disable();
  void enable();
  void setFrequency(int hz);
  void setFactor(float cool,float burn);
  bool isEnabled(); 
  float getVersion();
  int getFrequency();
  void clear();
  void writeDec(int DATA_DECIMAL);
  void writeBinStr(String DATA_BINARY_STRING);
  void writeChar(char DATA_CHAR);
};




class CMD:public Display{
  public:
  void init(int BAUD_RATE,int CHANNEL_NO);
  void execute(const char* command);
  private:
  void cmdHelp();
  void cmdGreet();
  void cmdClear();
};

void CMD::init(int BAUD_RATE,int CHANNEL_NO){
    if(CHANNEL_NO==2)CMD_SECOND_CHANNEL(BAUD_RATE);
    else CMD_DEFAULT_CHANNEL(BAUD_RATE);
    cmdClear();
    Serial.print(F("@root:~"));

}
void CMD::cmdGreet(){
  Serial.println(F("...........(c) ZeroHZ Corporation. All rights reserved.."));
 Serial.println(F(".........####%...######..####%...######..######........."));
  Serial.println(F(".........##..##....##....##..##....##......##..........."));
  Serial.println(F(".........##..##....##....####%.....##......##..........."));
  Serial.println(F(".........##..##....##....##........##......##..........."));
  Serial.println(F(".........####%...######..##........##....######........."));
  Serial.println(F("..............................[Version 1.01 beta]......."));
  Serial.println();
  Serial.println();
  Serial.println();
}
void CMD::cmdHelp(){
    Serial.println(F("Available commands:"));
    Serial.println(F("-help (For all available commands)"));
    Serial.println(F("clear (Clear Console)"));
    Serial.println(F("disp [function] [data]"));
}
void CMD::cmdClear(){
    for (int i = 0; i < 50; i++)Serial.println();
        cmdGreet();
}

void CMD::execute(const char* command){
    char commandCopy[bufferSize];
    strcpy(commandCopy, command);

    char* cmd = strtok(commandCopy, " ");
    char* arg1 = strtok(NULL, " ");
    char* arg2 = strtok(NULL, " ");

    if (strcmp(cmd, "-help") == 0)cmdHelp();
    else if(strcmp(cmd,"clear")==0)cmdClear();
    else if(strcmp(cmd,"disp")==0){
      if(strcmp(arg1,"-n")==0){
        char* endPtr;
        int n = strtol(arg2, &endPtr, 10);
        // Check for errors
        if (*endPtr != '\0' && *endPtr != '\n')Serial.println("Conversion error: Not a valid integer.");
        else DISP_DATA=n;
      }
      else if(strcmp(arg1,"-f")==0){
        char* endPtr;
        int n = strtol(arg2, &endPtr, 10);
        // Check for errors
        if (*endPtr != '\0' && *endPtr != '\n')Serial.println("Conversion error: Not a valid integer.");
        else Settings.frequency=n;

      }
    }
    else {
      Serial.println("Invalid command. Type '-help' for available commands.");
    }
}





void Display::setPins(int dataPin,int clockPin,int latchPin,int numOFcells){
Settings.DS=dataPin;
Settings.STCP=latchPin;
Settings.SHCP=clockPin;
Settings.quantity=numOFcells;
Settings.state=DEFAULT_STATE;
Settings.burning_coefficient=DEFAULT_BURNINGFACTOR;
Settings.cooling_coefficient=DEFAULT_COOLINGFACTOR;
}


void Display::initDisplay(int refresh_rate){
  Settings.frequency=refresh_rate;
  pinMode(Settings.DS,OUTPUT);
  pinMode(Settings.STCP,OUTPUT);
  pinMode(Settings.SHCP,OUTPUT);
}

void Display::setFactor(float cool, float burn){
  Settings.burning_coefficient=burn;
  Settings.cooling_coefficient=cool;
}

float Display::getVersion(){return version;}
int Display::getFrequency(){return Settings.frequency;}

void Display::setFrequency(int hz){Settings.frequency=hz;}
bool Display::isEnabled(){return Settings.state;}

void Display::enable(){Settings.state=DISPLAY_ENABLE;}
void Display::disable(){writeDec(IDLE_CODE);Settings.state=DISPLAY_DISABLE;}


int Display::binstrToint(String binary_string){
  return strtol(binary_string.c_str(), NULL, 2);
}

void Display::pushToRegister(int base_10){
  if(Settings.state==DISPLAY_ENABLE){
  digitalWrite(Settings.STCP, LOW);
  shiftOut(Settings.DS, Settings.SHCP, LSBFIRST, base_10);
  digitalWrite(Settings.STCP, HIGH);
  }
}

void Display::clear(){pushToRegister(RESET_CODE);}

void Display::writeDec(int DATA_DECIMAL){
  float timer = 1000.0 / ((Settings.burning_coefficient+Settings.cooling_coefficient)*Settings.frequency);
  pushToRegister(DATA_DECIMAL);
  delay((int)(timer*Settings.burning_coefficient));
  pushToRegister(IDLE_CODE);
  delay((int)(timer *Settings.cooling_coefficient));
}

void Display::writeBinStr(String DATA_BINARY_STRING){
  writeDec(binstrToint(DATA_BINARY_STRING));
}


//Pin connected to Clock ST_CP of 74HC595
int latchPin = 13;
//Pin connected to Latch SH_CP of 74HC595
int clockPin = 12;
////Pin connected to Data DS of 74HC595
int dataPin = 8;

CMD cmd;
void setup() {
  cmd.setPins(8,12,13,1);
  cmd.init(9600,1);
  cmd.initDisplay(4);
}
void loop() {
  while (Serial.available() > 0) {
    char incomingByte = Serial.read();
    // Process command if newline character is received
    if (incomingByte == '\n') {
      buffer[bufferIndex] = '\0'; // Null-terminate the string
      Serial.println(buffer);
      cmd.execute(buffer);
      Serial.print("@root:~");
      bufferIndex = 0; // Reset buffer index
    } else {
      // Add character to buffer if not newline
      if (bufferIndex < bufferSize - 1) {
        buffer[bufferIndex] = incomingByte;
        bufferIndex++;
      }
    }
  }
  cmd.writeDec(DISP_DATA);
}