
void play() {
  if (playFlag) { // "play" only works when playFlag is true

    if ((millis() % cycleLength) > shiftAmount &&
        (millis() % cycleLength) < shiftAmount + debounce && !bangFlag) {
      bangFlag_LSR = true;
      bangFlag_SLND = true;
      timeStamp_LSR = millis();
      bangFlag = true;
      turnFlag = true;
      strikeCount++;
    }

    // something common in Seq mode and non-seq mode
    if (millis() - timeStamp_LSR > debounce) {
      bangFlag = false;
    }

    // calculate modulated motor speed
    int modulatedMotorSpeed;
    if (speedModulation) {
      int pos = (millis() - shiftAmount) % cycleLength;
      //      int relPos = (absPos + shiftAmount) % cycleLength;
      modulatedMotorSpeed = map(pos, 0, cycleLength, motorSpeed, 0);
    }
    else {
      modulatedMotorSpeed = motorSpeed;
    }

    // moving forward
    // first, exception for boolean "noForwardMove"
    if (!bumpLreactFlag && !bumpRreactFlag && noForwardMove && !bangFlag_LSR) {
      analogWrite(MTR_A_PWM, 0);
      analogWrite(MTR_B_PWM, 0);
    }
    else if (!bumpLreactFlag && !bumpRreactFlag) {
      digitalWrite(MTR_A_F, HIGH);
      digitalWrite(MTR_A_B, LOW);
      digitalWrite(MTR_B_F, HIGH);
      digitalWrite(MTR_B_B, LOW);
      if (lightFollow) {
        int brghtnsL = analogRead(CDS_L);
        int brghtnsR = analogRead(CDS_R);
//        int brDif = constrain(((brghtnsL - brghtnsR) / 2.0), -255, 255); // this "2.0" affects curve steepness, should be a param.
        int brDif = constrain(((brghtnsL - brghtnsR) / curveSteepness), -255, 255); // this "2.0" affects curve steepness, should be a param.

        if (brDif > 0) { // means light is on the LEFT. so left wheel should be turn less.
          coefficient[0] = brDif;  // less power for LEFT
          coefficient[1] = 0;      // full power for RIGHT
        }
        else if (brDif < 0) {
          coefficient[0] = 0;          // full power for LEFT
          coefficient[1] = abs(brDif); // less power for RIGHT, minus change to plus.
        }
        analogWrite(MTR_A_PWM, constrain((modulatedMotorSpeed - coefficient[0]), 0, 255));
        analogWrite(MTR_B_PWM, constrain((modulatedMotorSpeed - coefficient[1]), 0, 255));
      } 
      else {
        analogWrite(MTR_A_PWM, modulatedMotorSpeed);
        analogWrite(MTR_B_PWM, modulatedMotorSpeed);
      }
    }

    // truning around while laser is on
    // noforwardmove && !laserScan is no move.
    if (noForwardMove && !laserScan) {
      analogWrite(MTR_A_PWM, 0);
      analogWrite(MTR_B_PWM, 0);
    }
    else if (laserScan) {
      if (bangFlag_LSR) {
        // turn
        if (turnDirection) {
          digitalWrite(MTR_B_F, LOW);
          digitalWrite(MTR_B_B, HIGH);
        }
        else if (!turnDirection) {
          digitalWrite(MTR_A_F, LOW);
          digitalWrite(MTR_A_B, HIGH);
        }
      }
      // finish turning when laser blink has finished
      if (millis() > timeStamp_LSR + reactionLength_LSR && turnFlag == true) {
        // digitalWrite(LASER, LOW);
        if (turnDirection) {
          digitalWrite(MTR_B_F, HIGH);
          digitalWrite(MTR_B_B, LOW);
          turnFlag = false;
        }
        else if (!turnDirection) {
          digitalWrite(MTR_A_F, HIGH);
          digitalWrite(MTR_A_B, LOW);
          turnFlag = false;
        }
        if (int(random(50)) % 2 == 1) turnDirection = true;
        else turnDirection = false;
        // Serial.println(turnDirection);
      }
    }
  }
  else if (!playFlag && lastPlayFlag) {
    analogWrite(MTR_A_PWM, 0);
    analogWrite(MTR_B_PWM, 0);
    // Serial.println("motor off");
  }

  // else if (!followLightFlag || !rotateReactFlag) {
  //   analogWrite(MTR_A_PWM, 0);
  //   analogWrite(MTR_B_PWM, 0);
  // }

  lastPlayFlag = playFlag;
}
