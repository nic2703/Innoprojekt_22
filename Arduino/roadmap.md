## ROADMAP
FOR ALL FURTHER CODE IN THIS PROJECT
---------------------------------------

All maths functions should be contained in the maths/linalg header files. in particular, vectors and other tools needed for drawing. this is already implemented fairly well and should be maintained.

The plotter should be a class with an iterator. The plotter stores the current shape it's drawing (pre-calculated or runtime, really doesnt matter). this can be first set with a call to a draw curve fn. 

in the main loop, we call the plotter iterator and that calls necessary updates in the various fns.