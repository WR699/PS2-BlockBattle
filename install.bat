@echo off
setlocal EnableExtensions EnableDelayedExpansion
chcp 65001 >nul

title Block Battle - Instalador de entorno PS2

rem ============================================================
rem Todo se calcula desde la carpeta donde vive este BAT.
rem No depende de C:, F: ni de una ruta fija.
rem ============================================================
set "ROOT=%~dp0"
if "%ROOT:~-1%"=="\" set "ROOT=%ROOT:~0,-1%"

call :map_root
if errorlevel 1 goto :fatal

set "SAFE_ROOT=!VDRIVE!:\"
set "TOOLS=!SAFE_ROOT!tools"
set "MSYS=!TOOLS!\msys64"
set "PS2DEV=!TOOLS!\ps2dev"
set "DOWNLOADS=!TOOLS!\downloads"

if not exist "!TOOLS!" mkdir "!TOOLS!"
if not exist "!DOWNLOADS!" mkdir "!DOWNLOADS!"

echo.
echo ========================================
echo       BLOCK BATTLE - INSTALAR ENTORNO PS2
echo ========================================
echo.
echo Proyecto: !SAFE_ROOT!
echo Herramientas: !TOOLS!
echo.

rem ============================================================
rem 1) MSYS2 portable/local
rem ============================================================
if exist "!MSYS!\usr\bin\bash.exe" (
    echo [OK] MSYS2 local ya existe.
) else (
    echo [1/4] Descargando MSYS2 x86_64 oficial...
    set "MSYS_SFX=!DOWNLOADS!\msys2-x86_64-latest.sfx.exe"

    powershell.exe -NoProfile -ExecutionPolicy Bypass -Command ^
      "$ErrorActionPreference='Stop';" ^
      "$ProgressPreference='SilentlyContinue';" ^
      "Invoke-WebRequest -UseBasicParsing -Uri 'https://repo.msys2.org/distrib/msys2-x86_64-latest.sfx.exe' -OutFile '!MSYS_SFX!'"

    if errorlevel 1 (
        echo ERROR: no se pudo descargar MSYS2.
        goto :fail
    )

    echo [2/4] Extrayendo MSYS2 dentro de tools\msys64...
    "!MSYS_SFX!" -y -o"!TOOLS!" >nul
    if errorlevel 1 (
        echo ERROR: no se pudo extraer MSYS2.
        goto :fail
    )

    if not exist "!MSYS!\usr\bin\bash.exe" (
        echo ERROR: MSYS2 no quedo en tools\msys64 como se esperaba.
        goto :fail
    )
)

echo [3/4] Preparando MSYS2...
"!MSYS!\usr\bin\bash.exe" -lc "true"
if errorlevel 1 (
    echo ERROR: MSYS2 no pudo iniciar.
    goto :fail
)

rem Cada pacman corre en un proceso nuevo. Esto permite que una actualizacion
rem del runtime de MSYS2 se aplique antes de la siguiente pasada.
"!MSYS!\usr\bin\bash.exe" -lc "pacman -Syu --noconfirm" >nul 2>&1
"!MSYS!\usr\bin\bash.exe" -lc "pacman -Syu --noconfirm" >nul 2>&1

"!MSYS!\usr\bin\bash.exe" -lc "pacman -S --needed --noconfirm make tar mingw-w64-i686-gcc"
if errorlevel 1 (
    echo ERROR: no se pudieron instalar las dependencias de MSYS2.
    goto :fail
)

