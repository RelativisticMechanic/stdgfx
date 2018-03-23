SET CC=gcc
SET CP=copy
SET RM=del
%CC% asteroids.c -o asteroids.exe -I..\include\ -lmingw32 -lm ..\lib\libgfx1.a
%CP% *.exe ..\bin\
%RM% *.exe
pause