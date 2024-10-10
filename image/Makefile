CC := gcc
CFLAGS := -Wall -Wextra -pedantic
LDLIBS := -lSDL2 -lSDL2_image -lm

colors : colors.o
colors.o : colors.c

rotation : rotation.o
rotation.o : rotation.c

clean :
	$(RM) colors colors.o rotation rotation.o
