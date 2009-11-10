NAME = demo

ifdef PANDORA

CC        = arm-none-linux-gnueabi-gcc
STRIP     = arm-none-linux-gnueabi-strip
LDFLAGS   = -g -L$(PNDSDK)/usr/lib
CFLAGS    = -Wall -O3 -march=armv7-a -mcpu=cortex-a8 -mtune=cortex-a8 -mfloat-abi=softfp \
		   -mfpu=neon -ftree-vectorize -ffast-math -fomit-frame-pointer -fno-strict-aliasing -fsingle-precision-constant\
			-I$(PNDSDK)/usr/include -D_PANDORA_
LIBS	  = -lGLES2D -lSDL -lSDL_image -lSDL_ttf -lGLES_CM -lIMGegl -lsrv_um -ltiff -ljpeg -lpng -lfreetype -lts -lX11 -lXau -lXdmcp -lz

else

STRIP     = strip
LDFLAGS   = -g -L/usr/lib
CFLAGS    = -g -Wall -I/usr/include -D_X11_
LIBS	  = -lGLES2D -lSDL -lSDL_image -lSDL_ttf -lGLES_CM -ltiff 

endif

OBJECTS   = main.o 

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@ 
	$(STRIP) $(NAME)

.cc.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(NAME)
