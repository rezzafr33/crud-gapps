#include "crud-app-dialog.h"
#include "crud-app-win.h"
#include "crud-app.h"

struct _CrudApp
{
  GtkApplication parent;
};

static guint id = 3;

static void
add_entry(CrudAppWindow *win);

static void
edit_entry(CrudAppWindow *win);

static void
delete_entry(CrudAppWindow *win);

static void
button_clicked(CrudAppWindow *win, gint button_id, gpointer data);

static void
crud_app_activate(GApplication *self);

static void
quit_activated(GSimpleAction *action, GVariant *params, gpointer app);

static void
crud_app_startup(GApplication *self);

G_DEFINE_TYPE(CrudApp, crud_app, GTK_TYPE_APPLICATION)

static void
crud_app_init(CrudApp *self)
{
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

static void
button_clicked(CrudAppWindow *win, gint button_id, gpointer data)
{
  switch (button_id) {
    case ADD_BUTTON_CLICKED:
      add_entry(win);
      break;
    case EDIT_BUTTON_CLICKED:
      edit_entry(win);
      break;
    case DELETE_BUTTON_CLICKED:
      delete_entry(win);
      break;
    default:
      break;
  }
}

static void
add_entry(CrudAppWindow *win)
{
  CrudAppDialog *dialog;
  gint response;
  gint year;
  gchararray name, language;

  dialog = crud_app_dialog_new(win);
  gtk_window_set_title(GTK_WINDOW(dialog), "Add new entry");

  gtk_window_present(GTK_WINDOW(dialog));

  response = gtk_dialog_run(GTK_DIALOG(dialog));

  if (response == GTK_RESPONSE_OK) {
    g_object_get(G_OBJECT(dialog), "entry-name", &name, "entry-year", &year,
                 "entry-language", &language, NULL);

    g_message("Name: %s, Released: %d, Language: %s", name, year, language);

    crud_app_window_add_to_list(win, id, name, year, language);
    id++;

    g_free(name);
    g_free(language);
  }

  gtk_widget_destroy(GTK_WIDGET(dialog));
}

static void
edit_entry(CrudAppWindow *win)
{
  CrudAppDialog *dialog;
  gint response;
  gint year;
  gchararray name, language;

  GtkWidget *view;
  GtkTreeIter iter;
  GtkTreeModel *store;
  GtkTreeSelection *selection;

  dialog = crud_app_dialog_new(win);
  gtk_window_set_title(GTK_WINDOW(dialog), "Edit entry");

  view = crud_app_window_get_treeview(win);

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));

  if (!gtk_tree_selection_get_selected(selection, &store, &iter))
    return;

  gtk_tree_model_get(store, &iter, NAME, &name, RELEASE, &year, LANGUAGE,
                     &language, -1);

  g_object_set(G_OBJECT(dialog), "entry-name", name, "entry-year", year,
               "entry-language", language, NULL);
  g_free(name);
  g_free(language);

  gtk_window_present(GTK_WINDOW(dialog));

  response = gtk_dialog_run(GTK_DIALOG(dialog));

  if (response == GTK_RESPONSE_OK) {
    g_object_get(G_OBJECT(dialog), "entry-name", &name, "entry-year", &year,
                 "entry-language", &language, NULL);

    gtk_list_store_set(GTK_LIST_STORE(store), &iter, NAME, name, RELEASE, year,
                       LANGUAGE, language, -1);

    g_free(name);
    g_free(language);
  }

  gtk_widget_destroy(GTK_WIDGET(dialog));
}

static void
delete_entry(CrudAppWindow *win)
{
  GtkDialogFlags flags;
  GtkTreeIter iter;
  GtkTreeModel *store;
  GtkTreeSelection *selection;
  GtkWidget *view, *dialog;
  gint response;
  gchararray name;

  view = crud_app_window_get_treeview(win);

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));

  if (!gtk_tree_selection_get_selected(selection, &store, &iter))
    return;

  gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, NAME, &name, -1);

  flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT
          | GTK_DIALOG_USE_HEADER_BAR;

  dialog
    = gtk_message_dialog_new(GTK_WINDOW(win), flags, GTK_MESSAGE_QUESTION,
                             GTK_BUTTONS_OK_CANCEL, "Are you sure\n"
                                                    "to delete this entry?");

  gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "%s",
                                           name);
  g_free(name);

  response = gtk_dialog_run(GTK_DIALOG(dialog));

  if (response == GTK_RESPONSE_OK) {
    gtk_list_store_remove(GTK_LIST_STORE(store), &iter);
  }

  gtk_widget_destroy(GTK_WIDGET(dialog));
}

static void
crud_app_activate(GApplication *self)
{
  CrudAppWindow *win;

  g_assert(CRUD_IS_APP(self));

  win
    = CRUD_APP_WINDOW(gtk_application_get_active_window(GTK_APPLICATION(self)));

  if (win == NULL) {
    win = crud_app_window_new(CRUD_APP(self));

    g_signal_connect(win, "add_button_clicked", G_CALLBACK(button_clicked),
                     NULL);
    g_signal_connect(win, "edit_button_clicked", G_CALLBACK(button_clicked),
                     NULL);
    g_signal_connect(win, "delete_button_clicked", G_CALLBACK(button_clicked),
                     NULL);
  }

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

  builder = gtk_builder_new_from_resource("/com/yami/crudapp/menu.ui");
  app_menu = G_MENU_MODEL(gtk_builder_get_object(builder, "app-menu"));

  action = g_simple_action_new("quit", NULL);
  g_signal_connect(action, "activate", G_CALLBACK(quit_activated), self);
  g_action_map_add_action(G_ACTION_MAP(self), G_ACTION(action));

  gtk_application_set_accels_for_action(GTK_APPLICATION(self), "app.quit",
                                        quit_accels);

  gtk_application_set_app_menu(GTK_APPLICATION(self), G_MENU_MODEL(app_menu));

  g_object_unref(builder);
}
