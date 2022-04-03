The SVGParser is meant to take the code behind an SVG image and turn it into a legible series of coordinates and draw commands.

svgparsing1.cpp
How to import SVG:
- Take SVG image, open in VSCode
- Copy text into text file (In this case "Phi.txt")
- Run the program
- Switch between cout and fout

The svgparsing1.cpp program reads the strings of the document until it reaches "<path>". The program will proceed to read character for character, and will search for the opening bracket "\"". From there, it uses a switch statement to search for statements.

The program will look for coordinate pairs, delimited by a comma or minus sign. For coordinate pairs and drawing commands, the output will be put on new lines. Appended is a small summary of some draw commands often found in SVG files:

"M": Start move, move the pen to a location before drawing
"Z": End move, lift the pen after drawing
"C" or "S": Cubic Bezier curve, uses coordinate pair in front of it and the three behind. (In some rare cases, I have seen C commands with only two coordinate pairs behind it, suggesting it readjusts to quadratic beziers)
"Q" or "T": Quadratic Bezier curve
"V": Draws a vertical line to a location
"H": Draws a horizontal line to a location
"L": Draws a line to a coordinate

TODO/ERRORS:
- Write to csv
- Include full list of SVG draw commands (Ellipse, Circle, etc...)
- Offload different work to different functions (current everything is a nested if-statement mess)
- Automatic idenfication of repeated coordinate pairs, better output
- Output directly to a python file
- Reading directly from an SVG
- Accounting for letters/interpolating Bezier curves from letters
- Multiple path statement

DANGER:
Different SVG files use different delimiters between coordinate pairs. Generally, between x and y in a pair, it always a comma. 
Between pairs, it can be:
 - A comma
 - A minus sign (minus for the next number)
 - Literally just a space