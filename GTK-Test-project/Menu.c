#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "Menu.h"

MenuItem menu_items[MENU_COUNT] = {
    {1,  "Burger",    350},
    {2,  "Pizza",     550},
    {3,  "Pasta",     420},
    {4,  "Salad",     280},
    {5,  "Soda",      120},
    {6,  "Fries",     200},
    {7,  "Ice Cream", 180},
    {8,  "Coffee",    150},
    {9,  "Tea",       100},
    {10, "Juice",     160}
};

/* ── State ── */
static int selected[MENU_COUNT];   /* 1 = selected, 0 = not */
static GtkWidget *total_label;
static GtkWidget *status_bar;

/* ── CSS ── */
static const char *css =
    "window {"
    "  background-color: #1a0a00;"
    "}"
    ".header-box {"
    "  background: linear-gradient(135deg, #2d0a00 0%, #5c1a00 50%, #2d0a00 100%);"
    "  border-bottom: 3px solid #ff6b00;"
    "  padding: 18px 24px;"
    "}"
    ".logo-label {"
    "  color: #ff6b00;"
    "  font-size: 42px;"
    "  font-weight: bold;"
    "}"
    ".tagline-label {"
    "  color: #ffaa66;"
    "  font-size: 13px;"
    "  letter-spacing: 4px;"
    "}"
    ".restaurant-name {"
    "  color: #ffffff;"
    "  font-size: 28px;"
    "  font-weight: bold;"
    "}"
    ".col-header {"
    "  color: #ff6b00;"
    "  font-size: 13px;"
    "  font-weight: bold;"
    "  letter-spacing: 2px;"
    "  padding: 10px 16px;"
    "  background-color: #2d1500;"
    "  border-bottom: 2px solid #ff6b00;"
    "}"
    ".row-box {"
    "  background-color: #1e0d00;"
    "  border-bottom: 1px solid #3d1a00;"
    "  padding: 10px 0px;"
    "}"
    ".row-box:hover {"
    "  background-color: #2d1500;"
    "}"
    ".id-label {"
    "  color: #ff6b00;"
    "  font-size: 14px;"
    "  font-weight: bold;"
    "  min-width: 50px;"
    "  padding-left: 20px;"
    "}"
    ".name-label {"
    "  color: #ffffff;"
    "  font-size: 15px;"
    "  min-width: 160px;"
    "}"
    ".price-label {"
    "  color: #ffaa66;"
    "  font-size: 14px;"
    "  min-width: 100px;"
    "}"
    ".select-btn {"
    "  background: #2d1500;"
    "  color: #ff6b00;"
    "  border: 1px solid #ff6b00;"
    "  border-radius: 4px;"
    "  font-size: 12px;"
    "  font-weight: bold;"
    "  padding: 5px 16px;"
    "  min-width: 90px;"
    "}"
    ".select-btn:hover {"
    "  background: #ff6b00;"
    "  color: #1a0a00;"
    "}"
    ".select-btn.selected {"
    "  background: #ff6b00;"
    "  color: #1a0a00;"
    "  border: 1px solid #ffaa66;"
    "}"
    ".order-btn {"
    "  background: linear-gradient(135deg, #ff6b00, #cc4400);"
    "  color: #ffffff;"
    "  border: none;"
    "  border-radius: 6px;"
    "  font-size: 16px;"
    "  font-weight: bold;"
    "  padding: 14px 40px;"
    "  letter-spacing: 1px;"
    "}"
    ".order-btn:hover {"
    "  background: linear-gradient(135deg, #ff8800, #ff6b00);"
    "}"
    ".total-label {"
    "  color: #ff6b00;"
    "  font-size: 20px;"
    "  font-weight: bold;"
    "}"
    ".footer-box {"
    "  background-color: #2d0a00;"
    "  border-top: 2px solid #ff6b00;"
    "  padding: 16px 24px;"
    "}"
    ".status-label {"
    "  color: #ffaa66;"
    "  font-size: 13px;"
    "  font-style: italic;"
    "}";

/* ── Helpers ── */
static void recalculate_total(void) {
    int total = 0;
    for (int i = 0; i < MENU_COUNT; i++)
        if (selected[i]) total += menu_items[i].price;

    char buf[64];
    snprintf(buf, sizeof(buf), "Total: Rs. %d", total);
    gtk_label_set_text(GTK_LABEL(total_label), buf);
}

