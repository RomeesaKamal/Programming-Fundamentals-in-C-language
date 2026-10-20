#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

/* =========================
   Global Widgets
========================= */

GtkWidget *name_entry;
GtkWidget *age_entry;
GtkWidget *gender_combo;
GtkWidget *disease_entry;
GtkWidget *doctor_entry;
GtkWidget *phone_entry;
GtkWidget *address_entry;
GtkWidget *room_entry;
GtkWidget *status_label;

/* =========================
   Add Patient Function
========================= */

void add_patient(GtkWidget *widget, gpointer data)
{
    FILE *fp;

    const char *name;
    const char *age;
    gchar *gender;
    const char *disease;
    const char *doctor;
    const char *phone;
    const char *address;
    const char *room;

    name = gtk_entry_get_text(GTK_ENTRY(name_entry));
    age = gtk_entry_get_text(GTK_ENTRY(age_entry));

    gender = gtk_combo_box_text_get_active_text(
                 GTK_COMBO_BOX_TEXT(gender_combo));

    disease = gtk_entry_get_text(GTK_ENTRY(disease_entry));
    doctor = gtk_entry_get_text(GTK_ENTRY(doctor_entry));
    phone = gtk_entry_get_text(GTK_ENTRY(phone_entry));
    address = gtk_entry_get_text(GTK_ENTRY(address_entry));
    room = gtk_entry_get_text(GTK_ENTRY(room_entry));

    /* Validation */

    if(strlen(name) == 0 ||
       strlen(age) == 0 ||
       strlen(disease) == 0 ||
       strlen(doctor) == 0)
    {
        gtk_label_set_text(GTK_LABEL(status_label),
                           "Please fill all required fields!");

        g_free(gender);
        return;
    }

    /* Open File */

    fp = fopen("patients.txt", "a");

    if(fp == NULL)
    {
        gtk_label_set_text(GTK_LABEL(status_label),
                           "Error opening file!");

        g_free(gender);
        return;
    }

    /* Store Data */

    fprintf(fp,
            "=====================================\n");

    fprintf(fp, "Patient Name : %s\n", name);
    fprintf(fp, "Age          : %s\n", age);
    fprintf(fp, "Gender       : %s\n", gender);
    fprintf(fp, "Disease      : %s\n", disease);
    fprintf(fp, "Doctor       : %s\n", doctor);
    fprintf(fp, "Phone        : %s\n", phone);
    fprintf(fp, "Address      : %s\n", address);
    fprintf(fp, "Room No      : %s\n", room);

    fprintf(fp,
            "=====================================\n\n");

    fclose(fp);

    gtk_label_set_text(GTK_LABEL(status_label),
                       "Patient Added Successfully!");

    /* Clear Fields */

    gtk_entry_set_text(GTK_ENTRY(name_entry), "");
    gtk_entry_set_text(GTK_ENTRY(age_entry), "");
    gtk_entry_set_text(GTK_ENTRY(disease_entry), "");
    gtk_entry_set_text(GTK_ENTRY(doctor_entry), "");
    gtk_entry_set_text(GTK_ENTRY(phone_entry), "");
    gtk_entry_set_text(GTK_ENTRY(address_entry), "");
    gtk_entry_set_text(GTK_ENTRY(room_entry), "");

    gtk_combo_box_set_active(GTK_COMBO_BOX(gender_combo), 0);

    g_free(gender);
}

/* =========================
   View Patients Function
========================= */

void view_patients(GtkWidget *widget, gpointer data)
{
    FILE *fp;
    char buffer[1024];

    GtkWidget *window;
    GtkWidget *scroll;
    GtkWidget *textview;

    GtkTextBuffer *text_buffer;

    /* Create New Window */

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window),
                         "Patient Records");

    gtk_window_set_default_size(GTK_WINDOW(window),
                                700,
                                500);

    gtk_container_set_border_width(GTK_CONTAINER(window),
                                   10);

    /* Scroll Window */

    scroll = gtk_scrolled_window_new(NULL, NULL);

    gtk_container_add(GTK_CONTAINER(window),
                      scroll);

    /* Text View */

    textview = gtk_text_view_new();

    gtk_text_view_set_editable(GTK_TEXT_VIEW(textview),
                               FALSE);

    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textview),
                                     FALSE);

    gtk_container_add(GTK_CONTAINER(scroll),
                      textview);

    text_buffer = gtk_text_view_get_buffer(
                      GTK_TEXT_VIEW(textview));

    /* Open File */

    fp = fopen("patients.txt", "r");

    if(fp == NULL)
    {
        gtk_text_buffer_set_text(text_buffer,
                                 "No patient records found!",
                                 -1);

        gtk_widget_show_all(window);
        return;
    }

    /* Read File Data */

    GString *content = g_string_new("");

    while(fgets(buffer, sizeof(buffer), fp))
    {
        g_string_append(content, buffer);
    }

    fclose(fp);

    /* Show Data */

    gtk_text_buffer_set_text(text_buffer,
                             content->str,
                             -1);

    g_string_free(content, TRUE);

    gtk_widget_show_all(window);
}

