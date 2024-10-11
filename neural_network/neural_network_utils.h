#pragma once


void save_weights_and_biases(const char* filename, neural_network *nn);

void load_weights_and_biases(const char* filename, neural_network *nn);

void print_weights_and_biases(neural_network *nn );