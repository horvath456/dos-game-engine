cls
set path=Z:\;C:\WATCOM\BINW;C:\DOSGAME\INCLUDE;D:\DOSGAME\INCLUDE
SET INCLUDE=C:\WATCOM\H;C:\DOSGAME\INCLUDE;D:\DOSGAME\INCLUDE
SET WATCOM=C:\WATCOM
SET EDPATH=C:\WATCOM\EDDAT
SET WIPFC=C:\WATCOM\WIPFC
rescan
del main.exe
mkdir build
del build\*.*
cd build
wpp -4 -fp5 -ml -xs -obehi+l+mrt -zq ..\src\mouse.cpp