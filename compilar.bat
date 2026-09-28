@echo off
setlocal
title TOTRUS - Compilador PS2

set "PROJECT_WIN=F:\aa_development_ps2\totrus"
set "PROJECT_MSYS=/f/aa_development_ps2/totrus"
set "MSYS_ENV=C:\msys64\usr\bin\env.exe"

echo ========================================
echo          TOTRUS - PS2 BUILD
echo ========================================
echo.

if not exist "%MSYS_ENV%" (
    echo ERROR: No encontre MSYS2 en C:\msys64
    echo.
    pause
    exit /b 1
)

if not exist "%PROJECT_WIN%\Makefile" (
    echo ERROR: No encontre %PROJECT_WIN%\Makefile
    echo.
    pause
    exit /b 1
)

"%MSYS_ENV%" MSYSTEM=MINGW32 CHERE_INVOKING=1 /usr/bin/bash -lc "export PS2DEV=/f/aa_development_ps2/totrus/ps2dev; export PS2SDK=$PS2DEV/ps2sdk; export GSKIT=$PS2DEV/gsKit; export PATH=$PS2DEV/bin:$PS2DEV/ee/bin:$PS2DEV/iop/bin:$PS2DEV/dvp/bin:$PS2SDK/bin:$PATH; cd /f/aa_development_ps2/totrus && make clean && make"

if errorlevel 1 (
    echo.
    echo ========================================
    echo          ERROR AL COMPILAR
    echo ========================================
    echo.
    pause
    exit /b 1
)

if not exist "%PROJECT_WIN%\TOTRUS.ELF" (
    echo.
    echo ERROR: make termino sin error, pero TOTRUS.ELF no existe.
    echo.
    pause
    exit /b 1
)

echo.
echo Compilacion OK.
echo Buscando pendrive LONE WOLF...
echo.

set "DRIVEFILE=%TEMP%\totrus_drive.txt"
del "%DRIVEFILE%" 2>nul

powershell.exe -NoProfile -ExecutionPolicy Bypass -Command "$v=Get-Volume -FileSystemLabel 'LONE WOLF' -ErrorAction SilentlyContinue | Select-Object -First 1; if($v -and $v.DriveLetter){$v.DriveLetter}" > "%DRIVEFILE%"

set "USB="
set /p USB=<"%DRIVEFILE%"
del "%DRIVEFILE%" 2>nul

if not defined USB (
    echo No encontre un volumen llamado LONE WOLF.
    echo.
    echo El juego SI fue compilado:
    echo %PROJECT_WIN%\TOTRUS.ELF
    echo.
    pause
    exit /b 0
)

set "USB=%USB%:"

echo Encontrado: %USB% [LONE WOLF]
echo.

if not exist "%USB%\APPS" mkdir "%USB%\APPS"
if not exist "%USB%\APPS\TOTRUS" mkdir "%USB%\APPS\TOTRUS"

copy /Y "%PROJECT_WIN%\TOTRUS.ELF" "%USB%\APPS\TOTRUS\TOTRUS.ELF" >nul

if errorlevel 1 (
    echo.
    echo ERROR copiando TOTRUS.ELF al pendrive.
    echo.
    pause
    exit /b 1
)

> "%USB%\APPS\TOTRUS\title.cfg" echo title=Totrus
>>"%USB%\APPS\TOTRUS\title.cfg" echo boot=TOTRUS.ELF

echo.
echo ========================================
echo             TODO LISTO
echo ========================================
echo.
echo Compilado:
echo %PROJECT_WIN%\TOTRUS.ELF
echo.
echo Instalado:
echo %USB%\APPS\TOTRUS\TOTRUS.ELF
echo %USB%\APPS\TOTRUS\title.cfg
echo.
echo Las demas aplicaciones de APPS no fueron tocadas.
echo.
pause
