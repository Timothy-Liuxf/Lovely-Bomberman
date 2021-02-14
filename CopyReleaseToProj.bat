@echo off
SETLOCAL
set returnval=0

copy /Y Release\GameSound.exe ø…∞Æ’®µØ»À\GameSound.exe || set returnval=1 && echo “∆∂Ø GameSound.exe  ß∞‹
copy /Y Release\Images.dll ø…∞Æ’®µØ»À\Images.dll || set returnval=1 && echo “∆∂Ø Images.dll  ß∞‹
copy /Y x64\Release\GameSound(x64).exe ø…∞Æ’®µØ»À\GameSound(x64).exe || set returnval=1 && echo “∆∂Ø GameSound(x64).exe  ß∞‹
copy /Y x64\Release\Images(x64).dll ø…∞Æ’®µØ»À\Images(x64).dll || set returnval=1 && echo “∆∂Ø Images(x64).dll  ß∞‹

if %returnval% EQU 0 (
    echo “∆∂Ø≥…π¶£°
)

pause

EXIT /B %returnval%
