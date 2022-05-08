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

  delay(200)
}

void up()
{
  for (; agl >= 135; --agl)
  {
    servo.write(agl);
    delay(15);
  }

  delay(200)
}

void setup()
{
  servo.attach(_SERVO);

  up();

  up();

  up();
  delay(200);
  plot.draw_line(85, 211);
  down();
  delay(200);
  plot.draw_line(-48, 156);
  delay(200);
  plot.draw_line(150, 0);
  delay(200);
  plot.draw_line(11, -49);
  delay(200);
  plot.draw_line(-75, 0);
  delay(200);
  plot.draw_line(0, -11);
  delay(200);
  plot.draw_line(64, 0);
  delay(200);
  plot.draw_line(11, -37);
  delay(200);
  plot.draw_line(-64, 0);
  delay(200);
  plot.draw_line(5, -16);
  delay(200);
  plot.draw_line(113, 0);
  delay(200);
  plot.draw_line(-32, 113);
  delay(200);
  plot.draw_line(70, 0);
  delay(200);
  plot.draw_line(32, -113);
  delay(200);
  plot.draw_line(49, 0);
  delay(200);
  plot.draw_line(-32, 113);
  delay(200);
  plot.draw_line(69, 0);
  delay(200);
  plot.draw_line(16, -59);
  delay(200);
  plot.draw_line(22, 0);
  delay(200);
  plot.draw_line(-16, 59);
  delay(200);
  plot.draw_line(69, 0);
  delay(200);
  plot.draw_line(43, -156);
  delay(200);
  plot.draw_line(-70, 0);
  delay(200);
  plot.draw_line(-15, 59);
  delay(200);
  plot.draw_line(-22, 0);
  delay(200);
  plot.draw_line(16, -59);
  delay(200);
  plot.draw_line(-365, 0);
  up();
}

void loop() {}
