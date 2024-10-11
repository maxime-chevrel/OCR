CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Werror -std=c99 -g
SRC = neural_network.c neural_network_utils.c train_neural_network.c
OBJ = $(SRC:.c=.o)

all: neuralnetwork.ocr

neuralnetwork.ocr: $(OBJ)
	$(CC) $(CFLAGS) -fsanitize=address -o $@ $(OBJ) -lm

val: $(OBJ)
	$(CC) $(CFLAGS) -o neuralnetwork.ocr $(OBJ) -lm

test: $(OBJ)
	$(CC) $(CFLAGS) -O3 -o test.ocr $(OBJ) -lm

clean:
	rm -f neuralnetwork.ocr test.ocr $(OBJ)


