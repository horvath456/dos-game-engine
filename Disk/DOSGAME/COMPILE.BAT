cls
set path=Z:\;C:\WATCOM\BINW;C:\ENGINE\INCLUDE;C:\ENGINE\BUILD;C:\ENGINE\BUILD\DIST;C:\DOSGAME\INCLUDE;C:\ENGINE\BUILD\DIST\INCLUDE
SET INCLUDE=C:\WATCOM\H;C:\ENGINE\INCLUDE;C:\ENGINE\BUILD\DIST\INCLUDE;C:\DOSGAME\INCLUDE
SET WATCOM=C:\WATCOM
SET EDPATH=C:\WATCOM\EDDAT
SET WIPFC=C:\WATCOM\WIPFC
rescan
del main.exe
mkdir build
cd build
del main.obj
del main.exe
wpp -4 -fp2 -ml -xs -obehi+l+mrt -zq ..\src\main.cpp
wlink @..\proj.lnk
cd ..
copy build\main.exe main.exe