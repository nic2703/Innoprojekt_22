from svg.path import parse_path
from svg.path.path import *
from xml.dom import minidom
from os import system, getcwd
from sys import argv as args
from time import sleep
from tqdm import tqdm

pa = ''
s = 0

if '-s' in args:
    if args.index('-s') != -1:
        s = float(args[args.index('-s')+1])
if '-p' in args:
    if args.index('-p') != -1:
        pa = args[args.index('-p')+1]

if not pa:
    pa = input("Enter path to SVG: ")
if not s:
    s = float(input("Enter scaling: "))

SCALING = s

del s

init = """#include <PlotterV3.h>
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
"""

conc = """}

void loop() {}"""

move_template = """
    up();
    delay(200);
    plot.draw_line(%d, %d);
    down();
"""

delay_template = "\tdelay(200);\n"

line_template = "\tplot.draw_line(%d, %d);\n"

cubic_bezier_template = "\tplot.bezier_c(%d, %d, %d, %d, %d, %d);\n"

quadratic_bezier_template = "\tplot.bezier_q(%d, %d, %d, %d);\n"


print("Converting to path")
system('inkscape -g --verb="EditSelectAll;SelectionUnGroup;EditSelectAll;SelectionUnGroup;EditSelectAll;SelectionUnGroup;EditSelectAll;SelectionUnGroup;EditSelectAll;ObjectRemoveTransform;EditSelectAll;SelectionGroup" --batch-process --export-text-to-path --export-plain-svg --export-filename={}\\out.svg '.format(getcwd()) + getcwd() + "\\" + pa)
print("Done converting\n")
sleep(1)

print("Fetching path")
# read the SVG file
doc = minidom.parse(getcwd() + "\\" 'out.svg')

sleep(0.4)
print("Trying with <path>")
path_strings = [path.getAttribute('d') for path
                in doc.getElementsByTagName('path')] #try getting it by the path tag

if not path_strings:
    print("No <path> found")
    print("Trying with <svg:path")
    path_strings = [path.getAttribute('d') for path
                in doc.getElementsByTagName('svg:path')] #try getting it by the svg:path tag

if not path_strings:
    print("Fatal error, no path found in svg")
    quit()

print("Path found")
print("Closing file\n")
doc.unlink()

x = 0
y = 0

print("Generating .ino file")
with open(getcwd() + "\\" + "generated_ino.ino", "w") as ino:
    print(".ino file generated, initialising file")
    ino.write(init) #initialise the file
    
    print("Generating code")
    sleep(0.8)

    for path_string in path_strings:
        p = parse_path(path_string) #parser go brrrr

        for e in tqdm(p):

            sleep(0.001)
            if isinstance(e, Move):
                #move to a given position
                ino.write(move_template % ( round((e.end.real-x)*SCALING), round((e.end.imag-y)*SCALING) ))

            elif isinstance(e, Line):
                #from absolute to relative, ende minus anfang
                ino.write(delay_template + line_template % ( round((e.end.real-e.start.real)*SCALING), round((e.end.imag-e.start.imag)*SCALING )) )

            elif isinstance(e, CubicBezier):
                #cubic bezier
                ino.write(delay_template + cubic_bezier_template % ( round(e.control1.real*SCALING), round(e.control1.imag*SCALING), round(e.control2.real*SCALING), round(e.control2.imag*SCALING), round(e.end.real*SCALING), round(e.end.imag*SCALING)) )

            elif isinstance(e, QuadraticBezier):
                #quadratic bezier
                ino.write(delay_template + quadratic_bezier_template % (round(e.control.real*SCALING ), round(e.control.imag*SCALING), round(e.end.real*SCALING), round(e.end.imag*SCALING) ) )

            elif isinstance(e, Close):
                #Closing the shape
                ino.write(delay_template + line_template % ( round((e.end.real-e.start.real)*SCALING), round((e.end.imag-e.start.imag)*SCALING )))

                ino.write("\tup();\n")

            else: 
                #linearly approximate everything you do not know
                ino.write(delay_template + line_template % ( round((e.end.real-e.start.real)*SCALING), round((e.end.imag-e.start.imag)*SCALING )))

                print("Unknown instance:")
                print(e)
            
            #update coords
            x = e.end.real
            y = e.end.imag
        
        sleep(0.3)
    
    print("Finalising file")
    ino.write(conc) #finalise the file

print("\nConversion complete, .ino file can be found here: {}".format(getcwd() + "\\" + "generated_ino.ino"))