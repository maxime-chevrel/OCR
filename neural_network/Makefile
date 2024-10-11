SRC=neural_network.c neural_network_utils.c train_neural_network.c

all : $(SRC)
	gcc -Wall -Wextra -pedantic -Werror -std=c99 -o neuralnetwork.ocr $(SRC) -g -fsanitize=address -lm
val:$(SRC)
	gcc -Wall -Wextra -pedantic -Werror -std=c99 -o neuralnetwork.ocr $(SRC) -g -lm
test:$(SRC)
	gcc -Wall -Wextra -pedantic -Werror -std=c99 -o test.ocr $(SRC) -g -lm -O3
clean:
	rm -rf neuralnetwork.ocr
	rm -rf test.ocr

