@echo off
setlocal enabledelayedexpansion

set PROJROOT=e:\Aixun Smoc\esp32_tes02_ctrl
set PORT=COM70
set RECOVERY=%PROJROOT%\.pio\build\esp32c3_recovery\firmware.bin

echo.
echo ========================================
echo ESP32-C3 Recovery Image Flash Tool
echo ========================================
echo.
echo This will flash the Recovery factory image to 0x320000
echo.
echo IMPORTANT: Put ESP32-C3 into bootloader mode FIRST:
echo   1. Hold BOOT button
echo   2. Click RESET or disconnect/reconnect USB
echo   3. Release BOOT
echo.
pause

if not exist "%RECOVERY%" (
    echo Building recovery firmware...
    pio run -e esp32c3_recovery
    if errorlevel 1 (
        echo ERROR: recovery build failed
        exit /b 1
    )
)

echo.
echo Flashing Recovery image to %PORT% ...
echo Command: python -m esptool write_flash 0x320000 "%RECOVERY%"
echo.

REM Find usable Python with esptool
for %%p in (py python python.exe) do (
    echo Trying %%p...
    %%p -m esptool --chip esp32c3 --port %PORT% --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m 0x320000 "%RECOVERY%"
    if !errorlevel! equ 0 goto success
)

goto fail

:success
echo.
echo ✓ SUCCESS! Recovery image flashed to ESP32-C3
echo   Flash address: 0x320000
echo   Size: 256 KB
echo   Device will reboot automatically
echo.
pause
exit /b 0

:fail
echo.
echo ✗ FAILED: Could not flash Recovery image
echo   Check if device is in bootloader mode
echo   Check if port %PORT% is correct
echo.
pause
exit /b 1
