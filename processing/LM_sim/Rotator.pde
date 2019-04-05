class Rotator {
  PVector location, speeds;
  float rSpeed, mSpeed, rotSize, angle, targetAngle, capAngle, blinkTime, r;
  int detectCount, shift, type, loopTime, defaultTime, laserRange, ledAngle;
  long shiftTime;
  boolean pairing, angleMatch, detectFlag, lastDetectFlag, soundFlag, lightType, beShot;
  String turnMode, lightMode;
  Rotator (float x, float y, float rs, int lt, int dt, float bt, int rr) {
    location = new PVector(x, y);
    speeds = new PVector(rs/5, rs);
    // rSpeed = rs;
    // mSpeed = ms;
    rotSize = moduleSize;
    angle = random(0, TWO_PI);
    detectCount = 0;
    shift = 0;
    angleMatch = false;
    turnMode = "OFF";
    loopTime = lt;
    shiftTime = dt;
    soundFlag = true;
    blinkTime = bt;
    laserRange = rr;
    //lightType = lm;
    ledAngle = 5; //for both direction, 10 degree in total.
    beShot = false;
  }

  // detecting other rotator's laser
  void selfblinking() {
    if ((millis() + shiftTime) % loopTime < blinkTime) {
      lightMode = "ON";
      if (r > 0.5) angle += speeds.x;
      else angle -= speeds.x;
      if (soundFlag) {
        float xpos = map(location.x, 0, width, -1, 1);
        xpos = constrain(xpos, -1, 1);
        soundFlag = false;
        r = random(0, 1);

        noise.play();
        noise.amp(volume);
        noise.pan(xpos);
        env.play(noise, attackTime, sustainTime, sustainLevel, releaseTime);
        count++;
      }
      // println(lightMode);
    } else {
      lightMode = "OFF";
      soundFlag = true;
      beShot = false;
    }
  }


  void react(ArrayList<Rotator> rots) {
    for (Rotator other : rots) {
      // calculate the distance
      if (location != other.location) { // this line is important to ignore itself
        float distance = PVector.dist(other.location, location);
        // calculate the tolerance from distance and rotSize
        float tolerance = atan2(rotSize, distance);
        // calculate the angle of two rotators
        float collideAngle = atan2(other.location.y - location.y, 
          other.location.x - location.x);
        capAngle = collideAngle;
        collideAngle = collideAngle + PI;  // because atan2 value is -PI ~ PI      
        // compare the angle of other's laser and itself
        float diff = abs(collideAngle - other.angle);
        // if(other.detectCount > 1 || other.turnMode != "OFF"){ 
        if (other.lightMode == "ON") { 
          // when it's on the corner, light is on
          // if(other.detectCount > 1){ 
          // if (diff < tolerance || abs(diff - TWO_PI) < tolerance) {
          if (diff < tolerance || abs(diff - TWO_PI) < tolerance && distance < laserRange) {
            detectCount ++;
            shift = shift + 5; 
            detectFlag = true;
            // if (detectFlag && !lastDetectFlag && lightMode == "OFF") {
            if (detectFlag && !lastDetectFlag) {
              //most important line of code for SYNC
              shiftTime = shiftTime + (loopTime - ((millis() + shiftTime) % loopTime));
              lastDetectFlag = true;
              beShot = true;
            }
          } else {
            detectFlag = false;
            if (!detectFlag && lastDetectFlag) {
              lastDetectFlag = false;
            }
          }
        }
      }
    }
  }

  // detecting collision
  void collision(ArrayList<Rotator> rots) {
    for (Rotator other : rots) {
      if (location != other.location) { // to ignore itself
        float d = PVector.dist(other.location, location); // calculate the distance to other modules
        if (d < rotSize*2) {
          // calculate the angle of two mods
          float collideAngle = atan2(location.y - other.location.y, 
            location.x - other.location.x);
          collideAngle = collideAngle + PI;
          if (turnMode == "OFF") {
            if (angle > PI) {
              if (angle - collideAngle < PI && angle - collideAngle > 0) {
                turnMode = "CW";
                targetAngle = angle + turnAmount;
              } else {
                turnMode = "CCW";
                targetAngle = angle - turnAmount;
              }
            } else if (angle < PI) {
              if (collideAngle - angle < PI && collideAngle - angle > 0) {
                turnMode = "CCW";
                targetAngle = angle - turnAmount;
              } else {
                turnMode = "CW";
                targetAngle = angle + turnAmount;
              }
            }
          }
        }
      }
    }
  }

  void edgeDetection() {
    // reflection detect for X axis end
    if (turnMode == "OFF") {
      // for right end
      if (location.x > (width-(rotSize))) {
        if (angle < TWO_PI && angle > PI*3/2) { // angle is down
          turnMode = "CCW";
          targetAngle = angle - turnAmount;
        } else if (angle > 0 && angle < HALF_PI) { // angle is up
          turnMode = "CW";
          targetAngle = angle + turnAmount;
        }
      }
      //for left end
      else if (location.x < (rotSize)) {
        if (angle > PI) { // angle is up
          turnMode = "CW";
          targetAngle = angle + turnAmount;
        } else if (angle < PI) { // angle is down
          turnMode = "CCW";
          targetAngle = angle - turnAmount;
        }
      }
      // reflection detect for Y axis end
      // for bottom end
      if (location.y > (height-(rotSize))) {
        // print("bot", angle, ' ');
        if (angle > (HALF_PI)) { // angle is right
          turnMode = "CW";
          targetAngle = angle + turnAmount;
          // println(turnMode);
        } else if (angle < (HALF_PI)) { // angle is left
          turnMode = "CCW";
          targetAngle = angle - turnAmount;
          // println(turnMode);
        }
      }
      //for top end
      else if (location.y < (rotSize)) {
        // print("top", angle, ' ');
        if (angle > PI+HALF_PI) { // angle is left
          turnMode = "CW";
          targetAngle = angle + turnAmount;
          // println(turnMode);
        } else if (angle < PI+HALF_PI) { // angle is right
          turnMode = "CCW";
          targetAngle = angle - turnAmount;
          // println(turnMode);
        }
      }
      // println(angle, " ", targetAngle);
    }
  }

  void movement() { // modules turn or rotate
    // rotate and change direction
    if (turnMode != "OFF") {
      if (turnMode == "CW") {
        angle += speeds.x;
        // rotation finish
        if (angle > targetAngle) {
          if (angle < 0) angle = angle + TWO_PI;
          if (angle > TWO_PI) angle = angle - TWO_PI;

          // move a little to prevent detect end again
          location.x = location.x + cos(angle)*speeds.y*2;
          location.y = location.y + sin(angle)*speeds.y*2;
          turnMode = "OFF";
        }
      } else if (turnMode == "CCW") {
        angle -= speeds.x;
        //rotation finish
        if (angle < targetAngle) {
          if (angle < 0) angle = angle + TWO_PI;
          if (angle > TWO_PI) angle = angle - TWO_PI;

          // move a little to prevent detect end again
          location.x = location.x + cos(angle)*speeds.y*2;
          location.y = location.y + sin(angle)*speeds.y*2;
          turnMode = "OFF";
        }
      }
    } else {
      location.x = location.x + cos(angle)*speeds.y;
      location.y = location.y + sin(angle)*speeds.y;
    }
  }

  // drawing things
  void update(float ms, int lt, int bt, int lr) {
    loopTime = lt;
    blinkTime = bt;
    laserRange = lr;
    //lightType = type;
    speeds.x = ms/5;
    speeds.y = ms;
    // change color to fill depend on it's detecting or not.
    if (lightMode == "ON")  fill(0, 200, 0);  
    else fill(150);
    // else fill(loopTime / 10);

    //drawing Modules
    strokeWeight(1);
    ellipseMode(RADIUS);
    stroke(100, 100, 100);
    ellipse(location.x, location.y, rotSize, rotSize);
    line(location.x, location.y, location.x + cos(angle)*rotSize, location.y + sin(angle)*rotSize);

    //drawing lasers
    stroke(0, 255, 0); // green

    strokeWeight(2);
    // if(detectCount != 0 || turnMode != "OFF" || lightMode == "ON"){ // when it's on the corner, light is on
    if (lightMode == "ON") { // when it's on the corner, light is on
      // if(detectCount != 0){
      // line(location.x + cos(angle)*rotSize, location.y + sin(angle)*rotSize, 
      // location.x + cos(angle)*rotSize*300, location.y + sin(angle)*rotSize*300);

      // laser with limit
      for (int i = 0; i<laserRange; i++) {
        if (beShot) {
          stroke(255, 0, 0, 255-i*255/laserRange);//laser is red when it is shot.
          //stroke(0, 255, 0, 255-i*255/laserRange);
        } else {
          stroke(0, 255, 0, 255-i*255/laserRange);
        }
        point(location.x + cos(angle)*i, location.y + sin(angle)*i);
      } 
      // long laser 
      //line(location.x + cos(angle)*rotSize, location.y + sin(angle)*rotSize, 
      //location.x + cos(angle)*rotSize*300, location.y + sin(angle)*rotSize*300);
    }
    // reset detect count when finish to draw
    detectCount = 0;
  }
}
