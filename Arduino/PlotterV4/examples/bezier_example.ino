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
    delayMicroseconds(1500);
  }

  delayMicroseconds(10000);
}

void up()
{
  for (; agl >= 130; --agl)
  {
    servo.write(agl);
    delayMicroseconds(1500);
  }

  delayMicroseconds(10000);
}

void print_pos()
{
  Serial.print(plot.pos_x()); Serial.print(", "); Serial.println(plot.pos_y());
}

void setup()
{
  Serial.begin(9600);
  servo.attach(_SERVO);

  up();
  plot.calibrate();


  up();
  delay(1000);
  plot.draw_line(2000, 2000);
  delay(500);
  down();
  delay(500);

  plot.draw_line(0, -500);
  delay(500);
  plot.draw_line(-500, 0);
  delay(500);
  plot.draw_line(0, 500);
  delay(500);
  plot.draw_line(500, 0);
  delay(500);
  plot.bezier_c(2000, 1500, 1500, 1500, 1500, 2000, 40);
  delay(500);
  up();
}

void loop() {}