rem ============================================================
rem 2) PS2DEV local
rem ============================================================
if exist "!PS2DEV!\ee\bin\mips64r5900el-ps2-elf-gcc.exe" (
    echo [OK] PS2DEV local ya existe.
) else (
    echo [4/4] Preparando PS2DEV precompilado para Windows...
    set "PS2DEV_ARCHIVE=!DOWNLOADS!\ps2dev-windows-latest.tar.gz"

    rem Si una ejecucion anterior ya bajo el archivo, lo reutilizamos.
    if not exist "!PS2DEV_ARCHIVE!" (
        echo Descargando PS2DEV...
        powershell.exe -NoProfile -ExecutionPolicy Bypass -Command ^
          "$ErrorActionPreference='Stop';" ^
          "$ProgressPreference='SilentlyContinue';" ^
          "Invoke-WebRequest -UseBasicParsing -Headers @{'User-Agent'='BlockBattle-Installer'} -Uri 'https://github.com/ps2dev/ps2dev/releases/download/latest/ps2dev-windows-latest.tar.gz' -OutFile '!PS2DEV_ARCHIVE!'"

        if errorlevel 1 (
            echo ERROR: no se pudo descargar PS2DEV.
            goto :fail
        )
    ) else (
        echo [OK] Se reutiliza el archivo PS2DEV ya descargado.
    )

    if not exist "!PS2DEV!" mkdir "!PS2DEV!"

    echo Extrayendo PS2DEV...
    rem ------------------------------------------------------------
    rem El tar de PS2DEV contiene symlinks Unix. MSYS2 usa por defecto
    rem winsymlinks:deepcopy; si el link aparece antes que su destino,
    rem la primera pasada puede fallar. Se extrae dos veces: la primera
    rem crea los destinos y la segunda resuelve/copias los links.
    rem Los errores esperados de la primera pasada se ocultan.
    rem ------------------------------------------------------------
    "!MSYS!\usr\bin\env.exe" MSYSTEM=MINGW32 MSYS=winsymlinks:deepcopy CHERE_INVOKING=1 "!MSYS!\usr\bin\bash.exe" -lc "tar -xf '/!VDRIVE!/tools/downloads/ps2dev-windows-latest.tar.gz' --strip-components=1 -C '/!VDRIVE!/tools/ps2dev' 2>/dev/null || true"

    "!MSYS!\usr\bin\env.exe" MSYSTEM=MINGW32 MSYS=winsymlinks:deepcopy CHERE_INVOKING=1 "!MSYS!\usr\bin\bash.exe" -lc "tar -xf '/!VDRIVE!/tools/downloads/ps2dev-windows-latest.tar.gz' --strip-components=1 -C '/!VDRIVE!/tools/ps2dev'"
    set "TAR_RESULT=!ERRORLEVEL!"

    rem Lo decisivo es que haya quedado el toolchain necesario para Block Battle.
    rem Si tar solo protesta por aliases/symlinks de ps2sdk-ports, pero el
    rem compilador, PS2SDK y gsKit existen, no bloqueamos el entorno.
    if not exist "!PS2DEV!\ee\bin\mips64r5900el-ps2-elf-gcc.exe" (
        echo ERROR: la extraccion no dejo el compilador de PS2 disponible.
        goto :fail
    )
    if not exist "!PS2DEV!\ps2sdk\samples\Makefile.pref" (
        echo ERROR: la extraccion no dejo PS2SDK completo.
        goto :fail
    )
    if not exist "!PS2DEV!\gsKit\include\gsKit.h" (
        echo ERROR: la extraccion no dejo gsKit completo.
        goto :fail
    )

    if not "!TAR_RESULT!"=="0" (
        echo AVISO: tar reporto enlaces opcionales que Windows no pudo recrear.
        echo        El compilador, PS2SDK y gsKit necesarios para Block Battle estan presentes.
    )
)

rem Prueba real del toolchain.
"!MSYS!\usr\bin\env.exe" MSYSTEM=MINGW32 CHERE_INVOKING=1 "!MSYS!\usr\bin\bash.exe" -lc "export PS2DEV='/!VDRIVE!/tools/ps2dev'; export PS2SDK=$PS2DEV/ps2sdk; export GSKIT=$PS2DEV/gsKit; export PATH=$PS2DEV/bin:$PS2DEV/ee/bin:$PS2DEV/iop/bin:$PS2DEV/dvp/bin:$PS2SDK/bin:$PATH; mips64r5900el-ps2-elf-gcc --version"
if errorlevel 1 (
    echo ERROR: PS2DEV se instalo, pero el compilador no pudo ejecutarse.
    goto :fail
)

rem Limpiamos los instaladores solo despues de verificar que todo funciona.
if exist "!DOWNLOADS!\msys2-x86_64-latest.sfx.exe" del /q "!DOWNLOADS!\msys2-x86_64-latest.sfx.exe" >nul 2>&1
if exist "!DOWNLOADS!\ps2dev-windows-latest.tar.gz" del /q "!DOWNLOADS!\ps2dev-windows-latest.tar.gz" >nul 2>&1

call :unmap_root

echo.
echo ========================================
echo              ENTORNO LISTO
echo ========================================
echo.
echo Todo el entorno de Block Battle esta dentro de .\tools\
echo La carpeta completa se puede mover a otra unidad o ruta.
echo.
pause
exit /b 0

:map_root
set "VDRIVE="
for %%D in (t u v w x y z r s q p) do (
    if not exist "%%D:\" (
        subst %%D: "%ROOT%" >nul 2>&1
        if not errorlevel 1 (
            set "VDRIVE=%%D"
            goto :map_ok
        )
    )
)
echo ERROR: no encontre una letra de unidad libre para montar temporalmente el proyecto.
exit /b 1

:map_ok
exit /b 0

:unmap_root
if defined VDRIVE subst !VDRIVE!: /d >nul 2>&1
exit /b 0

:fail
call :unmap_root
echo.
echo ========================================
echo        ERROR INSTALANDO ENTORNO
echo ========================================
echo.
pause
exit /b 1

:fatal
echo.
echo No se pudo preparar una ruta temporal segura para Block Battle.
pause
exit /b 1
