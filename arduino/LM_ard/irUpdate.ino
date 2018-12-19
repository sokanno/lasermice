/*
IR command list on SONY RMT-831
o  >>|    1724    12bit   turn right
o  |<<    3772    12bit   turn left
o  play   1436    12bit   play
o  stop   412     12bit   stop

DIY IR-Commander command list
id is 8bit(0~255)
op is 2bit(0~4) [==, !=, <, >]
address is 3bit(0~7)
value is 16bit(0~65535)

address
0 Cycle Length (0-4095 ms)
1 Laser Reaction Length (%)
2 Motor Speed (0-255 PWM value)
3 Threshold of Light Detect (0-255)
4 Solenoid On Time (0-255 ms)
5 Sequence bit (16bit)[0000000000000000]
6 usage length (4bit)
7 functions*

*functions
0 -
1 play
2 stop
3 go to charge
4 save
5 sequence mode on
6 sequence mode off
7 sequence count reset
8 cue
*/

void irUpdate() {
  // receiving signal
  if (irrecv.decode(&results)) {
    // Serial.println(results.value);
    dump(&results);
    irrecv.resume(); // Receive the next value
    irCommandFlag = true;
    timeStamp_MTR = millis();
  }

  if (irFunction == sonyRemote) { // react to sony remote
    if (results.value == 1724) { // turn right
      if (timeStamp_MTR + reactionLength_MTR > millis() && irCommandFlag) {
        digitalWrite(MTR_B_F, LOW);
        digitalWrite(MTR_B_B, HIGH);
        analogWrite(MTR_B_PWM, 100);
      }
      else {
        analogWrite(MTR_B_PWM, 0);
        irCommandFlag = false;
        irNotification = true;
      }
    }
    if (results.value == 3772) { // turn left
      if (timeStamp_MTR + reactionLength_MTR > millis() && irCommandFlag) {
        digitalWrite(MTR_A_F, LOW);
        digitalWrite(MTR_A_B, HIGH);
        analogWrite(MTR_A_PWM, 100);
      }
      else {
        analogWrite(MTR_A_PWM, 0);
        irCommandFlag = false;
        irNotification = true;
      }
    }
    if (results.value == 1436 && irCommandFlag) { // play the behavior
      playFlag = true;
      irCommandFlag = false;
      irNotification = true;
    }
    if (results.value == 412 && irCommandFlag) { // stop the behavior
      playFlag = false;
      irCommandFlag = false;
      irNotification = true;
    }
  }

  
  else if (irFunction == irCommander && irCommandFlag) {
    // Serial.println(results.value);
    if (results.value != ignoreThis) {
      id = results.value >> 21;
      op = (results.value >> 19) - (id << 2);
      address = (results.value >> 16) - (op << 3) - (id << 5);
      value = results.value - (address << 16) - (op << 19) - (id << 21);
    }
    // id = results.value >> 24;
    // op = (results.value >> 20) - (id << 4);
    // address = (results.value >> 16) - (op << 4) - (id << 8);
    // value = results.value - (address << 16) - (op << 20) - (id << 24);

//    Serial.print(id);
//    Serial.print(' ');
//    Serial.print(op);
//    Serial.print(' ');
//    Serial.print(address);
//    Serial.print(' ');
//    Serial.println(value);

    // id is 8bit(0~255)
    // op is 2bit(0~4) [==, !=, <, >]
    // address is 3bit(0~7)
    // value is 16bit(0~65535)
    if (id >= 0 && id < 256 && 
        op >= 0 && op < 5 &&
        address >= 0 && address < 8 &&
        value >= 0 && value < 65536) {
      if (value > 4096 && address == 0 || // cycleLength
          value > 100 && address == 1 ||  // laserReaction_ratio
          value > 255 && address == 2 ||  // motorSpeed
          value > 127 && address == 3 ||  // threshold
          value < 3   && address == 3 ||
          value > 127 && address == 4 ||  // solenoid on time
          value > 16  && address == 6 ||  // loopDigit
          value > 32 && address == 7      // functions
            ){
        Serial.println("irCommand Error");
        irCommandFlag = false;
      }
      else if(id == 0 && op == 0 && address == 0){
        Serial.println("irCommand Error");
        irCommandFlag = false;
      }
      // those are in case of error, prevent values replacing
      else if (id == 0) {
        correspond = true;
      }
      else{
        parameterUpdate();
      }
    }
    else {
      Serial.println("irCommand Error");
      irCommandFlag = false;
    }
  }

  if (irNotification) {
    if (blinkCount < 5) {
      if (millis() % 100 > 50) {
        digitalWrite(LED_R, LOW);
        if (countFlag) {
          blinkCount++;
          countFlag = false;
        }
      }
      else {
        if(commandReaction) digitalWrite(LED_R, HIGH);
        if (!countFlag) countFlag = true;
      }
    }
    else {
      // Serial.println("blinkfinished");
      blinkCount = 0;
      irNotification = false;
    }
  }

  
}

void dump(decode_results *results) {
  if (results->bits == 12) irFunction = sonyRemote;
  else if (results->bits == 29) irFunction = irCommander;
}
