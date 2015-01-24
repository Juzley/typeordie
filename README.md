# typeordie
A typing game written in C++ with SDL.

# Build
Dependencies: SDL2, SDL2-mixer, Boost (headers only), GL, GLM (headers only),
    C++11 standard library and capable compiler.
Once the dependencies have been satisfied, run 'make' from the root directory.

# Running
Run the typeordie executable from within the 'bin' directory.
The following commandline options are accepted:
--width or -w <width>: Set the horizontal resolution.
--height or -h <height>: Set the vertical resolution.
--fullscreen or -f: Run in fullscreen mode.
--text-scale or -t <scale-factor>: Scale the in-game phrase text by the
specified amount.
