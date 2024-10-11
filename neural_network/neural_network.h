#pragma once 


// Constantes globales
#define numInputs 2
#define numHiddenNodes 2
#define numOutputs 1
#define numTrainingSets 4

// Définition de la structure neural_network
typedef struct neural_network
{
    double lr; //learning rate

    double hiddenLayer[numHiddenNodes];
    double outputLayer[numOutputs];

    double hiddenLayerBias[numHiddenNodes];
    double outputLayerBias[numOutputs];

    double hiddenWeights[numInputs][numHiddenNodes];
    double outputWeights[numHiddenNodes][numOutputs];
}neural_network;


double init_weights();

void init_neural_network(neural_network * nn, double lr);

void predict(neural_network * nn,double a, double b);