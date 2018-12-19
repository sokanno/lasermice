void cp5setup() {
  cp5 = new ControlP5(this);
  cp5.addSlider("blinkTime")
    .setPosition(20, 20)
    .setSize(100, 20)
    .setRange(1, 200)
    .setValue(blinkTime);
  cp5.addSlider("loopTime")
    .setPosition(20, 50)
    .setSize(100, 20)
    .setRange(100, 4096)
    .setValue(2000);
  cp5.addSlider("laserRange")
    .setPosition(20, 80)
    .setSize(100, 20)
    .setRange(10, 800)
    //.setValue(laserRange);
    .setValue(300);
  cp5.addSlider("motorSpeed")
    .setPosition(20, 110)
    .setSize(100, 20)
    .setRange(0.1, 0.5)
    .setValue(motorSpeed);        
  cp5.addSlider("row")
    .setPosition(20, 200)
    .setSize(100, 20)
    .setRange(1, 15)
    .setValue(row);
  cp5.addSlider("column")
    .setPosition(20, 230)
    .setSize(100, 20)
    .setRange(1, 15)
    .setValue(column);
  cp5.addButton("Start")
    .setPosition(20, height-50)
    .setSize(100, 20);
}
