OBJS = gl_utils.o gl_labs.o gl_main.o
CC = g++
CFLAGS = -Wall -O -g
LIB_FLAGS = -lGL -lGLU -lglut

test : $(OBJS)
		$(CC) $^ -o $@ $(CFLAGS) $(LIB_FLAGS)
		./$@
%.o : %.c
		$(CC) $< -c $(CFLAGS) 

clean : 
		rm *.o test