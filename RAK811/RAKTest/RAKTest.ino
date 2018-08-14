char replybuffer[20];
unsigned long timer;

void setup() {
  Serial.begin(9600);

  sendCommand("at+version", "OK", 1000);
  sendCommand("at+mode=0", "OK", 1000);
  sendCommand("at+join=otaa", "at+recv=3,0,0", 8000);
}

void loop() {

  char payload[51];

  // Construct a Cayenne LPP packet (analogInput example)
  // Reference: https://mydevices.com/cayenne/docs/lora/#lora-cayenne-low-power-payload
  // Cast the 32 bit float (4 bytes) to a 16 bit signed integer (2 bytes)
  int16_t val = analogRead(A0);
  // Max input range -32768 to 32767
  // Note that the Cayenne LPP parser outputs this as -327.68 to 327.67 (the number is divided by 100)
  Serial.println(val);
  // Construct the command using sprintf: at+send=0,1,<Cayenne LPP> 0 = unconfirmed packet, portf = 1
  // 0102XXXX 01 = Channel 1, 02 = analogInput, XXXX = 2 byte hexadecimal integer
  // convert the two byte signed number to hexadecimal characters
  // output format %04x: % = start of character, 0 = pad with zeros, 4 = 4 digits, x = lowercase hexadecimal integer
  sprintf(payload, "at+send=0,1,0102%04x", val);

  sendCommand(payload, "at+recv=2,0,0", 2000);
  delay(60000);
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

