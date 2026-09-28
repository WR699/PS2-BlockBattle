TOTRUS - entorno portable para Windows 10/11

Primera vez:
  install.bat

Compilar:
  compilar.bat

Tambien puede ejecutarse compilar.bat directamente: si falta el entorno,
ejecutara install.bat automaticamente.

Todo el toolchain queda dentro de:
  tools\msys64
  tools\ps2dev

La salida queda dentro de:
  build\TOTRUS.ELF
  build\title.cfg

El proyecto se puede mover a otra ruta/unidad. Los BAT calculan sus rutas
desde su propia ubicacion y usan una unidad SUBST temporal para presentar
una ruta segura al toolchain PS2DEV.

LONE WOLF es opcional. Si esta conectado, compilar.bat intenta desplegar a:
  APPS\TOTRUS\
Si no esta conectado o falla el deploy, el build local sigue siendo valido.

Nota sobre PS2DEV en Windows:
El archivo oficial contiene algunos symlinks Unix. El instalador hace dos
pasadas de extraccion en modo MSYS2 winsymlinks:deepcopy y verifica los
componentes realmente requeridos por Totrus.