/* ── Select button callback ── */
static void on_select_clicked(GtkButton *btn, gpointer data) {
    int idx = GPOINTER_TO_INT(data);
    selected[idx] = !selected[idx];

    if (selected[idx]) {
        gtk_button_set_label(btn, "✓ Selected");
        gtk_style_context_add_class(
            gtk_widget_get_style_context(GTK_WIDGET(btn)), "selected");
    } else {
        gtk_button_set_label(btn, "Select");
        gtk_style_context_remove_class(
            gtk_widget_get_style_context(GTK_WIDGET(btn)), "selected");
    }
    recalculate_total();
}

/* ── Place Order button callback ── */
static void on_place_order(GtkButton *btn, gpointer data) {
    (void)btn; (void)data;

    int count = 0, total = 0;
    char msg[512];
    char *p = msg;
    p += sprintf(p, "🍽  ORDER CONFIRMED!\n\n");

    for (int i = 0; i < MENU_COUNT; i++) {
        if (selected[i]) {
            p += sprintf(p, "  %-14s  Rs. %d\n",
                         menu_items[i].name, menu_items[i].price);
            total += menu_items[i].price;
            count++;
        }
    }

    if (count == 0) {
        gtk_label_set_text(GTK_LABEL(status_bar),
                           "⚠  Please select at least one item before placing an order.");
        return;
    }

    sprintf(p, "\n  TOTAL:  Rs. %d", total);

    GtkWidget *dialog = gtk_message_dialog_new(
        NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "%s", msg);
    gtk_window_set_title(GTK_WINDOW(dialog), "Order Summary");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    /* Reset */
    memset(selected, 0, sizeof(selected));
    gtk_label_set_text(GTK_LABEL(status_bar),
                       "✔  Order placed! Thank you for dining with us.");
    recalculate_total();
}

