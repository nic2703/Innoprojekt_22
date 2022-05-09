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
  for (; agl >= 130; --agl)
  {
    servo.write(agl);
    delay(15);
  }

  delay(200);
}

void setup()
{
  servo.attach(_SERVO);

  up();
  plot.calibrate();


  up();
  delay(200);
  plot.draw_line(491, 84);
  down();
  delay(200);
  plot.draw_line(-49, 0);
  delay(200);
  plot.draw_line(17, -84);
  delay(200);
  plot.draw_line(-411, 0);
  delay(200);
  plot.draw_line(-43, 217);
  delay(200);
  plot.draw_line(164, 0);
  delay(200);
  plot.draw_line(11, -56);
  delay(200);
  plot.draw_line(-97, 0);
  delay(200);
  plot.draw_line(6, -28);
  delay(200);
  plot.draw_line(97, 0);
  delay(200);
  plot.draw_line(10, -49);
  delay(200);
  plot.draw_line(-97, 0);
  delay(200);
  plot.draw_line(6, -28);
  delay(200);
  plot.draw_line(154, 0);
  delay(200);
  plot.draw_line(-32, 161);
  delay(200);
  plot.draw_line(67, 0);
  delay(200);
  plot.draw_line(32, -161);
  delay(200);
  plot.draw_line(56, 0);
  delay(200);
  plot.draw_line(-32, 161);
  delay(200);
  plot.draw_line(67, 0);
  delay(200);
  plot.draw_line(17, -84);
  delay(200);
  plot.draw_line(49, 0);
  delay(200);
  plot.draw_line(-17, 84);
  delay(200);
  plot.draw_line(67, 0);
  delay(200);
  plot.draw_line(43, -217);
  delay(200);
  plot.draw_line(-67, 0);
  delay(200);
  plot.draw_line(-17, 84);
  up();

}

void loop() {}
