#include "crud-app-dialog.h"
#include <stdlib.h>

struct _CrudAppDialog
{
  GtkDialog parent;
};

typedef struct _CrudAppDialogPrivate CrudAppDialogPrivate;
struct _CrudAppDialogPrivate
{
  GtkWidget *name;
  GtkWidget *year;
  GtkWidget *language;
};

enum
{
  PROP_O,
  PROP_NAME,
  PROP_YEAR,
  PROP_LANGUAGE,
  N_PROPERTIES,
};

static GParamSpec *properties[N_PROPERTIES] = {
  NULL,
};

static void
crud_app_dialog_set_property(GObject *obj,
                             guint prop_id,
                             const GValue *value,
                             GParamSpec *pspec);

static void
crud_app_dialog_get_property(GObject *obj,
                             guint prop_id,
                             GValue *value,
                             GParamSpec *pspec);

static void
on_entry_changed(GtkEditable *entry, gpointer self);

gboolean
on_year_keypress(GtkWidget *widget, GdkEvent *event, gpointer user_data);

G_DEFINE_TYPE_WITH_PRIVATE(CrudAppDialog, crud_app_dialog, GTK_TYPE_DIALOG)

static void
crud_app_dialog_init(CrudAppDialog *self)
{
  GtkWidget *content_area, *grid, *label;
  CrudAppDialogPrivate *priv;

  gtk_dialog_add_button(GTK_DIALOG(self), "_OK", GTK_RESPONSE_OK);
  gtk_dialog_add_button(GTK_DIALOG(self), "_Cancel", GTK_RESPONSE_CANCEL);
  gtk_dialog_set_default_response(GTK_DIALOG(self), GTK_RESPONSE_OK);
  gtk_dialog_set_response_sensitive(GTK_DIALOG(self), GTK_RESPONSE_OK, FALSE);

  priv = crud_app_dialog_get_instance_private(self);
  content_area = gtk_dialog_get_content_area(GTK_DIALOG(self));

  grid = gtk_grid_new();

  label = gtk_label_new_with_mnemonic("_Software");
  priv->name = gtk_entry_new();
  gtk_label_set_mnemonic_widget(GTK_LABEL(label), priv->name);
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
  gtk_grid_attach_next_to(GTK_GRID(grid), priv->name, label, GTK_POS_RIGHT, 1,
                          1);
  gtk_widget_set_visible(label, TRUE);
  gtk_widget_set_visible(priv->name, TRUE);

  label = gtk_label_new_with_mnemonic("_Release Year");
  priv->year = gtk_entry_new();
  gtk_label_set_mnemonic_widget(GTK_LABEL(label), priv->year);
  gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
  gtk_grid_attach_next_to(GTK_GRID(grid), priv->year, label, GTK_POS_RIGHT, 1,
                          1);
  gtk_widget_set_visible(label, TRUE);
  gtk_widget_set_visible(priv->year, TRUE);

  label = gtk_label_new_with_mnemonic("_Language");
  priv->language = gtk_entry_new();
  gtk_label_set_mnemonic_widget(GTK_LABEL(label), priv->language);
  gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
  gtk_grid_attach_next_to(GTK_GRID(grid), priv->language, label, GTK_POS_RIGHT,
                          1, 1);
  gtk_widget_set_visible(label, TRUE);
  gtk_widget_set_visible(priv->language, TRUE);

  g_signal_connect(priv->year, "key-press-event", G_CALLBACK(on_year_keypress),
                   self);

  g_signal_connect(priv->name, "changed", G_CALLBACK(on_entry_changed), self);
  g_signal_connect(priv->year, "changed", G_CALLBACK(on_entry_changed), self);
  g_signal_connect(priv->language, "changed", G_CALLBACK(on_entry_changed),
                   self);

  gtk_container_add(GTK_CONTAINER(content_area), grid);
  gtk_widget_set_visible(grid, TRUE);
}

