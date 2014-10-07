Write a c library to interface with X11. (Xlib)
X11 Functions:
"XGetImage"
"XGetPixel"
"XImage" structure
"XMoveResizeWindow"
XQueryTree (page 53 of manual)

Can I just send the window to a screen that doesn't actually exist and interact with it there? Or use the second desktop?

Maybe send the mouse clicks through X as well?
Do the whole thing in c? Not ideal...

Look at the concept of "pixmaps". probably these wont be helpful actually.

Helpful links:

Use X11:
http://tronche.com/gui/x/xlib/
http://tronche.com/gui/x/xlib/graphics/XGetImage.html
Example of using this method? http://www.roard.com/docs/cookbook/cbsu19.html
Example 2: http://stackoverflow.com/questions/17518610/how-to-get-a-screen-pixels-color-in-x11
http://stackoverflow.com/questions/6063329/screenshot-using-opengl-and-or-x11
http://unix.stackexchange.com/questions/122875/how-to-take-a-screenshot-of-an-totally-obscured-window
http://www.cplusplus.com/forum/beginner/63367/
http://stackoverflow.com/questions/2703640/process-list-on-linux-via-python

"xwinfo" terminal utility.