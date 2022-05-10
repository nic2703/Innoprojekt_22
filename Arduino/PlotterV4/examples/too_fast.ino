#include <PlotterV3.h>
#include <Servo.h>

Plt plot = Plt();

Servo servo;

int agl = 160;

void down()
{
  for (; agl <= 160; ++agl)
  {
    servo.write(agl);
    delayMicroseconds(100*15);
  }
  delayMicroseconds(100*1000);
}

void up()
{
  for (; agl >= 130; --agl) // modify agl, now 120
  {
    servo.write(agl);
    delayMicroseconds(100*15);
  }
  delayMicroseconds(100*1000);
}


void setup()
{
  servo.attach(_SERVO);

  up();
  plot.calibrate();


  up();
  delayMicroseconds(100*200);
  plot.draw_line(1545, 937);
  down();
  delayMicroseconds(100*200);
  plot.draw_line(116, 0);
  delayMicroseconds(100*200);
  plot.draw_line(0, 129);
  delayMicroseconds(100*200);
  plot.bezier_q(1713, 990, 1771, 953);
  delayMicroseconds(100*200);
  plot.bezier_q(1829, 914, 1891, 914);
  delayMicroseconds(100*200);
  plot.bezier_q(1939, 914, 1992, 944);
  delayMicroseconds(100*200);
  plot.draw_line(-59, 96);
  delayMicroseconds(100*200);
  plot.bezier_q(1897, 1025, 1873, 1025);
  delayMicroseconds(100*200);
  plot.bezier_q(1816, 1025, 1763, 1072);
  delayMicroseconds(100*200);
  plot.bezier_q(1710, 1119, 1682, 1217);
  delayMicroseconds(100*200);
  plot.bezier_q(1661, 1293, 1661, 1523);
  delayMicroseconds(100*200);
  plot.draw_line(0, 299);
  delayMicroseconds(100*200);
  plot.draw_line(-116, 0);
  delayMicroseconds(100*200);
  plot.draw_line(0, -885);
  up();

  up();
  delayMicroseconds(100*200);
  plot.draw_line(399, -115);
  down();
  delayMicroseconds(100*200);
  plot.draw_line(53, 0);
  delayMicroseconds(100*200);
  plot.draw_line(18, -49);
  delayMicroseconds(100*200);
  plot.bezier_c(2031, 769, 2046, 764, 2060, 755);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.draw_line(48, 23);
  delayMicroseconds(100*200);
  plot.draw_line(37, -38);
  delayMicroseconds(100*200);
  plot.draw_line(-23, -48);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.bezier_c(2131, 679, 2136, 664, 2140, 647);
  delayMicroseconds(100*200);
  plot.draw_line(49, -18);
  delayMicroseconds(100*200);
  plot.draw_line(0, -54);
  delayMicroseconds(100*200);
  plot.draw_line(-49, -18);
  delayMicroseconds(100*200);
  plot.bezier_c(2136, 542, 2130, 526, 2122, 512);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.draw_line(23, -48);
  delayMicroseconds(100*200);
  plot.draw_line(-37, -38);
  delayMicroseconds(100*200);
  plot.draw_line(-48, 23);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.bezier_c(2046, 441, 2031, 432, 2014, 428);
  delayMicroseconds(100*200);
  plot.draw_line(-18, -52);
  delayMicroseconds(100*200);
  plot.draw_line(-53, 0);
  delayMicroseconds(100*200);
  plot.draw_line(-18, 52);
  delayMicroseconds(100*200);
  plot.bezier_c(1910, 432, 1895, 440, 1881, 448);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.draw_line(-48, -23);
  delayMicroseconds(100*200);
  plot.draw_line(-37, 38);
  delayMicroseconds(100*200);
  plot.draw_line(23, 48);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.bezier_c(1810, 526, 1802, 542, 1798, 558);
  delayMicroseconds(100*200);
  plot.draw_line(-51, 18);
  delayMicroseconds(100*200);
  plot.draw_line(0, 54);
  delayMicroseconds(100*200);
  plot.draw_line(51, 18);
  delayMicroseconds(100*200);
  plot.bezier_c(1802, 664, 1809, 679, 1818, 693);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.draw_line(-22, 48);
  delayMicroseconds(100*200);
  plot.draw_line(38, 38);
  delayMicroseconds(100*200);
  plot.draw_line(48, -23);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.bezier_c(1895, 765, 1910, 769, 1926, 774);
  delayMicroseconds(100*200);
  plot.draw_line(18, 49);
  up();

  up();
  delayMicroseconds(100*200);
  plot.draw_line(-62, -220);
  down();
  delayMicroseconds(100*200);
  plot.bezier_c(1882, 553, 1922, 513, 1970, 513);
  delayMicroseconds(100*200);
  plot.bezier_c(2019, 513, 2059, 553, 2059, 603);
  delayMicroseconds(100*200);
  plot.bezier_c(2059, 652, 2019, 692, 1970, 692);
  delayMicroseconds(100*200);
  plot.bezier_c(1922, 692, 1882, 652, 1882, 603);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  up();

  up();
  delayMicroseconds(100*200);
  plot.draw_line(403, 416);
  down();
  delayMicroseconds(100*200);
  plot.draw_line(31, -13);
  delayMicroseconds(100*200);
  plot.draw_line(-2, -33);
  delayMicroseconds(100*200);
  plot.bezier_c(2323, 966, 2331, 959, 2337, 950);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.draw_line(33, 2);
  delayMicroseconds(100*200);
  plot.draw_line(13, -31);
  delayMicroseconds(100*200);
  plot.draw_line(-25, -22);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.bezier_c(2360, 888, 2360, 878, 2358, 867);
  delayMicroseconds(100*200);
  plot.draw_line(25, -22);
  delayMicroseconds(100*200);
  plot.draw_line(-13, -31);
  delayMicroseconds(100*200);
  plot.draw_line(-33, 2);
  delayMicroseconds(100*200);
  plot.bezier_c(2331, 807, 2323, 800, 2315, 794);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.draw_line(2, -33);
  delayMicroseconds(100*200);
  plot.draw_line(-31, -13);
  delayMicroseconds(100*200);
  plot.draw_line(-22, 25);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.bezier_c(2253, 771, 2242, 770, 2232, 772);
  delayMicroseconds(100*200);
  plot.draw_line(-22, -25);
  delayMicroseconds(100*200);
  plot.draw_line(-31, 13);
  delayMicroseconds(100*200);
  plot.draw_line(2, 34);
  delayMicroseconds(100*200);
  plot.bezier_c(2171, 800, 2164, 807, 2158, 816);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.draw_line(-33, -2);
  delayMicroseconds(100*200);
  plot.draw_line(-13, 34);
  delayMicroseconds(100*200);
  plot.draw_line(25, 25);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.bezier_c(2135, 880, 2135, 891, 2136, 902);
  delayMicroseconds(100*200);
  plot.draw_line(-25, 21);
  delayMicroseconds(100*200);
  plot.draw_line(13, 30);
  delayMicroseconds(100*200);
  plot.draw_line(33, -2);
  delayMicroseconds(100*200);
  plot.bezier_c(2164, 959, 2172, 966, 2180, 973);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.draw_line(-2, 33);
  delayMicroseconds(100*200);
  plot.draw_line(31, 13);
  delayMicroseconds(100*200);
  plot.draw_line(22, -25);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  delayMicroseconds(100*200);
  plot.bezier_c(2242, 995, 2252, 995, 2263, 994);
  delayMicroseconds(100*200);
  plot.draw_line(22, 25);
  up();

  up();
  delayMicroseconds(100*200);
  plot.draw_line(-90, -114);
  down();
  delayMicroseconds(100*200);
  plot.bezier_c(2184, 876, 2197, 843, 2226, 831);
  delayMicroseconds(100*200);
  plot.bezier_c(2255, 819, 2287, 833, 2299, 862);
  delayMicroseconds(100*200);
  plot.bezier_c(2311, 890, 2297, 923, 2269, 935);
  delayMicroseconds(100*200);
  plot.bezier_c(2240, 947, 2207, 933, 2196, 904);
  delayMicroseconds(100*200);
  plot.draw_line(0, 0);
  up();
}

void loop()
{
  //Do Nothing
}