#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Window window = NULL;
Display *display = NULL;
XImage* image = NULL;

int findWindow(Window w, char* name);
int setupWindow(char* name);
int positionWindow();
void printScreenshotOfWindow(Window w);

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
    return 0;
}

int updateScreenshot()
{
    if(!window)
    {
       printf("You need to call \"setupWindow\" first to find the desired window.\n");
       fflush(stdout);
       return -1;
    }
    else
    {
        XWindowAttributes gwa;
        XGetWindowAttributes(display, window, &gwa);
        int width = gwa.width;
        int height = gwa.height;

        image = XGetImage(display, window, 0, 0, width, height, AllPlanes, ZPixmap);
        printf("Width: %d. Height: %d .\n", image->width, image->height);
        fflush(stdout);
        printScreenshotOfWindow(window);
        return 0;
    }
}

char* getGrid()
{
    if(!image)
    {
       printf("You need to call \"updateScreenshot\" first.\n");
       fflush(stdout);
       return "error";
    }

    unsigned long redMask = image->red_mask;
    unsigned long greenMask = image->green_mask;
    unsigned long blueMask = image->blue_mask;
    printf("Red: %lu. Green: %lu. Blue: %lu.\n", (redMask >> 16), (greenMask >> 8), blueMask);
    // Above values seem to always be 255.
    
    XWindowAttributes gwa;
    XGetWindowAttributes(display, window, &gwa);
    int width = gwa.width;
    int height = gwa.height;
    //Loop through each pixel for now to test the image. Later just pick ones I care about.
    int row, col = 0;
    for(row; row < height; row++)
    {
        for(col; col < width; col++)
        {
            unsigned long pix = XGetPixel(image, col, row);

        }
    }
    return NULL;
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

//  printf("Window: %x, ", (int)w);
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
    XFree(childWindows);
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

void printScreenshotOfWindow(Window win)
{
    // For now it creates filenames based on window's id
    XWindowAttributes gwa;
    XGetWindowAttributes(display, win, &gwa);
    int width = gwa.width;
    int height = gwa.height;
    //printf("Width: %d. Height: %d .\n", width, height);
    //printf("Map State: %d\n", gwa.map_state);
    //fflush(stdout);
    // 2 means it is viewable. Fix this so it's not a magic number
    if(gwa.map_state != 2 || width < 500 || height < 500)
    {
        return;
    }

    XImage* image = XGetImage(display, win, 0, 0, width, height, AllPlanes, ZPixmap);

    unsigned long redMask = image->red_mask;
    unsigned long greenMask = image->green_mask;
    unsigned long blueMask = image->blue_mask;
    // Above values seem to always be 255.
    
    //For now use the .ppm image format for simplicity.
    char *pre = "img_";
    char *post = ".ppm";
    char name[100];
    snprintf(name, sizeof(name), "%s%d%s", pre, (int)win, post);
    printf("Filename: %s\n", name);
    fflush(stdout);

    FILE* outFile = fopen(name, "wb");
    fprintf(outFile, "P6\n%d %d\n255\n", width, height);         //output the header information.
    //Loop through each pixel and add it to the file.
    int row = 0;
    int col = 0;
    for(row; row < height; row++)
    {
        //fprintf(outFile, "\n");
        col = 0;
        for(col; col < width; col++)
        {
            unsigned long pix = XGetPixel(image, col, row);
            char red = (pix & redMask) >> 16;
            char green = (pix & greenMask) >> 8;
            char blue = (pix & blueMask);
            fprintf(outFile, "%c%c%c", red, green, blue);
        }
    }
    fclose(outFile);
    return;
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

int getRootScreenshot()
{
    //This function still needs work. The root window doesn't seem to actually get a screenshot
    //of the desktop, but it's complicated due to virtual desktops etc.
    // For now this function prints the screenshot
    printf("Getting root screenshot. \n");
    printf("Num screens: %d\n", XScreenCount(display));
    fflush(stdout);
    
    Window root = XRootWindow(display, 0);
    printf("Root id: %d\n", (int)root);
    XWindowAttributes gwa;
    XGetWindowAttributes(display, root, &gwa);
    int width = gwa.width;
    int height = gwa.height;

    XImage* image = XGetImage(display, root, 0, 0, width, height, AllPlanes, ZPixmap);
    printf("Width: %d. Height: %d .\n", image->width, image->height);
    fflush(stdout);

    unsigned long redMask = image->red_mask;
    unsigned long greenMask = image->green_mask;
    unsigned long blueMask = image->blue_mask;
    printf("Red: %lu. Green: %lu. Blue: %lu.\n", (redMask >> 16), (greenMask >> 8), blueMask);
    // Above values seem to always be 255.
    
    //For now use the .ppm image format for simplicity.
    FILE* outFile = fopen("image.ppm", "wb");
    fprintf(outFile, "P6\n%d %d\n255\n", width, height);         //output the header information.
    //Loop through each pixel and add it to the file.
    int row = 0;
    int col = 0;
    for(row; row < height; row++)
    {
        //fprintf(outFile, "\n");
        col = 0;
        for(col; col < width; col++)
        {
            unsigned long pix = XGetPixel(image, col, row);
            char red = (pix & redMask) >> 16;
            char green = (pix & greenMask) >> 8;
            char blue = (pix & blueMask);
            fprintf(outFile, "%c%c%c", red, green, blue);
        }
    }
    fclose(outFile);
    return 0;
}
