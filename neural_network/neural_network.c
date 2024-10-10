#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <err.h>
#include <string.h>

#include "neural_network.h"
#include "train_neural_network.h"
#include "neural_network_utils.h"

#define OPTSTR "hvi:o:n:m:b:c:l:a:"

double init_weights() {
  return ((double)rand()) / ((double)RAND_MAX);
}



void init_neural_network(neural_network * nn,double lr){
  nn->lr = lr;

  for (size_t i = 0; i < numHiddenNodes; i++) {
    for (size_t j = 0; j < numOutputs; j++) {
      nn->outputWeights[i][j] = init_weights();
    }
  }
  for (size_t i = 0; i < numHiddenNodes; i++) {
    nn->hiddenLayerBias[i] = init_weights();
  }
  for (size_t i = 0; i < numOutputs; i++) {
    nn->outputLayerBias[i] = init_weights();
  }
  for (size_t i = 0; i < numInputs; i++) {
    for (size_t j = 0; j < numHiddenNodes; j++) {
      nn->hiddenWeights[i][j] = init_weights();
    }
  }

}



void predict(neural_network * nn,double a, double b){
  if(!((a==1 || a==0) && (b==1 || b==0)))
    errx(1,"valeur d'entrée de predict pas bonne");
  double inputs[2] = {a,b};
  for(size_t j = 0; j< numHiddenNodes; j++){
    double activation = nn->hiddenLayerBias[j];

    for(size_t k = 0; k < 2; k++){
      activation += inputs[k] * nn->hiddenWeights[k][j];
    }

    nn->hiddenLayer[j] = sigmoid(activation);
  }

  //Compute output layer activation
  for(size_t j = 0; j< numOutputs; j++){
    double activation = nn->outputLayerBias[j];

    for(size_t k = 0; k < numHiddenNodes; k++){
      activation += nn->hiddenLayer[k] * nn->outputWeights[k][j];
    }

    printf("Inputs : %g %g Ouputs : %g  \n",a,b,round(sigmoid(activation)));
  }
}


void print_help_page(char *name) {
  printf("Usage: %s [options]\n"
  "[TRAIN MODE OPTIONS]\n"
  "   -n nb    :  number of training iterations (def : 1 000 000)\n"
  "   -i file  :  file where is stored the neural network (def : weights_and_biaises.bin)\n"
  "   -o file  :  file where to save the neural network (def : weights_and_biaises.bin)\n"
  "   -l nb    :  learning rate (def : weights_and_biaises.bin)\n"
  "[PREDICT MODE OPTIONS]\n"
  "   -i file  :  file where is stored the neural network (def : weights_and_biaises.bin)\n"
  "   -a n,n   :  learning rate (def : weights_and_biaises.bin)\n"
  ,name);
}



int main(int argc, char **argv){
  neural_network nn;
  init_neural_network(&nn, 0.1);
  //print_help_page("blase");
  if(argc<2)
    errx(1,"lake of arguments");
  else if(strcmp(argv[1],"-t")==0){
    train(&nn);
    return 0;
  }
  else if(strcmp(argv[1],"-p")==0){
    if(argc<4)
      errx(1,"pas assez d'arguments pour predict");
    else
      predict(&nn,strtod(argv[2],NULL),strtod(argv[3],NULL));
  }
  else
    errx(1,"extension inconnu");
  return 0;
}
