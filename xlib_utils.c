#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Window window = NULL;
Display *display = NULL;

int findWindow(Window w, char* name);
int setupWindow(char* name);
int positionWindow();


int setupWindow(char* name)
{
    display = XOpenDisplay(NULL);
    if(!display)
    {
        printf("No display found.\n");
        fflush(stdout);
        return -1;
    }

    Window rootWin = XRootWindow(display, 0);
    if(!rootWin)
    {
       printf("Unable to find root window.\n");
       fflush(stdout);
       return;
    }
    findWindow(rootWin, name);
    if(!window)
    {
       printf("A window with this name doesn't exist.\n");
       fflush(stdout);
       return -1;
    }

    positionWindow(name);
    if(!window)
    {
       printf("Window could not be positioned correctly.\n");
       fflush(stdout);
       return -1;
    }

    getScreenshot();
    return 0;
}

// Recursive function to find a window with the given name that is a child of w.
int findWindow(Window w, char* name)
{
    //Loop through each window until a match is found.
    Window newRoot, newParent;
    unsigned int numChildren; 
    Window *childWindows;

    if(!XQueryTree(display, w, &newRoot, &newParent, &childWindows, &numChildren))
    {
       printf("Error querying the window tree.\n");
       fflush(stdout);
       return 1;
    }

    char *winName;
    if(XFetchName(display, w, &winName))
    {
        if(strncmp(name, winName, 50) == 0)
        {
            //The first one we find is the main window.
            if(!window)
            {
                printf("Window: %x, ", (int)w);
                printf("Name: %s\n", winName);
                fflush(stdout);
                window = w;
                XFree(childWindows);
                return 0;
            }
            else
            {
                return 1;
            }
        }
    }

    int i=0;
    for(i; i<numChildren; i++)
    {
        findWindow(childWindows[i], name);
    }

    //At this point we haven't found the window, so it doesn't exist
    return 1;
}

int positionWindow()
{
    //Possibly move the window to the second desktop and maximize it?


    // Bring the window to the front and make it take up most of the screen
    XRaiseWindow(display, window);
    XMoveResizeWindow(display, window, 10, 35, 1500, 800);
    XFlush(display);
    // Sleep a bit to give the animation time to complete
    sleep(1);
    return 0;
}

int getScreenshot()
{
    if(!window)
    {
       printf("You need to call \"setupWindow\" first to find the desired window.\n");
       fflush(stdout);
       return -1;
    }
    else
    {
        // Use the "XGetImage" function here.
        XWindowAttributes gwa;
        XGetWindowAttributes(display, window, &gwa);
        int width = gwa.width;
        int height = gwa.height;

        XImage* image = XGetImage(display, window, 0, 0, width, height, AllPlanes, ZPixmap);
        printf("Width: %d. Height: %d .\n", image->width, image->height);
        fflush(stdout);

        return 0;
    }

}

int getRootScreenshot()
{
    // Use the "XGetImage" function here.
    Window root = DefaultRootWindow(display);
    XWindowAttributes gwa;
    XGetWindowAttributes(display, root, &gwa);
    int width = gwa.width;
    int height = gwa.height;

    XImage* image = XGetImage(display, root, 0, 0, width, height, AllPlanes, ZPixmap);
    printf("Width: %d. Height: %d .\n", image->width, image->height);
    fflush(stdout);

    return 0;

}

///////////////////////////////////// Test Functions /////////////////////////

int testPerformance()
{
    int index = 0;
    for(index; index<40; index++)
    {
        long num = 10000000;
        long sum = 0; 
        int i=0;
        for(i; i<num; i++)
        {
            sum += i;
        }
    }
    //printf("Sum: %ld", sum);
    return 0;
}

int displayTestWindow()
{
    printf("Displaying a test window.\n");
    fflush(stdout);

    Display *display = XOpenDisplay(NULL);
    if(!display)
    {
        printf("No display found.\n");
        fflush(stdout);
        return -1;
    }
    else
    {
        Window w = XCreateWindow(display, DefaultRootWindow(display), 0, 0, 200,
                                 100, 0, CopyFromParent, CopyFromParent,
                                 CopyFromParent, 0, 0);
        XMapWindow(display, w);
        XFlush(display);
        sleep(10);
    }
    return 0;
}
