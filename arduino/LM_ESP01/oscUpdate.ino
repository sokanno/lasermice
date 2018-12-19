void oscUpdate() {
  OSCMessage msg;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      if (msg.match("/c")) {
        msg.dispatch("/c", response);
      }
      else if (msg.match("/i")) {
        debug();
      }
    } else {
      error = msg.getError();
      //      Serial.print("error: ");
      //      Serial.println(error);
    }
  }
}

void response(OSCMessage &msg) {
  OSCMessage rspns("/r");
  rspns.add(id);
  rspns.add(msg.getInt(1));
  rspns.add(msg.getInt(2));
  rspns.add(msg.getInt(3));
  Udp.beginPacket(outIp, outPort);
  rspns.send(Udp);
  Udp.endPacket();
  rspns.empty();
  Serial.write(msg.getInt(0));
  Serial.write(msg.getInt(1));
  Serial.write(msg.getInt(2));
  Serial.write(msg.getInt(3));
}

void debug() {
//   OSCMessage dbg("/d");
//   dbg.add(id);
//   dbg.add("all the variables would come");
//   Udp.beginPacket(outIp, outPort);
//   dbg.send(Udp);
//   Udp.endPacket();
//   dbg.empty();
   Serial.write(B00000000); // empty signal can be byte(0)
   Serial.write(B00100000); // means debug! can be byte(32)
}
