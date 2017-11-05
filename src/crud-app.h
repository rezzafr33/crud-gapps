#ifndef __CRUD_APP
#define __CRUD_APP

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define CRUD_TYPE_APP crud_app_get_type()

G_DECLARE_FINAL_TYPE(CrudApp, crud_app, CRUD, APP, GtkApplication)

CrudApp *
crud_app_new(void);

G_END_DECLS

#endif /* ifndef __CRUD_APP */
