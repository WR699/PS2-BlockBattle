@echo off
setlocal

title TOTRUS - PS2 BUILD

echo ========================================
echo          TOTRUS - PS2 BUILD
echo ========================================
echo.

set "PROJECT=F:\aa_development_ps2\totrus"
set "LOCAL_CFG=%PROJECT%\title.cfg"

"C:\msys64\usr\bin\env.exe" MSYSTEM=MINGW32 CHERE_INVOKING=1 /usr/bin/bash -lc "export PS2DEV=/f/aa_development_ps2/totrus/ps2dev; export PS2SDK=$PS2DEV/ps2sdk; export GSKIT=$PS2DEV/gsKit; export PATH=$PS2DEV/bin:$PS2DEV/ee/bin:$PS2DEV/iop/bin:$PS2DEV/dvp/bin:$PS2SDK/bin:$PATH; cd /f/aa_development_ps2/totrus && make clean && make"

if errorlevel 1 (
    echo.
    echo ========================================
    echo          ERROR AL COMPILAR
    echo ========================================
    echo.
    pause
    exit /b 1
)

echo title=Totrus> "%LOCAL_CFG%"
echo boot=TOTRUS.ELF>> "%LOCAL_CFG%"

echo.
echo Compilacion OK.
echo Config local actualizado:
echo %LOCAL_CFG%
echo.
echo Buscando pendrive LONE WOLF...

echo.
set "DRIVEFILE=%TEMP%\totrus_drive.txt"
del "%DRIVEFILE%" 2>nul

powershell.exe -NoProfile -ExecutionPolicy Bypass -Command "$v=Get-Volume -FileSystemLabel 'LONE WOLF' -ErrorAction SilentlyContinue; if($v -and $v[0].DriveLetter){$v[0].DriveLetter}" > "%DRIVEFILE%"

set "USB="
set /p USB=<"%DRIVEFILE%"
del "%DRIVEFILE%" 2>nul

if not defined USB (
    echo No encontre el pendrive LONE WOLF.
    echo.
    echo El build local quedo listo en:
    echo %PROJECT%\TOTRUS.ELF
    echo %LOCAL_CFG%
    echo.
    pause
    exit /b 0
)

set "USB=%USB%:"

echo Encontrado: %USB% [LONE WOLF]
echo.

if not exist "%USB%\APPS" mkdir "%USB%\APPS"
if not exist "%USB%\APPS\TOTRUS" mkdir "%USB%\APPS\TOTRUS"

copy /Y "%PROJECT%\TOTRUS.ELF" "%USB%\APPS\TOTRUS\TOTRUS.ELF" >nul
if errorlevel 1 (
    echo.
    echo ERROR copiando TOTRUS.ELF al pendrive.
    echo.
    pause
    exit /b 1
)

copy /Y "%LOCAL_CFG%" "%USB%\APPS\TOTRUS\title.cfg" >nul
if errorlevel 1 (
    echo.
    echo ERROR copiando title.cfg al pendrive.
    echo.
    pause
    exit /b 1
)

echo.
echo ========================================
echo              TODO LISTO
echo ========================================
echo.
echo Build local:
echo %PROJECT%\TOTRUS.ELF
echo %LOCAL_CFG%
echo.
echo Copiado a:
echo %USB%\APPS\TOTRUS\TOTRUS.ELF
echo %USB%\APPS\TOTRUS\title.cfg
echo.
pause
