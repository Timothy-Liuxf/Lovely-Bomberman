@echo off
SETLOCAL
set returnval=0

mkdir 可爱炸弹人_Product(x64)

copy /Y x64\Release\可爱炸弹人(x64).exe 可爱炸弹人_Product(x64)\可爱炸弹人(x64).exe || set returnval=1 && echo 发布 可爱炸弹人(x64).exe 失败
copy /Y x64\Release\GameSound(x64).exe 可爱炸弹人_Product(x64)\GameSound(x64).exe || set returnval=1 && echo 发布 GameSound(x64).exe 失败
copy /Y x64\Release\Images(x64).dll 可爱炸弹人_Product(x64)\Images(x64).dll || set returnval=1 && echo 发布 Images(x64).dll 失败

if %returnval% EQU 0 (
    echo 发布成功！
)

pause

EXIT /B %returnval%
