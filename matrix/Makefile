
SRC=matrix.c main.c

all : $(SRC)
	gcc -Wall -Wextra -pedantic -Werror -std=c99 -o matrix $(SRC) -g -fsanitize=address
	./matrix
val:$(SRC)
	gcc -Wall -Wextra -pedantic -Werror -std=c99 -o matrix $(SRC) -g
	valgrind ./matrix
clean:
	rm -rf matrix
