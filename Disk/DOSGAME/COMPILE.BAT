cls
set path=Z:\;C:\WATCOM\BINW;C:\DOSGAME\INCLUDE
SET INCLUDE=C:\WATCOM\H;C:\DOSGAME\INCLUDE
SET WATCOM=C:\WATCOM
SET EDPATH=C:\WATCOM\EDDAT
SET WIPFC=C:\WATCOM\WIPFC
rescan
del main.exe
mkdir build
del build\*.*
cd build
wpp -4 -fp5 -ml -xs -obehi+l+mprt -zq ..\src\adlib.cpp 
wpp -4 -fp5 -ml -xs -obehi+l+mprt -zq ..\src\animobj.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mprt -zq ..\src\drawobj.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mprt -zq ..\src\keyboard.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mprt -zq ..\src\main.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mprt -zq ..\src\palette.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mprt -zq ..\src\player.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mprt -zq ..\src\screen.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mprt -zq ..\src\timer.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mprt -zq ..\src\font.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mprt -zq ..\src\text.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mprt -zq ..\src\mouse.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mprt -zq ..\src\joystick.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mprt -zq ..\src\random.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mprt -zq ..\src\level.cpp
wlink @..\proj.lnk
cd ..
copy build\main.exe main.exe