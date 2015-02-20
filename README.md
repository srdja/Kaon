# Kaon - Core
Cross platform scriptable game engine

### Project dependencies:

- cmake >= version 3.0
- gcc / MinGW
- SDL2
- SDL image
- SDL ttf
- glew

###Building for a specific architecture:

By default cmake builds for the 64bit architecture. It can however be instructed to build
for the 32bit arch by passing the `-DARCH:STRING=32` argument.

###Building on windows
If building on windows with mingw the path to the mingw library must be explicitly specified
by passing the 
