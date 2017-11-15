#ifndef __CRUD_APP_DIALOG__
#define __CRUD_APP_DIALOG__

#include "crud-app-win.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define CRUD_TYPE_APP_DIALOG crud_app_dialog_get_type()
G_DECLARE_FINAL_TYPE(CrudAppDialog,
                     crud_app_dialog,
                     CRUD,
                     APP_DIALOG,
                     GtkDialog)

CrudAppDialog *
crud_app_dialog_new(CrudAppWindow *win);

G_END_DECLS

#endif /* ifndef __CRUD_APP_DIALOG__ */
