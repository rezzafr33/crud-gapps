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

CrudAppWindow *
crud_app_window_new(CrudApp *app);

G_END_DECLS

#endif /* ifndef __CRUD_APP_WINDOW */
