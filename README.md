# Graphics Programming
A fun graphics repository that uses `C` as programming language with OpenGL and GLUT. The project is divided into multiple programs, each demonstrating a different concept.

## Prerequisites
- OpenGL and GLUT libraries (install according to your OS)
- C compiler (gcc or clang)

## Build and run the programs
```bash
$ gcc -framework OpenGL -framework GLUT -framework Cocoa -DGL_SILENCE_DEPRECATION <program_name>.c -o <program_name>

$ ./<program_name>
```
### For example to build and run the rotating_hexagon.c program:
```bash
$ gcc -framework OpenGL -framework GLUT -framework Cocoa -DGL_SILENCE_DEPRECATION rotating_hexagon.c -o rotating_hexagon

$ ./rotating_hexagon
```
