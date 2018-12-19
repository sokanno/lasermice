
void laserDetect_React() {
  int raw0 = analogRead(CDS);
  int smoothedByMeanFilter0 = smoothByMeanFilter(0);
  int raw1 = analogRead(CDS_L);
  int smoothedByMeanFilter1 = smoothByMeanFilter(1);
  int raw2 = analogRead(CDS_R);
  int smoothedByMeanFilter2 = smoothByMeanFilter(2);


  if (!bangFlag_LSR && raw0 - smoothedByMeanFilter0 > threshold ||
      threeSensorsMode && !bangFlag_LSR && raw1 - smoothedByMeanFilter1 > threshold ||
      threeSensorsMode && !bangFlag_LSR && raw2 - smoothedByMeanFilter2 > threshold ) {
    timeStamp_LSR = millis();
    bangFlag_LSR = true;
    bangFlag_SLND = true;
    turnFlag = true;
    // shiftAmount = shiftAmount + (millis()%cycleLength);
    shiftAmount = millis() % cycleLength;
    beShotCount ++;

    //report to database
    if (detectReport) {
      Serial1.write(byte(0));
      Serial1.write(byte(myId));
      int slb = strikeCount >> 8;
      Serial1.write(byte(slb));
      Serial1.write(byte(strikeCount - (slb << 8)));
      int blb = beShotCount >> 8;
      Serial1.write(byte(blb));
      Serial1.write(byte(beShotCount - (blb << 8)));
    }
  }
  if (bangFlag_SLND) {
    digitalWrite(SOLENOID, HIGH);
    // Serial.println("solonoid on");
  }
  if (millis() > timeStamp_LSR + reactionLength_SLND) {
    digitalWrite(SOLENOID, LOW);
    bangFlag_SLND = false;
  }
  if (bangFlag_LSR) {
    digitalWrite(LASER, HIGH);
    // Serial.println("laser on");
  }
  if (millis() > timeStamp_LSR + reactionLength_LSR) {
    if (!keepLaser) digitalWrite(LASER, LOW);
    bangFlag_LSR = false;
  }
  // if(sequenceMode){
  //   if (millis() > timeStamp_LSR + reactionLength_LSR / 2) {
  //     digitalWrite(LASER, LOW);
  //     bangFlag_LSR = false;
  //   }
  // }
  // else if(!sequenceMode){
  //   if (millis() > timeStamp_LSR + reactionLength_LSR) {
  //     digitalWrite(LASER, LOW);
  //     bangFlag_LSR = false;
  //   }
  // }


  buffer0[index] = raw0;
  buffer1[index] = raw1;
  buffer2[index] = raw2;
  index = (index + 1) % BUFFER_LENGTH;
  // if(index % BUFFER_LENGTH == 0){
  //   Serial.println(millis());
  // }

  // Serial.print(raw);
  // Serial.print(' ');
  // Serial.println(smoothedByMeanFilter);

  if (rotateReactFlag && !playFlag) {
    // truning around while laser is on
    if (bangFlag_LSR) {
      // turn
      if (turnDirection) {
        digitalWrite(MTR_A_F, HIGH);
        digitalWrite(MTR_A_B, LOW);
        digitalWrite(MTR_B_F, LOW);
        digitalWrite(MTR_B_B, HIGH);
        analogWrite(MTR_A_PWM, motorSpeed);
        analogWrite(MTR_B_PWM, motorSpeed);
      }
      else if (!turnDirection) {
        digitalWrite(MTR_A_F, LOW);
        digitalWrite(MTR_A_B, HIGH);
        digitalWrite(MTR_B_F, HIGH);
        digitalWrite(MTR_B_B, LOW);
        analogWrite(MTR_A_PWM, motorSpeed);
        analogWrite(MTR_B_PWM, motorSpeed);
      }
    }
    // finish turning when laser blink has finished
    if (millis() > timeStamp_LSR + reactionLength_LSR && turnFlag == true) {
      // digitalWrite(LASER, LOW);
      if (turnDirection) {
        analogWrite(MTR_A_PWM, 0);
        analogWrite(MTR_B_PWM, 0);
        // digitalWrite(MTR_B_F, HIGH);
        // digitalWrite(MTR_B_B, LOW);
        turnFlag = false;
      }
      else if (!turnDirection) {
        analogWrite(MTR_A_PWM, 0);
        analogWrite(MTR_B_PWM, 0);
        // digitalWrite(MTR_A_F, HIGH);
        // digitalWrite(MTR_A_B, LOW);
        turnFlag = false;
      }
      // Serial.println("turn finished");
      if (int(random(50)) % 2 == 1) turnDirection = true;
      else turnDirection = false;
      // Serial.println(turnDirection);
    }
  }
}

int smoothByMeanFilter(int a) {
  long sum = 0;
  switch (a) {
    case 0:
      for (int i = 0; i < BUFFER_LENGTH; i++) {
        sum += buffer0[i];
      }
      break;
    case 1:
      for (int i = 0; i < BUFFER_LENGTH; i++) {
        sum += buffer1[i];
      }
      break;
    case 2:
      for (int i = 0; i < BUFFER_LENGTH; i++) {
        sum += buffer2[i];
      }
      break;
  }
  return (int)(sum / BUFFER_LENGTH);
}

