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



void init_neural_network(neural_network * nn){
  nn->lr = 0.1;

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



double predict(double a, double b){
  if(!((a==1 || a==0) && (b==1 || b==0)))
    errx(1,"valeur d'entré de predict pas bonne");
  printf("fais la fonction connard\n entrée 1 : %g entrée 2 : %g\n",a , b );
  return a;
}


static const char* ARGS_HELP =
    "%s 3.14.15 help:\n"
    "[ IMAGE mode specific options ]\n"
    "   -i file: Specifiy the input file (required)\n"
    "   -o file: Specify the output file (default: out.bmp)\n"
    "   --show: Show the image being processed, one step at a time\n"
    "[ TRAIN mode specific options ]\n"
    "   -n nb: Specifiy the number of iterations to train the neural net with (default is 100 000)\n"
    "   -o file: Specify the output file to save the neural network\n"
    "   --batch-size n / -b n: Specify the numbers of elements in a minibatch size (default 100)\n"
    "   --nb-images n: Specify the number of image to train with. (default 8228)\n"
    "   --learning-rate n / --step-size n / -l n: Specify the step size (default 0.25)\n"
    "[ PREDICT mode specific options ]\n"
    "   -i file: Specify the image file to predict the digit\n"
    "   -a file: Specify the file containing the weights and biais of the neural network\n"
    "[ SOLVE mode specific options ]\n"
    "   -i file: Specify the input file containing the grid to solve (default: grid.txt)"
    "[ General options ]\n"
    "   -v: Increase the verbose level (default 0), can be used up to 3 times\n"
    "   --mode mode: Specify the mode to use. Can be one of IMAGE/TRAIN/GUI/PREDICT/SOLVE (default is GUI)\n"
    "   -h / --help: Show this help and quit\n"
    ;



int main(int argc, char **argv){
  neural_network nn;
  init_neural_network(&nn);
  if(argc<2)
    errx(1,"manque d'arguments");
  else if(strcmp(argv[1],"-t")==0){
    train(&nn);
    return 0;
  }
  else if(strcmp(argv[1],"-p")==0){
    if(argc<4)
      errx(1,"pas assez d'arguments pour predict");
    else
      predict(strtod(argv[2],NULL),strtod(argv[3],NULL));
  }
  else
    errx(1,"extension inconnu");
  return 0;
}
