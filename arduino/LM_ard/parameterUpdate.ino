void parameterUpdate() {
  switch (op) {
    case 0: // ==
      if (id == myId) correspond = true;
      else {
        correspond = false;
        irCommandFlag = false;
      }
      break;
    case 1: // !=
      if (id != myId) correspond = true;
      else {
        correspond = false;
        irCommandFlag = false;
      }
      break;
    case 2: // <
      if (id < myId) correspond = true;
      else {
        correspond = false;
        irCommandFlag = false;
      }
      break;
    case 3: // >
      if (id > myId) correspond = true;
      else {
        correspond = false;
        irCommandFlag = false;
      }
      break;
      // default:
  }

  if (correspond) {
    switch (address) {
      case 0:
        if (value < 4096) cycleLength = value;
        if (!sequenceMode) reactionLength_LSR = int(cycleLength * laser_reactionLength_ratio);
        else if (sequenceMode) reactionLength_LSR = int(cycleLength * laser_reactionLength_ratio / 2);
        irCommandFlag = false;
        irNotification = true;
        break;
      case 1:
        laser_reactionLength_ratio = value / 100.0;
        if (!sequenceMode) reactionLength_LSR = int(cycleLength * laser_reactionLength_ratio);
        else if (sequenceMode) reactionLength_LSR = int(cycleLength * laser_reactionLength_ratio / 2);
        irCommandFlag = false;
        irNotification = true;
        break;
      case 2:
        motorSpeed = value;
        irCommandFlag = false;
        irNotification = true;
        break;
      case 3:
        if (value > 0 && value < 1024) threshold = value;
        irCommandFlag = false;
        irNotification = true;
        break;
      case 4:
        if (value > (cycleLength / 4)) value = cycleLength / 4;
        reactionLength_SLND = value;
        irCommandFlag = false;
        irNotification = true;
        break;
      case 5:
        reactTimes = value;
        reactCount = reactTimes;
        //        sequence = value;
        //converting value to array
        //        for (int i = 0; i < 16; i++) {
        //          beat[15 - i] = (value >> i) % 2;
        //        }
        irCommandFlag = false;
        irNotification = true;
        break;
      case 6:
        loopDigit = value;
        curveSteepness = value / 4.0;
        irCommandFlag = false;
        irNotification = true;
        break;
      case 7:
        switch (value) {
          case 1: // play
            playFlag = true;
            goToChargeFlag = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 2: // stop
            playFlag = false;
            goToChargeFlag = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 3: // go to charge
            goToChargeFlag = true;
            playFlag = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 4: // save
            //EEPROM saving function
            /*
              int cycleLength_add_0 = 0;
              int cycleLength_add_1 = 1;
              int laser_reactionLength_ratio_add = 2;
              int motorSpeed_add = 3;
              int threshold_add = 4;
              int reactionLength_SLND_add = 5;
            */

            // EEPROM.write(cycleLength_add_0, cycleLength >> 8);
            // EEPROM.write(cycleLength_add_1, cycleLength - ((cycleLength >> 8) << 8));
            // EEPROM.write(laser_reactionLength_ratio_add, int(laser_reactionLength_ratio * 100));
            // EEPROM.write(motorSpeed_add, motorSpeed);
            // EEPROM.write(threshold_add, threshold);
            // EEPROM.write(reactionLength_SLND_add, reactionLength_SLND);

            irCommandFlag = false;
            irNotification = true;
            break;
          case 5: // sequence mode on
            sequenceMode = true;
            reactionLength_LSR = int(cycleLength * laser_reactionLength_ratio / 2);
            bangFlag = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 6: // sequence mode off
            sequenceMode = false;
            reactionLength_LSR = int(cycleLength * laser_reactionLength_ratio);
            bangFlag = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 7: // sequence count reset
            seqCount = 0;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 8: // cue
            timeStamp_LSR = millis();
            bangFlag_LSR = true;
            bangFlag_SLND = true;
            shiftAmount = millis() % cycleLength;
            bangFlag = true;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 9: // debug
            //                Serial.print("cycleLength");
            //                Serial.print("         ");
            //                Serial.println(cycleLength);
            //                Serial.print("reactionLength_LSR");
            //                Serial.print("  ");
            //                Serial.println(reactionLength_LSR);
            //                Serial.print("motorSpeed");
            //                Serial.print("          ");
            //                Serial.println(motorSpeed);
            //                Serial.print("threshold");
            //                Serial.print("           ");
            //                Serial.println(threshold);
            //                Serial.print("reactionLength_SLND");
            //                Serial.print(' ');
            //                Serial.println(reactionLength_SLND);
            //                Serial.print("sequenceMode");
            //                Serial.print("        ");
            //                Serial.println(sequenceMode);
            //                Serial.print("loopDigit");
            //                Serial.print("           ");
            //                Serial.println(loopDigit);
            irCommandFlag = false;
            irNotification = true;
            break;
          case 10: // tail LED on
            tailLED = true;
            //            rotateReactFlag = true;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 11: // tail LED off
            tailLED = false;
            //            rotateReactFlag = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 12: // IR receiving on
            irReceive = true;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 13: // IR receiving off
            irReceive = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 14: // Laser Keep on
            keepLaser = true;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 15: // Laser normal mode
            keepLaser = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 16: // Three Sensors Mode activate
            threeSensorsMode = true;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 17: // Three Sensors Mode deactivate
            threeSensorsMode = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 18: // detectReport activate
            detectReport = true;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 19: // detectReport deactivate
            detectReport = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 20: // commandReaction activate
            commandReaction = true;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 21: // commandReaction deactivate
            commandReaction = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 22: // speedModulation activate
            speedModulation = true;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 23: // speedModulation deactivate
            speedModulation = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 24: // lightFollow activate
            lightFollow = true;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 25: // lightFollow deactivate
            lightFollow = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 26: // laserScan activate
            laserScan = true;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 27: // laserScan deactivate
            laserScan = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 28: // noForwardMove activate
            noForwardMove = true;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 29: // noForwardMove deactivate
            noForwardMove = false;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 30: // noForwardMove activate
            reactor = true;
            irCommandFlag = false;
            irNotification = true;
            break;
          case 31: // noForwardMove deactivate
            reactor = false;
            irCommandFlag = false;
            irNotification = true;
            break;
        }
        break;
    }
  }

  //send params back to ESP to make a report
  // Serial.write();
  // Serial.write();
  // Serial.write();
  // Serial.write();
}
