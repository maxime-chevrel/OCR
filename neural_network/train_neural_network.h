#pragma once



double sigmoid(double x);

double dSigmoid(double x);

void train(neural_network * nn, char* outputfile, long int train_size, int print);

void shuffle(int *array, size_t n);
