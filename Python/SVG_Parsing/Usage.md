# How to use

Place a Valid `.svg` file in the same folder as the `.exe` or `.py` file.

Double-Click the `.exe` or `.py` file to run it.

It will prompt you to enter the path to the svg, enter the name of the svg file, example: `MySvg.svg`.
If the svg is in a different location on your disk, you can also enter the relative path to the svg, example: `./../../../MySvgFolder/Folder2/MySvg.svg`. this however can become very tedious very quickly, so we advise you to simply place the svg in the same directory as the `.exe file`

It will then prompt you to enter a scaling factor, this can be any positive floating point number, for example: `1`, `1.23423`, or even `47538342592372309328325.7789463209285747353429873459` (not advised). We find it typically works best with numbers between `1` and `10` (we are usually also too lazy to wite the decimal point).

The Program will take a few seconds to compute the `.ino` file that can be used to draw the specified svg using our plotter.\
The generated `.ino` file will be called `generated_ino.ino`.

Both the exe and python can be run from the command line with the following flags:
|Flag|Meaning|
|----|-------|
|`-s`|Scaling Factor|
|`-p`|Relative Path to SVG|
