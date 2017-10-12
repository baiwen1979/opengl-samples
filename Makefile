OBJS = gl_utils.o gl_labs.o gl_main.o
CC = g++
CFLAGS = -Wall -O -g
GL_LIB_FLAGS_DARWIN = -framework OpenGL -framework glut -framework foundation
GL_LIB_FLAGS = -lGL -lGLU -lglut
OS_NAME = $(shell uname)

ifeq ($(OS_NAME), Darwin)
	GL_LIB_FLAGS := $(GL_LIB_FLAGS_DARWIN)
endif

test : $(OBJS)
		$(CC) $^ -o $@ $(CFLAGS) $(GL_LIB_FLAGS)
		./$@
%.o : %.c
		$(CC) $< -c $(CFLAGS) 

clean : 
		rm $(OBJS) test