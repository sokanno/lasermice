void espUpdate(){
  if(Serial1.available() > 0){
    byte data[4]; 
    Serial1.readBytes(data, 4);

    id = data[0];
    identifier = data[1] >> 5;
    op = data[1] >> 3;
    address = data[1] - ((op << 3)+(identifier << 5));
    value = (data[2] << 8) + data[3];

    if (identifier == 0){
    	parameterUpdate();    
    }
    else if(identifier == 1){
    	//send all the parameters 
      /*
      important memo 
      bit length of all params
      myId:                       8bit
      playFlag:                   1bit
      cycleLength:                12bit
      laser_reactionLength_ratio: 7bit
      motorSpeed:                 8bit
      threshold:                  7bit
      reactionLength_SLND         7bit
      sequence                    16bit
      loopDigit                   4bit
      sequenceMode                1bit
      followLightFlag             1bit
      battery                     8bit
      time in sec                 16bit
      
      Tail on/off                 1bit
      IR communication on/off     1bit
      Laser On Mode               1bit

      3 variable is 7bit 
      3 variable is 1bit 
      those can be combined like below
      "playFlag" with "laser_reactionLength_ratio"
      "sequenceMode" with "threshold"
      "followLightFlag" with "reactionLength_SLND"

      Order is following:

      0:  identifier (2)                                  [2bit]
      1:  myId                                            [8bit]
      2:  loopDigit + cycleLength (upper bit)             [8bit]
      3:  cycleLength (lower bit)                         [8bit]
      4:  playFlag & laser_reactionLength_ratio           [8bit]  
      5:  motorSpeed                                      [8bit]
      6:  sequenceMode & threshold                        [8bit]
      7:  followLightFlag & reactionLength_SLND           [8bit]
      8:  sequence (upper bit)                            [8bit]
      9:  sequence (lower bit)                            [8bit]
      10: battery                                         [8bit]
      11: time in sec (upper bit)                         [8bit]
      12: time in sec (lower bit)                         [8bit]
      13: Tail & IR & LsrOn & spdMod & lf & scan & nofwd  [7bit]

      */
      Serial1.write(byte(2));
      Serial1.write(byte(myId));
      int cub = cycleLength >> 8;
      Serial1.write(byte((loopDigit << 4) + cub));
      Serial1.write(byte(cycleLength - (cub << 8)));
      Serial1.write(byte((playFlag << 7)+(laser_reactionLength_ratio * 100)));
      Serial1.write(byte(motorSpeed));
      Serial1.write(byte((sequenceMode << 7) + threshold));
      Serial1.write(byte((goToChargeFlag << 7) + reactionLength_SLND));
      int sub = sequence >> 8;
      Serial1.write(byte(sub));
      Serial1.write(byte(sequence - (sub << 8)));
      Serial1.write(byte(analogRead(BAT_CHK)/4));      
      int t = int(millis() / 1000);
      int tlb = t >> 8;
      Serial1.write(byte(tlb));
      Serial1.write(byte(t - (tlb << 8)));
      Serial1.write(byte((tailLED << 6) + (irReceive << 5) + (keepLaser << 4) + 
                         (speedModulation << 3) + (lightFollow << 2) + (laserScan << 1) + 
                         (noForwardMove))); 
      }
    }
    else if(identifier == 2){
      myId = id;
      // Serial.println(myId);
    }
  }


// should have battery check
