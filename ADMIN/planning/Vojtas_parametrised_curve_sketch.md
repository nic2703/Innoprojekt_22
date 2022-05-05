# param curve suggestion

u have a parametrised curve as (x(t), y(t)), take the derivative, call it g

then you start at g(x(0), y(0)). you define a dt, time elapsed since the last time the motors were updated, so the next loop you can just do g(x(0+dt), y(0+dt)), now define a variable of time and set time to this new t. now repeat, so g(x(time+dt), y(time+dt)) now you just go all the way through, until you complete the curve.

now get a funciton that brings you to the start of the function
