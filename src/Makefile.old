# GLES 2D Makefile

SRC =  	GLES2D_video.o GLES2D_texture.o GLES2D_control.o GLES2D_control_pnd.o \
		GLES2D_font.o GLES2D_drawing.o GLES2D_utils.o GLES2D_collision.o GLES2D_sprite.o \
		GLES2D_tiles.o GLES2D_particle.o

OBJ = $(SRC:.c=.o)

OUT = libGLES2D.a

CFLAGS		= -g -Wall -Werror
INCLUDES	= -I. -Iinclude

ifdef PANDORA

CC		= arm-none-linux-gnueabi-gcc
CXX		= arm-none-linux-gnueabi-g++
AR		= arm-none-linux-gnueabi-ar
CFLAGS		+= -D_PANDORA_ -O3 -march=armv7-a -mcpu=cortex-a8 -mtune=cortex-a8 -mfloat-abi=softfp \
		   -mfpu=neon -ftree-vectorize -ffast-math -fomit-frame-pointer -fno-strict-aliasing -fsingle-precision-constant
INCLUDES	+= -I$(PNDSDK)/usr/include
INSTALL		= cp libGLES2D.a $(PNDSDK)/usr/lib && cp include/GLES2D/* $(PNDSDK)/usr/include/GLES2D/

else

CFLAGS		+= -D_X11_
INCLUDES	+= -I/usr/include
INSTALL		= sudo cp libGLES2D.a /usr/lib && sudo cp include/GLES2D/* /usr/include/GLES2D/

endif

.SUFFIXES: .c

default: $(OUT)

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

.cpp.o:
	$(CXX) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	$(AR) rcs $(OUT) $(OBJ)
	$(INSTALL)
	

clean:
	rm -f $(OBJ) $(OUT) 

