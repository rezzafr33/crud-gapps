#include "crud-app-win.h"

typedef struct _CrudAppWindowPrivate CrudAppWindowPrivate;

struct _CrudAppWindowPrivate
{

  GtkWidget *view;
  GtkWidget *button_add;
  GtkWidget *button_delete;
  GtkWidget *button_edit;
};

struct _CrudAppWindow
{
  GtkApplicationWindow parent;
};

static guint signals[N_SIGNALS];

static gchar *columns[N_COLUMNS]
  = { "id", "Software", "Release Year", "Programming Language" };

static void
init_treeview(GtkWidget *treeview);

static void
add_to_list(GtkWidget *treeview,
            guint id,
            gchar *name,
            guint year,
            gchar *lang);

G_DEFINE_TYPE_WITH_PRIVATE(CrudAppWindow,
                           crud_app_window,
                           GTK_TYPE_APPLICATION_WINDOW)

static void
crud_app_window_init(CrudAppWindow *self)
{
  GtkWidget *grid;
  GtkWidget *sw;
  CrudAppWindowPrivate *priv;

  priv = crud_app_window_get_instance_private(CRUD_APP_WINDOW(self));

  gtk_window_set_title(GTK_WINDOW(self), "GApps CRUD");
  gtk_window_set_default_size(GTK_WINDOW(self), 350, 200);
  gtk_container_set_border_width(GTK_CONTAINER(self), 10);

  grid = gtk_grid_new();
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), FALSE);
  gtk_grid_set_row_homogeneous(GTK_GRID(grid), FALSE);
  gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
  gtk_widget_set_visible(grid, TRUE);

  sw = gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_set_hexpand(sw, TRUE);
  gtk_widget_set_vexpand(sw, TRUE);
  gtk_widget_set_visible(sw, TRUE);

  priv->view = gtk_tree_view_new();
  init_treeview(priv->view);

  add_to_list(priv->view, 1, "Firefox", 2002, "C++");
  add_to_list(priv->view, 2, "Chrome", 2008, "C++");

  gtk_container_add(GTK_CONTAINER(sw), priv->view);
  gtk_grid_attach(GTK_GRID(grid), sw, 0, 0, 8, 10);
  gtk_widget_set_visible(priv->view, TRUE);

  priv->button_add = gtk_button_new_with_mnemonic("_Add");
  gtk_grid_attach_next_to(GTK_GRID(grid), priv->button_add, sw, GTK_POS_BOTTOM,
                          2, 1);
  gtk_widget_set_visible(priv->button_add, TRUE);
  g_signal_connect_swapped(priv->button_add, "clicked",
                           G_CALLBACK(crud_app_window_add_button_clicked),
                           self);

  priv->button_delete = gtk_button_new_with_mnemonic("_Delete");
  gtk_grid_attach_next_to(GTK_GRID(grid), priv->button_delete, priv->button_add,
                          GTK_POS_RIGHT, 2, 1);
  gtk_widget_set_visible(priv->button_delete, TRUE);
  g_signal_connect_swapped(priv->button_delete, "clicked",
                           G_CALLBACK(crud_app_window_delete_button_clicked),
                           self);

  priv->button_edit = gtk_button_new_with_mnemonic("_Edit");
  gtk_grid_attach_next_to(GTK_GRID(grid), priv->button_edit,
                          priv->button_delete, GTK_POS_RIGHT, 2, 1);
  gtk_widget_set_visible(priv->button_edit, TRUE);
  g_signal_connect_swapped(priv->button_edit, "clicked",
                           G_CALLBACK(crud_app_window_edit_button_clicked),
                           self);

  gtk_container_add(GTK_CONTAINER(self), grid);
}

static void
crud_app_window_class_init(CrudAppWindowClass *klass)
{
  signals[ADD_BUTTON_CLICKED] = g_signal_new(
    "add_button_clicked", G_TYPE_FROM_CLASS(klass),
    G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
    0, // G_STRUCT_OFFSET( CrudAppWindowClass, add_button_clicked ),
    NULL, NULL, NULL, G_TYPE_NONE, 1, G_TYPE_INT);

  signals[EDIT_BUTTON_CLICKED] = g_signal_new(
    "edit_button_clicked", G_TYPE_FROM_CLASS(klass),
    G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
    0, // G_STRUCT_OFFSET( CrudAppWindowClass, edit_button_clicked ),
    NULL, NULL, NULL, G_TYPE_NONE, 1, G_TYPE_INT);

  signals[DELETE_BUTTON_CLICKED] = g_signal_new(
    "delete_button_clicked", G_TYPE_FROM_CLASS(klass),
    G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
    0, // G_STRUCT_OFFSET( CrudAppWindowClass, delete_button_clicked ),
    NULL, NULL, NULL, G_TYPE_NONE, 1, G_TYPE_INT);
}

void
crud_app_window_add_button_clicked(CrudAppWindow *self)
{
  g_return_if_fail(CRUD_IS_APP_WINDOW(self));

  g_signal_emit(self, signals[ADD_BUTTON_CLICKED], 0, ADD_BUTTON_CLICKED);
}

void
crud_app_window_edit_button_clicked(CrudAppWindow *self)
{
  g_return_if_fail(CRUD_IS_APP_WINDOW(self));

  g_signal_emit(self, signals[EDIT_BUTTON_CLICKED], 0, EDIT_BUTTON_CLICKED);
}

void
crud_app_window_delete_button_clicked(CrudAppWindow *self)
{
  g_return_if_fail(CRUD_IS_APP_WINDOW(self));

  g_signal_emit(self, signals[DELETE_BUTTON_CLICKED], 0, DELETE_BUTTON_CLICKED);
}

CrudAppWindow *
crud_app_window_new(CrudApp *app)
{
  return g_object_new(CRUD_TYPE_APP_WINDOW, "application", app, NULL);
}

GtkWidget *
crud_app_window_get_treeview(CrudAppWindow *self)
{
  CrudAppWindowPrivate *priv;

  priv = crud_app_window_get_instance_private(self);

  return (GtkWidget *) priv->view;
}

void
crud_app_window_add_to_list(CrudAppWindow *self,
                            guint id,
                            gchar *name,
                            guint year,
                            gchar *lang)
{
  g_return_if_fail(CRUD_IS_APP_WINDOW(self));

  CrudAppWindowPrivate *priv;
  priv = crud_app_window_get_instance_private(self);

  add_to_list(priv->view, id, name, year, lang);
}

static void
add_to_list(GtkWidget *treeview, guint id, gchar *name, guint year, gchar *lang)
{

  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));

  g_return_if_fail(store != NULL);

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, ID, id, NAME, name, RELEASE, year, LANGUAGE,
                     lang, -1);
}

static void
init_treeview(GtkWidget *treeview)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  GtkListStore *store;

  renderer = gtk_cell_renderer_text_new();

  gint i;
  for (i = 0; i < N_COLUMNS; ++i) {
    column = gtk_tree_view_column_new_with_attributes(columns[i], renderer,
                                                      "text", i, NULL);
    gtk_tree_view_column_set_sort_column_id(column, i);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
  }

  column = gtk_tree_view_get_column(GTK_TREE_VIEW(treeview), 0);
  gtk_tree_view_column_set_visible(column, FALSE);

  store = gtk_list_store_new(N_COLUMNS, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT,
                             G_TYPE_STRING);
  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));

  g_object_unref(store);
}
