all : pyToC.so xlib_utils.o

# The libraries being linked to must come after the .c file for gcc to handle it correctly.
pyToC.so : pyToC.c xlib_utils.h xlib_utils.o
	gcc pyToC.c -o pyToC.so xlib_utils.o -shared -fPIC -I/usr/include/python3.4m/ -lX11 -lpython3.4m

xlib_utils.o : xlib_utils.c
	gcc xlib_utils.c -c -fPIC

clean :
	rm -f *.so
	rm -f *.o
