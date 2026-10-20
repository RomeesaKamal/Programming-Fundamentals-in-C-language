#ifndef MENU_H
#define MENU_H

#include <gtk/gtk.h>

#define MENU_COUNT 10

typedef struct {
    int id;
    char name[30];
    int price;
} MenuItem;

extern MenuItem menu_items[MENU_COUNT];

void create_main_window(GtkApplication *app, gpointer user_data);

#endif