/* ── Build UI ── */
void create_main_window(GtkApplication *app, gpointer user_data) {
    (void)user_data;

    /* CSS */
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    memset(selected, 0, sizeof(selected));

    /* Window */
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "🍔 Flame & Fork – Food Ordering System");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 700);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 0);

    /* Root vbox */
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    /* ── HEADER ── */
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_style_context_add_class(gtk_widget_get_style_context(header_box), "header-box");
    gtk_box_pack_start(GTK_BOX(vbox), header_box, FALSE, FALSE, 0);

    /* Logo (left) */
    GtkWidget *logo = gtk_label_new("🔥");
    gtk_style_context_add_class(gtk_widget_get_style_context(logo), "logo-label");
    gtk_box_pack_start(GTK_BOX(header_box), logo, FALSE, FALSE, 0);

    /* Center text */
    GtkWidget *center_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_widget_set_hexpand(center_vbox, TRUE);
    gtk_box_pack_start(GTK_BOX(header_box), center_vbox, TRUE, TRUE, 0);

    GtkWidget *name_lbl = gtk_label_new("Flame & Fork");
    gtk_style_context_add_class(gtk_widget_get_style_context(name_lbl), "restaurant-name");
    gtk_label_set_justify(GTK_LABEL(name_lbl), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(center_vbox), name_lbl, FALSE, FALSE, 0);

    GtkWidget *tag_lbl = gtk_label_new("FINE DINING  ·  FAST SERVICE  ·  FRESH FLAVORS");
    gtk_style_context_add_class(gtk_widget_get_style_context(tag_lbl), "tagline-label");
    gtk_label_set_justify(GTK_LABEL(tag_lbl), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(center_vbox), tag_lbl, FALSE, FALSE, 0);

    /* Right logo */
    GtkWidget *logo2 = gtk_label_new("🍴");
    gtk_style_context_add_class(gtk_widget_get_style_context(logo2), "logo-label");
    gtk_box_pack_end(GTK_BOX(header_box), logo2, FALSE, FALSE, 0);

    /* ── COLUMN HEADERS ── */
    GtkWidget *col_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(vbox), col_row, FALSE, FALSE, 0);

    const char *col_names[] = {"ID", "ITEM NAME", "PRICE (Rs.)", "ACTION"};
    int col_expand[] = {0, 1, 0, 0};
    for (int c = 0; c < 4; c++) {
        GtkWidget *lbl = gtk_label_new(col_names[c]);
        gtk_style_context_add_class(gtk_widget_get_style_context(lbl), "col-header");
        if (c == 0) gtk_widget_set_size_request(lbl, 70, -1);
        if (c == 2) gtk_widget_set_size_request(lbl, 130, -1);
        if (c == 3) gtk_widget_set_size_request(lbl, 130, -1);
        gtk_label_set_xalign(GTK_LABEL(lbl), 0.5f);
        gtk_box_pack_start(GTK_BOX(col_row), lbl, col_expand[c], col_expand[c], 0);
    }

    /* ── SCROLLABLE MENU LIST ── */
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scroll, TRUE);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);

    GtkWidget *list_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(scroll), list_box);

    for (int i = 0; i < MENU_COUNT; i++) {
        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_style_context_add_class(gtk_widget_get_style_context(row), "row-box");
        gtk_box_pack_start(GTK_BOX(list_box), row, FALSE, FALSE, 0);

        /* ID */
        char id_buf[8];
        snprintf(id_buf, sizeof(id_buf), "#%02d", menu_items[i].id);
        GtkWidget *id_lbl = gtk_label_new(id_buf);
        gtk_style_context_add_class(gtk_widget_get_style_context(id_lbl), "id-label");
        gtk_widget_set_size_request(id_lbl, 70, -1);
        gtk_label_set_xalign(GTK_LABEL(id_lbl), 0.5f);
        gtk_box_pack_start(GTK_BOX(row), id_lbl, FALSE, FALSE, 0);

        /* Name */
        GtkWidget *name_l = gtk_label_new(menu_items[i].name);
        gtk_style_context_add_class(gtk_widget_get_style_context(name_l), "name-label");
        gtk_label_set_xalign(GTK_LABEL(name_l), 0.0f);
        gtk_box_pack_start(GTK_BOX(row), name_l, TRUE, TRUE, 8);

        /* Price */
        char price_buf[20];
        snprintf(price_buf, sizeof(price_buf), "Rs. %d", menu_items[i].price);
        GtkWidget *price_l = gtk_label_new(price_buf);
        gtk_style_context_add_class(gtk_widget_get_style_context(price_l), "price-label");
        gtk_widget_set_size_request(price_l, 130, -1);
        gtk_label_set_xalign(GTK_LABEL(price_l), 0.5f);
        gtk_box_pack_start(GTK_BOX(row), price_l, FALSE, FALSE, 0);

        /* Select button */
        GtkWidget *sel_btn = gtk_button_new_with_label("Select");
        gtk_style_context_add_class(gtk_widget_get_style_context(sel_btn), "select-btn");
        gtk_widget_set_size_request(sel_btn, 110, 34);
        GtkWidget *btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_widget_set_size_request(btn_box, 130, -1);
        gtk_box_set_homogeneous(GTK_BOX(btn_box), TRUE);
        gtk_box_pack_start(GTK_BOX(btn_box), sel_btn, FALSE, FALSE, 10);
        gtk_box_pack_start(GTK_BOX(row), btn_box, FALSE, FALSE, 0);

        g_signal_connect(sel_btn, "clicked", G_CALLBACK(on_select_clicked),
                         GINT_TO_POINTER(i));
    }

    /* ── FOOTER ── */
    GtkWidget *footer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_style_context_add_class(gtk_widget_get_style_context(footer), "footer-box");
    gtk_box_pack_start(GTK_BOX(vbox), footer, FALSE, FALSE, 0);

    /* Total row */
    GtkWidget *total_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(footer), total_row, FALSE, FALSE, 0);

    total_label = gtk_label_new("Total: Rs. 0");
    gtk_style_context_add_class(gtk_widget_get_style_context(total_label), "total-label");
    gtk_box_pack_start(GTK_BOX(total_row), total_label, TRUE, TRUE, 0);

    GtkWidget *order_btn = gtk_button_new_with_label("🛒  PLACE ORDER");
    gtk_style_context_add_class(gtk_widget_get_style_context(order_btn), "order-btn");
    gtk_box_pack_end(GTK_BOX(total_row), order_btn, FALSE, FALSE, 0);
    g_signal_connect(order_btn, "clicked", G_CALLBACK(on_place_order), NULL);

    /* Status bar */
    status_bar = gtk_label_new("Select your items and tap 'Place Order' when ready.");
    gtk_style_context_add_class(gtk_widget_get_style_context(status_bar), "status-label");
    gtk_label_set_xalign(GTK_LABEL(status_bar), 0.0f);
    gtk_box_pack_start(GTK_BOX(footer), status_bar, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
}
