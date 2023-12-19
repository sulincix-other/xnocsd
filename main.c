#define _GNU_SOURCE
#include <stdio.h>
#include <X11/Xlib.h>
#include <dlfcn.h>
#include <stdlib.h>

// Function pointer for the real XChangeProperty
typedef int (*XChangeProperty_t)(Display*, Window, Atom, Atom, int, int, const unsigned char*, int);

// Real XChangeProperty function
static XChangeProperty_t real_XChangeProperty = NULL;

// Replacement XChangeProperty function
int XChangeProperty(Display *display, Window w, Atom property, Atom type,
                    int format, int mode, const unsigned char *data, int nelements) {
    int prop = XInternAtom(display, "_MOTIF_WM_HINTS", False);
    if (property == prop ) {
        return 0;  // Block the change by returning 0
    }
    // Call the real XChangeProperty function
    if (!real_XChangeProperty) {
        real_XChangeProperty = (XChangeProperty_t)dlsym(RTLD_NEXT, "XChangeProperty");
    }

    return real_XChangeProperty(display, w, property, type, format, mode, data, nelements);
}
