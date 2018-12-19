void serialUpdate() {
  if (Serial.available() > 0) {
    Serial.readBytes(data, 14);

    identifier = int(data[0]);
    // 0 = update
    // 1 = response
    // 2 = debug
    // 3 = id inquiery
    
    switch (identifier) {
      case 0: // update
        {
          OSCMessage msg("/u");
          for (int i = 1; i < 6; i++) {
            msg.add(int(data[i]));
          }
          Udp.beginPacket(outIp, outPort);
          msg.send(Udp);
          Udp.endPacket();
          msg.empty();

          //try to send to superCollider
          // it worked but ratency is so long like 1 sec
          // OSCMessage smsg("/laserMice");
          // smsg.add(int(data[1]));
          // Udp.beginPacket(outIp, superCollider);
          // smsg.send(Udp);
          // Udp.endPacket();
          // smsg.empty();
        }
        break;
      case 1: // response
        // {
        //   OSCMessage msg("/r");
        //   for (int i = 0; i < 14; i++) {
        //     msg.add(int(data[i]));
        //   }
        //   Udp.beginPacket(outIp, outPort);
        //   msg.send(Udp);
        //   Udp.endPacket();
        //   msg.empty();
        // }
        break;
      case 2: // debug
        {
          OSCMessage msg("/d");
          for (int i = 1; i < 14; i++) {
            msg.add(int(data[i]));
          }
          Udp.beginPacket(outIp, outPort);
          msg.send(Udp);
          Udp.endPacket();
          msg.empty();
        }
        break;
      case 3:
        {
          Serial.write(id);
          Serial.write(B01000000); // means this message is ID info
        }
    }
  }
}

