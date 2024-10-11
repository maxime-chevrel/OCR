#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <err.h>
#include <string.h>

#include "neural_network.h"
#include "neural_network_utils.h"
#include "train_neural_network.h"

// Function to save weights and bias
void save_weights_and_biases(const char* filename, neural_network *nn) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Unable to open file for saving");
        return;
    }
    
    // Save hidden weights
    fwrite(nn->hiddenWeights, sizeof(double), numInputs * numHiddenNodes, file);
    // Save output weights
    fwrite(nn->outputWeights, sizeof(double), numHiddenNodes * numOutputs, file);
    // Save hidden bias
    fwrite(nn->hiddenLayerBias, sizeof(double), numHiddenNodes, file);
    // Save output bias
    fwrite(nn->outputLayerBias, sizeof(double), numOutputs, file);
    
    fclose(file);
}

// Function to load weight and bias
void load_weights_and_biases(const char* filename, neural_network *nn) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Unable to open file for loading");
        return;
    }
    
    // Load hidden Bias
    if (fread(nn->hiddenWeights, sizeof(double), numInputs * numHiddenNodes, file) != numInputs * numHiddenNodes) {
        perror("Error reading hiddenWeights");
        fclose(file);
        return;
    }
    
    // Load output bias
    if (fread(nn->outputWeights, sizeof(double), numHiddenNodes * numOutputs, file) != numHiddenNodes * numOutputs) {
        perror("Error reading outputWeights");
        fclose(file);
        return;
    }
    
    // Load hidden bias
    if (fread(nn->hiddenLayerBias, sizeof(double), numHiddenNodes, file) != numHiddenNodes) {
        perror("Error reading hiddenLayerBias");
        fclose(file);
        return;
    }
    
    // load output bias
    if (fread(nn->outputLayerBias, sizeof(double), numOutputs, file) != numOutputs) {
        perror("Error reading outputLayerBias");
        fclose(file);
        return;
    }
    
    fclose(file);
}

void print_weights_and_biases(neural_network *nn ) {
    printf("Hidden Weights:\n");
    for (size_t i = 0; i < numInputs; i++) {
        for (size_t j = 0; j < numHiddenNodes; j++) {
            printf("%f ", nn->hiddenWeights[i][j]);
        }
        printf("\n");
    }
    
    printf("\nOutput Weights:\n");
    for (size_t i = 0; i < numHiddenNodes; i++) {
        for (size_t j = 0; j < numOutputs; j++) {
            printf("%f ", nn->outputWeights[i][j]);
        }
        printf("\n");
    }
    
    printf("\nHidden Layer Biases:\n");
    for (size_t i = 0; i < numHiddenNodes; i++) {
        printf("%f ", nn->hiddenLayerBias[i]);
    }
    printf("\n");
    
    printf("\nOutput Layer Biases:\n");
    for (size_t i = 0; i < numOutputs; i++) {
        printf("%f ", nn->outputLayerBias[i]);
    }
    printf("\n");
}