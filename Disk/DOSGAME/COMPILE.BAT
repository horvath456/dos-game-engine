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
wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\adlib.cpp 
wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\animobj.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\drawobj.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\keyboard.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\palette.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\screen.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\timer.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\font.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\text.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\mouse.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\joystick.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\speaker.cpp
wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\random.cpp
wlib engine +adlib +animobj +drawobj 
wlib engine +keyboard +palette +screen 
wlib engine +timer +font +text +mouse 
wlib engine +joystick +speaker +random

mkdir dist
del dist\engine.lib
copy engine.lib dist\engine.lib
mkdir dist\include
del dist\include\*.h
copy ..\include\*.h dist\include

wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\main.cpp
wlink @..\proj.lnk
cd ..
copy build\main.exe main.exe