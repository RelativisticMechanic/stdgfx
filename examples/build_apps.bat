SET CC=gcc
SET CP=copy
SET RM=del

%CC% app.c -o app.exe -I..\include\ ..\lib\libgfx.a ..\lib\libportaudio.dll.a -lm -lmingw32 -lSDLmain -lSDL
%CC% app2.c -o app2.exe -I..\include\ ..\lib\libgfx.a ..\lib\libportaudio.dll.a -lm -lmingw32 -lSDLmain -lSDL
%CC% raycast.c -o raycast.exe -I..\include\ ..\lib\libgfx.a ..\lib\libportaudio.dll.a -lm -lmingw32 -lSDLmain -lSDL
%CP% *.exe ..\bin
%RM% *.exe
pause