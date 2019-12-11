cls
set path=Z:\;C:\WATCOM\BINW;C:\DOSGAME\INCLUDE
SET INCLUDE=C:\WATCOM\H;C:\DOSGAME\INCLUDE
SET WATCOM=C:\WATCOM
SET EDPATH=C:\WATCOM\EDDAT
SET WIPFC=C:\WATCOM\WIPFC
rescan
mkdir build
del build\*.*
cd build
wpp -ml ..\src\adlib.cpp 
wpp -ml ..\src\animobj.cpp
wpp -ml ..\src\drawobj.cpp
wpp -ml ..\src\keyboard.cpp
wpp -ml ..\src\main.cpp
wpp -ml ..\src\palette.cpp
wpp -ml ..\src\player.cpp
wpp -ml ..\src\screen.cpp
wpp -ml ..\src\timer.cpp
wlink @..\proj.lnk
cd ..
copy build\main.exe main.exe