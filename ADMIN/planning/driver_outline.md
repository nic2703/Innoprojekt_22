
# Outline
## Sketch
1. plotter header
    - all macros
    - const data   
    - global drawing fn declarations (these are global so no object needs to be constructed for individual tests and use)
    - plotter class woth iterator, init and private data and metadata on position, shape, number of moves, current uptime, etc

2. driver cpp main
    - implement all global drawing fns
    - implement plotter class
        - particularly, iterator to move through list of instructions. these are then updated by the iterator. these could be single instructions or a vec of some length
        - data on position, relative speeds and so on updates with iteration
    - implement servo use

3. plotter maths
    - contains all linalg and trig functionality
    - specifically vector class
    - time to dist and dist to time conversions
    - bitspeed to irl speed conversions and opposite


--------------------------

## Indepth

`; all indepth discussion as comments should begin with a semicolon and in code formatting`

1. time calculations will be done by using the speed mapping instead of the old method of time to rotations to rpm calculation, which we cannot tell how reliable that is.\
the hope is with this method, that the number of intermediate steps between distance to the resulting time should be fewer and thus easier to debug. a further simplification is that on could make it stepwise, though this would likely sacrifice quite a bit of accuracy

2. 