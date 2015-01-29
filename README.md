# typeordie
A typing game written in C++ with SDL.

# Build
Dependencies: SDL2, SDL2-mixer, Boost + Boost program options, GL, GLM (headers only),
    C++11 standard library and capable compiler.
Once the dependencies have been satisfied, run 'make' from the root directory.

On a recent unbuntu:
  sudo apt-get install libsdl2-dev libsdl2-mixer-dev libboost-dev libboost-program-options-dev libglm-dev
  make
  
For windows, the Nugen MinGW distro (http://nuwen.net/mingw.html) comes packaged with all the required libraries.

# Running
Run the typeordie executable from within the 'bin' directory.
The following commandline options are accepted:
--width or -w <width>: Set the horizontal resolution.
--height or -h <height>: Set the vertical resolution.
--fullscreen or -f: Run in fullscreen mode.
--text-scale or -t <scale-factor>: Scale the in-game phrase text by the
specified amount.
