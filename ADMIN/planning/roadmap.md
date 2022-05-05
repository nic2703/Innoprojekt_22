# ROADMAP

## FOR ALL FURTHER CODE IN THIS PROJECT

Unfortunately, it seems our code (2.0) is very messy again! let's use what we have and improve it.
we had a lot of good ideas in 1.0 and we have had a lot more in 2.0. we can meld these to make the best code so far.

It's not worth creating an entirely new version of the code, but we should continue with 2.1. this is an update on the overhaul we did just a bit ago.

All maths functions should be contained in the maths/linalg header files. in particular, vectors and other tools needed for drawing. this is already implemented fairly well and should be maintained.

The plotter should be a class with an iterator. The plotter stores the current shape it's drawing (pre-calculated or runtime, really doesnt matter). this can be first set with a call to a draw curve fn.

in the main loop, we call the plotter iterator and that calls necessary updates in the various fns.

//UPDATE: 2022/05/05

these rules are now outdated\
however, apart from the iterator, we implemented most of this stuff, so i'm happy
