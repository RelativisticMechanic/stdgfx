SET CC=gcc
%CC% app.c -o app.exe -I..\include\ ..\lib\libgfx.a -lm -lmingw32 -lSDLmain -lSDL
%CC% app2.c -o app2.exe -I..\include\ ..\lib\libgfx.a -lm -lmingw32 -lSDLmain -lSDL
%CC% raycast.c -o raycast.exe -I..\include\ ..\lib\libgfx.a -lm -lmingw32 -lSDLmain -lSDL
pause