static void
crud_app_dialog_class_init(CrudAppDialogClass *klass)
{
  GObjectClass *oclass = G_OBJECT_CLASS(klass);
  G_OBJECT_CLASS(klass)->set_property = crud_app_dialog_set_property;
  G_OBJECT_CLASS(klass)->get_property = crud_app_dialog_get_property;

  properties[PROP_NAME]
    = g_param_spec_string("entry-name", "Entry Name", "Name of software", NULL,
                          G_PARAM_READWRITE);

  properties[PROP_YEAR]
    = g_param_spec_uint("entry-year", "Entry Year", "Year Released", 0, 9999, 0,
                        G_PARAM_READWRITE);

  properties[PROP_LANGUAGE]
    = g_param_spec_string("entry-language", "Entry Language", "Year Released",
                          NULL, G_PARAM_READWRITE);

  g_object_class_install_properties(oclass, N_PROPERTIES, properties);
}

CrudAppDialog *
crud_app_dialog_new(CrudAppWindow *win)
{
  return g_object_new(CRUD_TYPE_APP_DIALOG, "transient-for", win,
                      "use-header-bar", TRUE, "modal", TRUE, NULL);
}

static void
crud_app_dialog_set_property(GObject *obj,
                             guint prop_id,
                             const GValue *value,
                             GParamSpec *pspec)
{
  CrudAppDialog *self;
  CrudAppDialogPrivate *priv;
  gchararray name, year, language;

  self = CRUD_APP_DIALOG(obj);
  priv = crud_app_dialog_get_instance_private(self);

  switch (prop_id) {
    case PROP_NAME:
      name = g_value_dup_string(value);
      gtk_entry_set_text(GTK_ENTRY(priv->name), name);
      g_free(name);
      break;
    case PROP_YEAR:
      year = g_strdup_printf("%d", g_value_get_uint(value));
      gtk_entry_set_text(GTK_ENTRY(priv->year), year);
      g_free(year);
      break;
    case PROP_LANGUAGE:
      language = g_value_dup_string(value);
      gtk_entry_set_text(GTK_ENTRY(priv->language), language);
      g_free(language);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static void
crud_app_dialog_get_property(GObject *obj,
                             guint prop_id,
                             GValue *value,
                             GParamSpec *pspec)
{
  CrudAppDialog *self;
  CrudAppDialogPrivate *priv;

  self = CRUD_APP_DIALOG(obj);
  priv = crud_app_dialog_get_instance_private(self);

  switch (prop_id) {
    case PROP_NAME:
      g_value_set_string(value, gtk_entry_get_text(GTK_ENTRY(priv->name)));
      break;
    case PROP_YEAR:
      g_value_set_uint(value, atoi(gtk_entry_get_text(GTK_ENTRY(priv->year))));
      break;
    case PROP_LANGUAGE:
      g_value_set_string(value, gtk_entry_get_text(GTK_ENTRY(priv->language)));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, prop_id, pspec);
      break;
  }
}

static void
on_entry_changed(GtkEditable *entry, gpointer self)
{

  CrudAppDialog *dialog;
  CrudAppDialogPrivate *priv;

  dialog = CRUD_APP_DIALOG(self);
  priv = crud_app_dialog_get_instance_private(dialog);

  if ((gtk_entry_get_text_length(GTK_ENTRY(priv->name)) > 0)
      && (gtk_entry_get_text_length(GTK_ENTRY(priv->year)) > 0)
      && (gtk_entry_get_text_length(GTK_ENTRY(priv->language)) > 0)) {
    gtk_dialog_set_response_sensitive(GTK_DIALOG(dialog), GTK_RESPONSE_OK,
                                      TRUE);
  } else {
    gtk_dialog_set_response_sensitive(GTK_DIALOG(dialog), GTK_RESPONSE_OK,
                                      FALSE);
  }
}

gboolean
on_year_keypress(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
  guint key = event->key.keyval;

  if ((key >= GDK_KEY_0 && key <= GDK_KEY_9)
      || (key >= GDK_KEY_KP_0 && key <= GDK_KEY_KP_9) || (key == GDK_KEY_Left)
      || (key == GDK_KEY_Right) || (key == GDK_KEY_KP_Left)
      || (key == GDK_KEY_KP_Right) || (key == GDK_KEY_Tab)
      || (key == GDK_KEY_BackSpace) || (key == GDK_KEY_Up)
      || (key == GDK_KEY_Down) || (key == GDK_KEY_KP_Up)
      || (key == GDK_KEY_KP_Down) || (key == GDK_KEY_Escape)) {
    return FALSE;
  }

  return TRUE;
}
