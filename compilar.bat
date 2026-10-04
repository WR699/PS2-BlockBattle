@echo off
setlocal EnableExtensions EnableDelayedExpansion
chcp 65001 >nul

title Block Battle - PlayStation 2 Build

rem Todo parte de la ubicacion real de compilar.bat.
set "ROOT=%~dp0"
if "%ROOT:~-1%"=="\" set "ROOT=%ROOT:~0,-1%"

if not exist "%ROOT%\Makefile" (
    echo ERROR: no encontre Makefile junto a compilar.bat.
    pause
    exit /b 1
)

rem Si falta cualquier parte del entorno local, instalarla automaticamente.
if not exist "%ROOT%\tools\msys64\usr\bin\bash.exe" goto :install
if not exist "%ROOT%\tools\ps2dev\ee\bin\mips64r5900el-ps2-elf-gcc.exe" goto :install
goto :environment_ready

:install
echo No se encontro el entorno local completo. Ejecutando install.bat...
call "%ROOT%\install.bat"
if errorlevel 1 exit /b 1

:environment_ready
call :map_root
if errorlevel 1 goto :fatal

set "SAFE_ROOT=!VDRIVE!:\"
set "MSYS=!SAFE_ROOT!tools\msys64"
set "BUILD=!SAFE_ROOT!build"
set "ROOT_UNIX=/!VDRIVE!"
set "PS2DEV_UNIX=/!VDRIVE!/tools/ps2dev"

echo.
echo ========================================
echo          BLOCK BATTLE - PS2 BUILD
echo ========================================
echo.

"!MSYS!\usr\bin\env.exe" MSYSTEM=MINGW32 CHERE_INVOKING=1 "!MSYS!\usr\bin\bash.exe" -lc "export PS2DEV='!PS2DEV_UNIX!'; export PS2SDK=$PS2DEV/ps2sdk; export GSKIT=$PS2DEV/gsKit; export PATH=$PS2DEV/bin:$PS2DEV/ee/bin:$PS2DEV/iop/bin:$PS2DEV/dvp/bin:$PS2SDK/bin:$PATH; cd '!ROOT_UNIX!' && make clean && make"
set "BUILD_RESULT=!ERRORLEVEL!"

if not "!BUILD_RESULT!"=="0" (
    call :unmap_root
    echo.
    echo ========================================
    echo          ERROR AL COMPILAR
    echo ========================================
    echo.
    pause
    exit /b !BUILD_RESULT!
)

if not exist "!BUILD!\BLOCKBATTLE.ELF" (
    call :unmap_root
    echo ERROR: make termino sin error pero no existe build\BLOCKBATTLE.ELF.
    pause
    exit /b 1
)

rem El build local queda listo para copiar directamente a APPS\BLOCKBATTLE.
if not exist "!BUILD!" mkdir "!BUILD!"
> "!BUILD!\title.cfg" echo title=BlockBattle
>>"!BUILD!\title.cfg" echo boot=BLOCKBATTLE.ELF

echo.
echo Build local listo:
echo   !BUILD!\BLOCKBATTLE.ELF
echo   !BUILD!\title.cfg

rem ============================================================
rem Deploy OPCIONAL. PowerShell hace todo el deploy directamente.
rem No encontrar PS2128 nunca invalida el build.
rem ============================================================
echo.
powershell.exe -NoProfile -ExecutionPolicy Bypass -Command ^
  "$v=$null; foreach($candidate in (Get-Volume -ErrorAction SilentlyContinue)){ if($candidate.FileSystemLabel -eq 'PS2128' -and $candidate.DriveLetter){ $v=$candidate; break } }; if($null -eq $v){ Write-Host 'PS2128 no esta conectado. Se omite el deploy; la compilacion fue exitosa.'; exit 0 }; $dest=($v.DriveLetter + ':\APPS\BLOCKBATTLE'); Write-Host ('PS2128 encontrado en ' + $v.DriveLetter + ': - intentando deploy...'); $null=New-Item -ItemType Directory -Force -Path $dest; Copy-Item -LiteralPath '%ROOT%\build\BLOCKBATTLE.ELF' -Destination (Join-Path $dest 'BLOCKBATTLE.ELF') -Force -ErrorAction Stop; Copy-Item -LiteralPath '%ROOT%\build\title.cfg' -Destination (Join-Path $dest 'title.cfg') -Force -ErrorAction Stop; Write-Host ('Deploy actualizado: ' + $dest)"
set "DEPLOY_RESULT=!ERRORLEVEL!"
if not "!DEPLOY_RESULT!"=="0" (
    echo AVISO: el build local esta bien, pero fallo el deploy opcional a PS2128.
)

call :unmap_root

echo.
echo ========================================
echo          BUILD COMPLETADO
echo ========================================
echo.
echo Salida local: build\BLOCKBATTLE.ELF + build\title.cfg
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

:fatal
echo.
echo No se pudo preparar una ruta temporal segura para Block Battle.
pause
exit /b 1
