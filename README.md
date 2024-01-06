# Innoprojekt_22

Code for Innoprojekt competition at ETHZ.

The main goal of this project was to create a user-friendly, smooth experience to encourage young teens interest in engineering.
(The folder structure as-is does not yet inspire confidence, but it's still a work in progress)

## ip22 code for our plotter device

Usage:

- Windows installation of python through ´.zip´. NOTE: requireas inkscape as a dependency,
- Drivers for the the Arduino in the folder ´/PlotterV5´. Include drivers as dependency when compiling the python generated plotter instructions.


## What currently works

The drivers as provided are an accurate enough control system for simple drawings. For more complex shapes, there is significant drift even with pre-calibration and a PID controller with more advanced hardware would be required.


## Future
_THIS PROJECT WILL BE UNDER SLOW DEVELOPMENT AS THE OFFICIAL COMPETITION CONCLUDED IN 2022_

As it stands at the moment, the roadmap and planned features are as follows

- Upgrade motors to servo or stepper
- Update the Bluetooth connection protocol and allow for non-hardcoded plots to be drawn
- Implement simple PID control (if necessary)
- Unify code style
- Increase type-safety and speed (because that is _obviously_ very necessary) by rewriting in Rust