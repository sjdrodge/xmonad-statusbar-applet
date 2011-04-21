AM_CPPFLAGS = -Wall $(DEPS_CFLAGS)
AM_LDFLAGS = -Wl,--as-needed

appletdir = $(libdir)/gnome-applets
applet_PROGRAMS = xmonad-statusbar-applet
xmonad_statusbar_applet_SOURCES = applet.c
xmonad_statusbar_applet_LDADD = $(DEPS_LIBS)
