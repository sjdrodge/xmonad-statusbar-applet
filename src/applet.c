/*
 * xmonad-statusbar-applet/src/applet.c
 *
 * Copyright (C) 2011 Stephen Drodge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <gio/gio.h>
#include <gtk/gtk.h>
#include <panel-applet.h>

static inline GtkLabel *
get_label_from_applet(PanelApplet *applet)
{
    return GTK_LABEL(gtk_bin_get_child(GTK_BIN(applet)));
}

/*
 * GDBusSignalCallback
 *
 * We are expecting signals which implement the Update member of the
 * org.xmonad.Log interface; that means a single string of markup.
 */
static void
handle_xmonad_log_update_signal(GDBusConnection *connection,
                                const gchar *sender_name,
                                const gchar *object_path,
                                const gchar *interface_name,
                                const gchar *signal_name,
                                GVariant *parameters,
                                gpointer user_data)
{
    gchar *update_string;
    GError *error = NULL;

    if(strcmp(g_variant_get_type_string(parameters), "(s)") != 0) {
        gtk_label_set_text(get_label_from_applet(user_data),
                           "Received malformed DBus signal.");
        return;
    }

    g_variant_get(parameters, "(s)", &update_string);

    if(!pango_parse_markup(update_string, -1, 0, NULL, NULL, NULL, &error)) {
        gtk_label_set_text(get_label_from_applet(user_data),
                           "Received malformed markup.");
        g_printerr("Received malformed markup: %s\n", error->message);
        g_error_free(error);
    } else {
        gtk_label_set_markup(get_label_from_applet(user_data), update_string);
    }

    g_free(update_string);
}

/* GAsyncReadyCallback */
static void
subscribe_to_xmonad_log_update_signal(GObject *source_object,
                                      GAsyncResult *res,
                                      gpointer user_data)
{
    GDBusConnection *connection;
    GError *error = NULL;

    connection = g_bus_get_finish(res, &error);

    if(connection == NULL) {
        gtk_label_set_text(get_label_from_applet(user_data),
                           "Failed to connect to DBus.");
        g_printerr("Failed to connect to DBus: %s\n", error->message);
        g_error_free(error);
    } else {
        g_dbus_connection_signal_subscribe(connection, NULL,
                /* Interface Name */       "org.xmonad.Log",
                /* Signal Name */          "Update",
                /* Object Path */          "/org/xmonad/Log",
                                           NULL, G_DBUS_SIGNAL_FLAGS_NONE,
                                           &handle_xmonad_log_update_signal,
                                           user_data, NULL);

        gtk_label_set_text(get_label_from_applet(user_data),
                           "Waiting for XMonad...");
    }
}

static void
xmonad_statusbar_applet_fill(PanelApplet *applet)
{
    GtkWidget *label;

    panel_applet_set_flags(applet, PANEL_APPLET_EXPAND_MINOR |
                                   PANEL_APPLET_EXPAND_MAJOR);

    label = gtk_label_new(PACKAGE_STRING);
    gtk_label_set_single_line_mode(GTK_LABEL(label), TRUE);
    gtk_label_set_ellipsize(GTK_LABEL(label), PANGO_ELLIPSIZE_END);
    gtk_misc_set_alignment(GTK_MISC(label), 0.0f, 0.5f);

    gtk_container_add(GTK_CONTAINER(applet), label);
    gtk_widget_show_all(GTK_WIDGET(applet));
}

/* PanelAppletFactoryCallback */
static gboolean
xmonad_statusbar_applet_factory(PanelApplet *applet,
                                const gchar *iid,
                                gpointer user_data)
{
    if(strcmp(iid, "OAFIID:" PACKAGE_COMPRESSEDNAME) != 0)
        return FALSE;

    xmonad_statusbar_applet_fill(applet);

    /* Connect to the Session Bus (DBus) */
    g_bus_get(G_BUS_TYPE_SESSION, NULL,
              &subscribe_to_xmonad_log_update_signal, applet);

    gtk_label_set_text(get_label_from_applet(applet),
                       "Connecting to Session Bus...");

    return TRUE;
}

PANEL_APPLET_BONOBO_FACTORY("OAFIID:" PACKAGE_COMPRESSEDNAME "_Factory",
                            PANEL_TYPE_APPLET, PACKAGE_COMPRESSEDNAME,
                            PACKAGE_VERSION, &xmonad_statusbar_applet_factory,
                            NULL);
