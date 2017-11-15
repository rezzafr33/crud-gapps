#ifndef __CRUD_APP_WINDOW
#define __CRUD_APP_WINDOW

#include "crud-app.h"

G_BEGIN_DECLS

#define CRUD_TYPE_APP_WINDOW crud_app_window_get_type()
G_DECLARE_FINAL_TYPE(CrudAppWindow,
                     crud_app_window,
                     CRUD,
                     APP_WINDOW,
                     GtkApplicationWindow)

typedef enum {
  ADD_BUTTON_CLICKED,
  EDIT_BUTTON_CLICKED,
  DELETE_BUTTON_CLICKED,
  N_SIGNALS,
} CrudWindowSignal;

typedef enum {
  ID,
  NAME,
  RELEASE,
  LANGUAGE,
  N_COLUMNS,
} CrudWindowColumn;

CrudAppWindow *
crud_app_window_new(CrudApp *app);

void
crud_app_window_add_button_clicked(CrudAppWindow *self);

void
crud_app_window_edit_button_clicked(CrudAppWindow *self);

void
crud_app_window_delete_button_clicked(CrudAppWindow *self);

GtkWidget *
crud_app_window_get_treeview(CrudAppWindow *self);

void
crud_app_window_add_to_list(CrudAppWindow *self,
                            guint id,
                            gchar *name,
                            guint year,
                            gchar *lang);

G_END_DECLS

#endif /* ifndef __CRUD_APP_WINDOW */
