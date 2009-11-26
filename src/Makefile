
#
# GLES2D Makefile
#

# tools
CC = ${CROSSCOMPILE}gcc
CXX = ${CROSSCOMPILE}g++
AR = ${CROSSCOMPILE}ar
RANLIB = ${CROSSCOMPILE}ranlib
RM = rm

# environment
CFLAG_SO = -fPIC #-fPIC not always needed, but good to have
CFLAGS = -Wall -Werror -I./include -g ${CFLAG_SO}

ifdef CROSSCOMPILE
CFLAGS += -D_PANDORA_ -I$(PNDSDK)/usr/include -O3 -march=armv7-a -mcpu=cortex-a8 -mtune=cortex-a8 -mfloat-abi=softfp \
		-mfpu=neon -ftree-vectorize -ffast-math -fomit-frame-pointer -fno-strict-aliasing -fsingle-precision-constant
endif

# code
LIB = libGLES2D.a 
SOLIB = libGLES2D.so.1         # canonicle name
SOLIB1 = libGLES2D.so.1.0.1    # versioned name
ALLOBJ = GLES2D_video.o GLES2D_texture.o GLES2D_control.o GLES2D_control_pnd.o GLES2D_font.o \
		GLES2D_tiles.o GLES2D_drawing.o GLES2D_utils.o GLES2D_collision.o GLES2D_sprite.o \
		GLES2D_particle.o GLES2D_pvrtc.o

all: ${SOLIB} ${LIB}

clean:
	${RM} -f ${ALLOBJ} ${LIB} ${SOLIB1} ${SOLIB}
	${RM} -rf deployment
	find . -name "*~*" -exec rm {} \; -print

# component targets
#

libGLES2D.a:	${ALLOBJ}
	${AR} r ${LIB} ${ALLOBJ}
	${RANLIB} ${LIB}

libGLES2D.so.1:	${ALLOBJ}
	${CC} -shared -Wl,-soname,${SOLIB} -o ${SOLIB1} ${ALLOBJ}
	ln -f -s ${SOLIB1} ${SOLIB}

${SOLIB1}:	${ALLOBJ} 
	${CC} -shared -Wl,-soname,${SOLIB} -o ${SOLIB1} ${ALLOBJ}
	ln -f -s ${SOLIB1} ${SOLIB}

# deployment and assembly components
#

deploy: 
	# populate deployment directory for copying into image bakes
	# make dirs
	mkdir -p deployment/usr/lib
	mkdir -p deployment/usr/include/GLES2D
	# copy in goodies
	cp libGLES2D* deployment/usr/lib
	cp include/GLES2D/* deployment/usr/include/GLES2D

