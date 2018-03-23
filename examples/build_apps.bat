SET CC=gcc
SET CP=copy
SET RM=del

%CC% app.c -o app.exe -I..\include\ -lm -lmingw32 ..\lib\libgfx1.a
%CC% app2.c -o app2.exe -I..\include\ -lm -lmingw32 ..\lib\libgfx1.a
%CC% raycast.c -o raycast.exe -I..\include\ -lm -lmingw32 ..\lib\libgfx1.a
%CP% *.exe ..\bin
%RM% *.exe
pause