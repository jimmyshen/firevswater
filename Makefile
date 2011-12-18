CC		= gcc
LD      = gcc
RM      = rm -f
INCLUDE = include
TARGET	= fvw

SRCS = src/fvw_core.c   \
	   src/fvw_render.c \
	   src/fvw_game.c   \
	   src/fvw_main.c

OBJS = ${SRCS:.c=.o}

GEN_RES = res/make_header.sh
RES     = res/fvw_resources.h

ifdef FVW_DEBUG
DEBUG_FLAGS = -g -DDEBUG_MODE
endif

ifdef FVW_EMBED_RES
EMBED_FLAGS = -DFVW_EMBED_RES -Ires
endif

CFLAGS	= -Wall `sdl-config --cflags` -I${INCLUDE} ${DEBUG_FLAGS} ${EMBED_FLAGS}
LDFLAGS	= `sdl-config --libs` -lSDL_image

.PHONY: ${RES}

.c.o:
	${CC} -o $@ ${CFLAGS} -c $<

all: ${RES} ${OBJS}
	${LD} -o ${TARGET} ${LDFLAGS} ${OBJS}

${RES}:
ifdef FVW_EMBED_RES
	${GEN_RES}
endif

clean:
	${RM} ${RES}
	${RM} ${OBJS}
	${RM} ${TARGET}

