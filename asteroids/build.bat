SET CC=gcc
SET CP=copy
SET RM=del
%CC% asteroids.c -o asteroids.exe -I..\include\ ..\lib\libgfx.a ..\lib\libportaudio.dll.a -lmingw32 -lSDLmain -lSDL
%CP% *.exe ..\bin\
%RM% *.exe
pause