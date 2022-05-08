#include <PlotterV3.h>
#include <Servo.h>

Plt plot = Plt();
Servo servo;

int agl = 170;

void down()
{
  for (; agl <= 160; ++agl)
  {
    servo.write(agl);
    delay(15);
  }

  delay(200);
}

void up()
{
  for (; agl >= 135; --agl)
  {
    servo.write(agl);
    delay(15);
  }

  delay(200);
}

void panic()
{
  plot.draw_line(0,0);
  
  Serial.write("Panic");
  
  exit(1);
}

void setup()
{
  Serial.begin(9600);
  servo.attach(_SERVO);

  up();
  plot.calibrate();
  
  down();
  plot.draw_line(1000, 1000);
  up();
}

void loop() {}
