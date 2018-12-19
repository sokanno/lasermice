/* 
 LASERBUG SIMULATOR by So KANNO
 05.11.2014
 
 this is a simulation of new idea of installation named "laserbug".
 inspired by swarm intelligence.
 
 algorhithm of each instance is based in mechanical structures.
 */

//sound
import processing.sound.*;
WhiteNoise noise;
Env env;
float attackTime = 0.001;
float sustainTime = 0.002;
float sustainLevel = 0.01;
float releaseTime = 0.01;
float volume = 0.2;

int count = 0;

//for control parameters
import controlP5.*;
ControlP5 cp5;
int blinkTime = 50;

int loopTime = 3000;
float range = 1.0;
float motorSpeed = 0.3;
boolean startFlag = false;
int laserRange = 300;

//to make a report file in csv
PrintWriter output;
long reportTime = 1 * 60 * 1000;
int detectCount = 0;
int totalLT = 0;
String date;

ArrayList<Rotator> rots;
//number and size of modules
int row = 5;
int column = 7;
int moduleSize = 5; //half diamiter

// turnAmount is angle that rotater turns when hit other module or wall.
// from science of leaf angle of "5/13".
float turnAmount = 2.416; 


void setup() {
  println(loopTime);
  size(1100, 500);
  cp5setup();
  date = year() + nf(month(), 2) + nf(day(), 2) + "-"  + nf(hour(), 2) + nf(minute(), 2) + ".csv"; 
  noise = new WhiteNoise(this);
  env  = new Env(this);
  println(laserRange);
}

void draw() {
  smooth();
  ellipseMode(RADIUS);
  background(30);

  if (startFlag) {
    for (Rotator r : rots) {
      r.react(rots);
      r.collision(rots);
      r.selfblinking();
      r.edgeDetection();
      r.movement();
      r.update(motorSpeed, loopTime, blinkTime, laserRange);
    }
  } else {
    rots = new ArrayList<Rotator>();
    // calculate the interval
    float xInterval = width / column + 1;
    float yInterval = height / row + 1;
    for (int i=0; i<row; i++) {
      for (int j=0; j<column; j++) {
        float xpos = (j+0.5)*xInterval;
        float ypos = (i+0.5)*yInterval;
        float modSpeed = motorSpeed;
        int eachloopTime = loopTime;
        int shiftTime = int(random(0, loopTime));
        rots.add(new Rotator(xpos, ypos, modSpeed, eachloopTime, shiftTime, blinkTime, laserRange));
      }
    }
    for (Rotator r : rots) {
      r.update(motorSpeed, loopTime, blinkTime, laserRange);
    }
  }
  fill(200);
  text(frameRate, 20, height-40);

  if (millis() > reportTime) {
    //    report();
  }
}

// for debug
void keyPressed() {
  // println("new loop");
  if (key == 'm') {
    background(30);
    for (Rotator r : rots) {
      r.react(rots);
      r.collision(rots);
      r.selfblinking();
      r.edgeDetection();
      r.movement();
      r.update(motorSpeed, loopTime, blinkTime, laserRange);
    }
  }
}

public void Start(int theValue) {
  startFlag = !startFlag;
}

void blinkTimeSlider(int blinkTime) {
  for (Rotator r : rots) {
    r.blinkTime = blinkTime;
  }
}

// void loopTimeSlider(int loopTime){
//   for (Rotator r : rots) {
//     r.loopTime = int(random(loopTime*range, loopTime));
//   }
// }

// void rangeSlider(int range){
//   for (Rotator r : rots) {
//     r.loopTime = int(random(loopTime*range, loopTime));
//   }
// }
