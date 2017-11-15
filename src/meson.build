resources = gnome.compile_resources('crud-app_resources',
  'crud-app.gresource.xml',
  source_dir: '.')

sources = [
  'crud-app.c',
  'crud-app-win.c',
  'crud-app-dialog.c',
  'main.c',
]

crud = executable('crud',
    sources,
    resources,
    dependencies : [gtk])
