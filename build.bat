@echo off

SET CC=gcc
SET AR=ar
SET CP=copy
SET CFLAGS=-O2 -I..\include
SET RM=del
SET CD=cd

%CD% ./libgfx

%CC% %CFLAGS% -c pa_interface.c -o pa_interface.o

%CC% %CFLAGS% -c libgfx.c -o libgfx.o
%CC% %CFLAGS% -c gfx_caption.c -o gfx_caption.o
%CC% %CFLAGS% -c gfx_clear.c -o gfx_clear.o
%CC% %CFLAGS% -c gfx_color.c -o gfx_color.o
%CC% %CFLAGS% -c gfx_flush.c -o gfx_flush.o
%CC% %CFLAGS% -c gfx_input.c -o gfx_input.o
%CC% %CFLAGS% -c gfx_iskey.c -o gfx_iskey.o
%CC% %CFLAGS% -c gfx_lock.c -o gfx_lock.o
%CC% %CFLAGS% -c gfx_ok.c -o gfx_ok.o
%CC% %CFLAGS% -c gfx_screen.c -o gfx_screen.o
%CC% %CFLAGS% -c gfx_sleep.c -o gfx_sleep.o
%CC% %CFLAGS% -c gfx_ticks.c -o gfx_ticks.o
%CC% %CFLAGS% -c gfx_update.c -o gfx_update.o
%CC% %CFLAGS% -c gfx_pixel.c -o gfx_pixel.o
%CC% %CFLAGS% -c gfx_font.c -o gfx_font.o
%CC% %CFLAGS% -c gfx_putc.c -o gfx_putc.o
%CC% %CFLAGS% -c gfx_printf.c -o gfx_printf.o
%CC% %CFLAGS% -c gfx_raster.c -o gfx_raster.o
%CC% %CFLAGS% -c gfx_image.c -o gfx_image.o
%CC% %CFLAGS% -c gfx_init.c -o gfx_init.o
%CC% %CFLAGS% -c gfx_text_cursor.c -o gfx_text_cursor.o
%CC% %CFLAGS% -c gfx_resize.c -o gfx_resize.o
%CC% %CFLAGS% -c gfx_saveshot.c -o gfx_saveshot.o
%CC% %CFLAGS% -c gfx_audio.c -o gfx_audio.o

%CC% %CFLAGS% -c __gfx_backend.c -o __gfx_backend.o

%AR% -rvs libgfx.a *.o

REM Copy headers
REM Portaudio and SDL headers
%CP% ..\portaudio\include\*.h ..\include
%CP% ..\sdl-1.2\include\*.h ..\include\

REM Copy the library
%CP% libgfx.a ..\lib

%RM% *.o
%CD% ..

REM Merge libraries now
%CD% lib
%AR% -rcT ./libgfx1.a ./libgfx.a ./libportaudio.dll.a ./libSDLmain.a ./libSDL.dll.a 

%CD% ..
pause