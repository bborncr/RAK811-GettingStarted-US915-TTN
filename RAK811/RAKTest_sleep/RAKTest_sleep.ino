// Requires Adafruit SleepyDog library (Install from the Library Manager)
#include <SoftwareSerial.h>
#include <Adafruit_SleepyDog.h>
#define DEBUG 1

#ifdef DEBUG
SoftwareSerial debug(2, 3);
#endif

char replybuffer[20];
unsigned long timer;
#ifdef DEBUG
bool WOKE = false;
#endif

void setup() {
  Serial.begin(9600);
#ifdef DEBUG
  debug.begin(9600);
#endif

#ifdef DEBUG
  debug.print("Initializing LoRaWAN Mode...");
#endif
  sendCommand("at+mode=0", "OK", 1000);
#ifdef DEBUG
  debug.println("OK");
#endif

#ifdef DEBUG
  debug.print("Joining gateway...");
#endif
  sendCommand("at+join=otaa", "at+recv=3,0,0", 8000);
  delay(8000);
#ifdef DEBUG
  debug.println("OK");
#endif
}

void loop() {
#ifdef DEBUG
  debug.print("Sending data...");
#endif
  sendCommand("at+send=0,8,01670110", "at+recv=2,0,0", 2000);
#ifdef DEBUG
  debug.println("OK");
#endif
  sleep();
}

bool sendCommand(char *command, char *reply, uint16_t timeout) {
  while (Serial.available()) Serial.read(); // Clear input buffer
  Serial.println(command);

  uint8_t idx = 0;
  bool replyMatch = false;
  timer = millis();

  while (!replyMatch && millis() - timer < timeout) {
    if (Serial.available()) {
      replybuffer[idx] = Serial.read();
      idx++;
      if (strstr(replybuffer, reply) != NULL) replyMatch = true; // Only checks if desired reply is inside replybuffer
      //if (strcmp(replybuffer, reply) == 0) replyMatch = true; // This means the reply must start with the desired reply to be successful
    }
  }
  if (replyMatch) return true;
  else return false;
}

void sleep() {

#ifdef DEBUG
  debug.print("Putting radio to sleep...");
#endif

  // put the radio to sleep
  sendCommand("at+sleep", "OK", 1000);

#ifdef DEBUG
  debug.println("OK");
#endif

#ifdef DEBUG
  debug.print("uC going to sleep...");
#endif

  // put the microcontroller to sleep for 8000mS * 7 ~56seconds
  for (int i = 0; i < 7; i++) {
    int sleepMS = Watchdog.sleep(8000);
  }
#ifdef DEBUG
  debug.println("waking up");
#endif

#ifdef DEBUG
  debug.print("Waking radio...");
#endif
  // wake the radio up (any character will wake up radio)
  if (sendCommand("w", "at+recv=8,0,0", 1000)) {
#ifdef DEBUG
    WOKE = true;
#endif
  }
#ifdef DEBUG
  if (WOKE) {
    debug.println("OK");
  } else {
    debug.println("FAIL");
    WOKE = false;
  }
#endif
}