/* =========================
   Main Function
========================= */

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *grid;

    GtkWidget *title;

    GtkWidget *name_label;
    GtkWidget *age_label;
    GtkWidget *gender_label;
    GtkWidget *disease_label;
    GtkWidget *doctor_label;
    GtkWidget *phone_label;
    GtkWidget *address_label;
    GtkWidget *room_label;

    GtkWidget *add_button;
    GtkWidget *view_button;

    gtk_init(&argc, &argv);

    /* Main Window */

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window),
                         "Advanced Hospital Management System");

    gtk_window_set_default_size(GTK_WINDOW(window),
                                900,
                                650);

    gtk_container_set_border_width(GTK_CONTAINER(window),
                                   20);

    g_signal_connect(window,
                     "destroy",
                     G_CALLBACK(gtk_main_quit),
                     NULL);

    /* Grid */

    grid = gtk_grid_new();

    gtk_grid_set_row_spacing(GTK_GRID(grid), 15);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 15);

    gtk_container_add(GTK_CONTAINER(window),
                      grid);

    /* Title */

    title = gtk_label_new(NULL);

    gtk_label_set_markup(GTK_LABEL(title),
                         "<span font='22' weight='bold' foreground='blue'>"
                         "HOSPITAL MANAGEMENT SYSTEM"
                         "</span>");

    gtk_grid_attach(GTK_GRID(grid),
                    title,
                    0,
                    0,
                    2,
                    1);

    /* Patient Name */

    name_label = gtk_label_new("Patient Name:");
    name_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 1, 1, 1);

    /* Age */

    age_label = gtk_label_new("Age:");
    age_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), age_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), age_entry, 1, 2, 1, 1);

    /* Gender */

    gender_label = gtk_label_new("Gender:");

    gender_combo = gtk_combo_box_text_new();

    gtk_combo_box_text_append_text(
        GTK_COMBO_BOX_TEXT(gender_combo),
        "Male");

    gtk_combo_box_text_append_text(
        GTK_COMBO_BOX_TEXT(gender_combo),
        "Female");

    gtk_combo_box_text_append_text(
        GTK_COMBO_BOX_TEXT(gender_combo),
        "Other");

    gtk_combo_box_set_active(
        GTK_COMBO_BOX(gender_combo),
        0);

    gtk_grid_attach(GTK_GRID(grid), gender_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gender_combo, 1, 3, 1, 1);

    /* Disease */

    disease_label = gtk_label_new("Disease:");
    disease_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), disease_label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), disease_entry, 1, 4, 1, 1);

    /* Doctor */

    doctor_label = gtk_label_new("Doctor Name:");
    doctor_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), doctor_label, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), doctor_entry, 1, 5, 1, 1);

    /* Phone */

    phone_label = gtk_label_new("Phone Number:");
    phone_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), phone_label, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), phone_entry, 1, 6, 1, 1);

    /* Address */

    address_label = gtk_label_new("Address:");
    address_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), address_label, 0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), address_entry, 1, 7, 1, 1);

    /* Room Number */

    room_label = gtk_label_new("Room Number:");
    room_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), room_label, 0, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), room_entry, 1, 8, 1, 1);

    /* Add Button */

    add_button = gtk_button_new_with_label("Add Patient");

    g_signal_connect(add_button,
                     "clicked",
                     G_CALLBACK(add_patient),
                     NULL);

    gtk_grid_attach(GTK_GRID(grid),
                    add_button,
                    0,
                    9,
                    1,
                    1);

    /* View Button */

    view_button = gtk_button_new_with_label("View Patients");

    g_signal_connect(view_button,
                     "clicked",
                     G_CALLBACK(view_patients),
                     NULL);

    gtk_grid_attach(GTK_GRID(grid),
                    view_button,
                    1,
                    9,
                    1,
                    1);

    /* Status Label */

    status_label = gtk_label_new("");

    gtk_grid_attach(GTK_GRID(grid),
                    status_label,
                    0,
                    10,
                    2,
                    1);

    /* Show Window */

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}