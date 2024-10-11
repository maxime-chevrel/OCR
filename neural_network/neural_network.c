#include<unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <err.h>
#include <getopt.h>
#include <string.h>


#include "neural_network.h"
#include "train_neural_network.h"
#include "neural_network_utils.h"



double init_weights() {
  return ((double)rand()) / ((double)RAND_MAX);
}



void init_neural_network(neural_network * nn,double lr, char * filename){
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

  load_weights_and_biases(filename,nn);

}



void predict(neural_network * nn,double a, double b){
  if(!((a==1 || a==0) && (b==1 || b==0)))
    errx(1,"not good value for predict");
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


const char * print_help_page=
  "Usage: [options]\n"
  "[TRAIN MODE OPTIONS]\n"
  "   -n nb , --training-number nb  :  number of training iterations (def : 1 000 000)\n"
  "   -i file , --input-file file   :  file where is stored the neural network (def : weights_and_biases.bin)\n"
  "   -o file , --output-file file  :  file where to save the neural network (def : weights_and_biases.bin)\n"
  "   -l nb , --learning-rate       :  learning rate (def : 0.1)\n"
  "   -p , --print-training      :  print result while training\n"
  "[PREDICT MODE OPTIONS]\n"
  "   -i file , --input-file file   :  file where is stored the neural network (def : weights_and_biaises.bin)\n"
  "   -a n1,n2 , --arg n1,n2        :  argument 1 and 2 for the predict can only be 0 and 1\n"
  "[GENERAL OPTIONS]\n"
  "   -m mode , --mode mode         : switch between the mode PREDICT and TRAIN (def : TRAIN)\n"
  "   -h , --help                   : show this help page\n";


/*
int main(int argc, char **argv){
  neural_network nn;
  init_neural_network(&nn, 0.1);
  print_help_page("blase");
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
*/

int main(int argc, char ** argv) {
  int opt;
  int option_index = 0;

  // Struct for long opt
  struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"mode", required_argument, 0, 'm'},
    {"input-file", required_argument, 0, 'i'},
    {"output-file", required_argument, 0, 'o'},
    {"learning-rate", required_argument, 0, 'l'},
    {"training-number", required_argument, 0, 'n'},
    {"print-training", no_argument, 0, 'p'},
    {"arg", required_argument, 0, 'a'},
    {0, 0, 0, 0}
  };

  struct cust_options {
    char *mode;
    char *input_file;
    char *output_file;
    double learning_rate;
    int print_training;
    double arg1;
    double arg2;
    long int training_number;
  };

  struct cust_options opts = {
    .mode = "TRAIN",
    .input_file = "weights_and_biases.bin",
    .output_file = "weights_and_biases.bin",
    .learning_rate = 0.1,
    .print_training = 0,
    .arg1 = 0,
    .arg2 = 0,
    .training_number = 1000000

  };

  while ((opt = getopt_long(argc, argv, "hm:a:i:l:o:n:p", long_options, &option_index)) != -1) {
    switch (opt) {
      case 'h':
        printf("%s",print_help_page);
        return EXIT_SUCCESS;

      case 'm':
        opts.mode = optarg;
        if((strcmp(opts.mode,"TRAIN")!=0) && (strcmp(opts.mode,"PREDICT")!=0))
          errx(EXIT_FAILURE,"invalid argument for -m , -mode : refer to -h , -help");
        break;

      case 'a':
      {
        char *token = strtok(optarg, ",");
        if (token != NULL) {
          double temp = atof(token);
          if (temp ==0 || temp == 1) {
            opts.arg1 = temp;
          }
          else
            errx(EXIT_FAILURE,"invalid argument for -a , -arg : refer to -h , -help");
          token = strtok(NULL, ",");
          if (token != NULL) {
            temp = atof(token);
            if (temp ==0 || temp == 1) {
              opts.arg2 = temp;
            }
            else
              errx(EXIT_FAILURE,"invalid argument for -a , -arg : refer to -h , -help");
            opts.arg2 = atof(token);
          }
          else {
            errx(EXIT_FAILURE, "invalid argument for -a , -arg : refer to -h , -help");
          }
        }
        else {
          errx(EXIT_FAILURE, "invalid argument for -a , -arg : refer to -h , -help");
        }
      }
      break;

      case 'i':
        opts.input_file = optarg;
        break;

      case 'o':
        opts.output_file = optarg;
        break;

      case 'l':
        opts.learning_rate = atof(optarg);
        break;

      case 'n':
        opts.training_number = atof(optarg);
        break;

      case 'p':
        opts.print_training = 1;
        break;
      default:
        errx(EXIT_FAILURE,"invalid format : refer to -h , -help");

    }
  }
  neural_network nn;
  init_neural_network(&nn,opts.learning_rate, opts.input_file);
  if (strcmp(opts.mode, "TRAIN") == 0) {
    train(&nn, opts.output_file, opts.training_number, opts.print_training);
    return EXIT_SUCCESS;
  } else if (strcmp(opts.mode, "PREDICT") == 0) {
    predict(&nn, opts.arg1, opts.arg2);
    return EXIT_SUCCESS;
  } else {
    errx(EXIT_FAILURE, "invalid mode : refer to -h , -help");
  }
}

