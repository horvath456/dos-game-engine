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
wpp -ml -xs ..\src\adlib.cpp 
wpp -ml -xs ..\src\animobj.cpp
wpp -ml -xs ..\src\drawobj.cpp
wpp -ml -xs ..\src\keyboard.cpp
wpp -ml -xs ..\src\main.cpp
wpp -ml -xs ..\src\palette.cpp
wpp -ml -xs ..\src\player.cpp
wpp -ml -xs ..\src\screen.cpp
wpp -ml -xs ..\src\timer.cpp
wlink @..\proj.lnk
cd ..
copy build\main.exe main.exe