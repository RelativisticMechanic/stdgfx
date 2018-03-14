SET CC=gcc
%CC% asteroids.c -o asteroids.exe -I..\include\ ..\lib\libgfx.a -lmingw32 -lSDLmain -lSDL
pause