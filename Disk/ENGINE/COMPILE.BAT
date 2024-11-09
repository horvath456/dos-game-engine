cls
set path=Z:\;C:\WATCOM\BINW;C:\ENGINE\INCLUDE
SET INCLUDE=C:\WATCOM\H;C:\ENGINE\INCLUDE
SET WATCOM=C:\WATCOM
SET EDPATH=C:\WATCOM\EDDAT
SET WIPFC=C:\WATCOM\WIPFC
rescan
del main.exe
mkdir build
del build\*.* /Q
cd build
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\adlib.cpp 
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\animobj.cpp
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\draw.cpp
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\drawobj.cpp
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\font.cpp
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\joystick.cpp
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\keyboard.cpp
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\mouse.cpp
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\palette.cpp
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\random.cpp
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\screen.cpp
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\speaker.cpp
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\sprite.cpp
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\sprsheet.cpp
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\text.cpp
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\timer.cpp
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\tween.cpp

wlib engine +adlib +animobj +draw
wlib engine +drawobj +font +joystick
wlib engine +keyboard +mouse +palette
wlib engine +random +screen +speaker
wlib engine +sprite +sprsheet +text
wlib engine +timer +tween

mkdir dist
del dist\engine.lib
copy engine.lib dist\engine.lib
mkdir dist\include
del dist\include\*.h
copy ..\include\*.h dist\include

cd ..