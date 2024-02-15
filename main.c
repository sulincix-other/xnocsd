#define _GNU_SOURCE
#include <stdio.h>
#include <X11/Xlib.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdbool.h>

// Function pointer for the real XChangeProperty
typedef int (*XChangeProperty_t)(Display*, Window, Atom, Atom, int, int, const unsigned char*, int);

// Real XChangeProperty function
static XChangeProperty_t real_XChangeProperty = NULL;

// Replacement XChangeProperty function
int XChangeProperty(Display *display, Window w, Atom property, Atom type,
                    int format, int mode, const unsigned char *data, int nelements) {
    int prop = XInternAtom(display, "_MOTIF_WM_HINTS", False);
    if (property == prop && getenv("CSD") == NULL) {
        return 0;  // Block the change by returning 0
    }
    // Call the real XChangeProperty function
    if (!real_XChangeProperty) {
        real_XChangeProperty = (XChangeProperty_t)dlsym(RTLD_NEXT, "XChangeProperty");
    }

    return real_XChangeProperty(display, w, property, type, format, mode, data, nelements);
}

// Function pointer for the real XGetSelectionOwner
typedef Window (*XGetSelectionOwner_t)(Display*, Atom);

// Real XGetSelectionOwner function
static XGetSelectionOwner_t real_XGetSelectionOwner = NULL;

Window XGetSelectionOwner(Display *dpy, Atom selection){
    char prop_name[20];
    snprintf(prop_name, 20, "_NET_WM_CM_S%d", XDefaultScreen(dpy));
    Atom prop_atom = XInternAtom(dpy, prop_name, False);
    if (prop_atom == selection && getenv("CSD") == NULL){
        return (Window)NULL;
    }
    // Call the real XChangeProperty function
    if (!real_XGetSelectionOwner) {
        real_XGetSelectionOwner = (XGetSelectionOwner_t)dlsym(RTLD_NEXT, "XGetSelectionOwner");
    }
    return real_XGetSelectionOwner(dpy, selection);

}

// Block gtk shadows for gliches
bool gdk_screen_is_composited(){
    return false;
}

bool gdk_display_is_composited(){
    return false;
}

typedef void (*gtk_header_bar_set_show_title_buttons_t)(void* header, bool settings);
typedef void (*gtk_header_bar_set_show_close_button_t)(void* header, bool settings);
typedef void (*adw_header_bar_set_show_start_title_buttons_t)(void* header, bool settings);


// block window control show (gtk4)
static gtk_header_bar_set_show_title_buttons_t real_gtk_header_bar_set_show_title_buttons = NULL;
void gtk_header_bar_set_show_title_buttons(void* header, bool settings){
    if(!real_gtk_header_bar_set_show_title_buttons){
        real_gtk_header_bar_set_show_title_buttons = 
            (gtk_header_bar_set_show_title_buttons_t)dlsym(RTLD_NEXT, "gtk_header_bar_set_show_title_buttons");
    }
    real_gtk_header_bar_set_show_title_buttons(header, false);
}

// block window control show (libadwaita)
static adw_header_bar_set_show_start_title_buttons_t real_adw_header_bar_set_show_start_title_buttons = NULL;
void adw_header_bar_set_show_start_title_buttons(void* header, bool settings){
    if(!real_adw_header_bar_set_show_start_title_buttons){
        real_adw_header_bar_set_show_start_title_buttons = 
            (adw_header_bar_set_show_start_title_buttons_t)dlsym(RTLD_NEXT, "adw_header_bar_set_show_start_title_buttons");
    }
    real_adw_header_bar_set_show_start_title_buttons(header, false);
}
// block window control show (gtk3)
static gtk_header_bar_set_show_close_button_t real_gtk_header_bar_set_show_close_button = NULL;
void gtk_header_bar_set_show_close_button(void* header, bool settings){
    if(!real_gtk_header_bar_set_show_close_button){
        real_gtk_header_bar_set_show_close_button = 
                        (gtk_header_bar_set_show_close_button_t)dlsym(RTLD_NEXT, "gtk_header_bar_set_show_close_button");
    }
    real_gtk_header_bar_set_show_close_button(header, false);
}
