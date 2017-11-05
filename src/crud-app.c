#include "crud-app.h"

struct _CrudApp
{
  GtkApplication parent;
};

G_DEFINE_TYPE(CrudApp, crud_app, GTK_TYPE_APPLICATION)

static void
crud_app_init(CrudApp *self)
{
}

static void
crud_app_activate(GApplication *self)
{
  GtkWidget *win;
  win = gtk_application_window_new(GTK_APPLICATION(self));

  gtk_window_present(GTK_WINDOW(win));
}

static void
quit_activated(GSimpleAction *action, GVariant *params, gpointer app)
{
  g_message("GAction called: %s\n", g_action_get_name(G_ACTION(action)));
  g_application_quit(G_APPLICATION(app));
}

static void
crud_app_startup(GApplication *self)
{
  GtkBuilder *builder;
  GSimpleAction *action;
  GMenuModel *app_menu;

  const gchar *quit_accels[2] = { "<Ctrl>Q", NULL };

  G_APPLICATION_CLASS(crud_app_parent_class)->startup(self);

  builder = gtk_builder_new_from_string(
    "<interface>"
    " <menu id='app-menu'>"
    "   <section>"
    "     <item>"
    "       <attribute name='label'>_Quit</attribute>"
    "       <attribute name='action'>app.quit</attribute>"
    "     </item>"
    "   </section>"
    " </menu>"
    "</interface>",
    -1);
  app_menu = G_MENU_MODEL(gtk_builder_get_object(builder, "app-menu"));

  action = g_simple_action_new("quit", NULL);
  g_signal_connect(action, "activate", G_CALLBACK(quit_activated), self);
  g_action_map_add_action(G_ACTION_MAP(self), G_ACTION(action));

  gtk_application_set_accels_for_action(GTK_APPLICATION(self), "app.quit",
                                        quit_accels);

  gtk_application_set_app_menu(GTK_APPLICATION(self), G_MENU_MODEL(app_menu));

  g_object_unref(builder);
}

static void
crud_app_class_init(CrudAppClass *klass)
{
  G_APPLICATION_CLASS(klass)->startup = crud_app_startup;
  G_APPLICATION_CLASS(klass)->activate = crud_app_activate;
}

CrudApp *
crud_app_new()
{
  return g_object_new(CRUD_TYPE_APP, "application-id", "com.yami.crudapp",
                      NULL);
}
