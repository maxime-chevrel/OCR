#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <err.h>
#include <string.h>

#include "neural_network.h"
#include "neural_network_utils.h"
#include "train_neural_network.h"



double sigmoid(double x){
  return 1/ (1 + exp(-x));
}

double dSigmoid(double x){
  return x * (1-x);
}

void shuffle(int *array, size_t n){
  if(n>1){
    for(size_t i = 0 ; i < n-1 ; i++){
      size_t j = i + rand() / (RAND_MAX / (n-i) + 1);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

void train(neural_network * nn, char* outputfile, long int train_size, int print){




  double training_inputs[numTrainingSets][numInputs] = {{0.0f,0.0f},
                                                        {1.0f,0.0f},
                                                        {0.0f,1.0f},
                                                        {1.0f,1.0f}};


  double training_outputs[numTrainingSets][numOutputs] = {{1.0f},
                                                          {0.0f},
                                                          {0.0f},
                                                          {1.0f}};

  
  //load_weights_and_biases(inputfile, nn);

  int trainingSetOrder[] = {0,1,2,3};

  long int numberOfEpochs = train_size;

  // Train the neural Network
  
  for(long int epoch = 0; epoch < numberOfEpochs; epoch++) {
    //    printf("%ld\n", epoch);
    shuffle(trainingSetOrder, numTrainingSets);

      for (int x=0; x<numTrainingSets; x++) {

        int i = trainingSetOrder[x];

        // Forward pass

        // Compute hidden layer activation
        for (int j=0; j<numHiddenNodes; j++) {
            double activation = nn->hiddenLayerBias[j];
             for (int k=0; k<numInputs; k++) {
                activation += training_inputs[i][k] * nn->hiddenWeights[k][j];
            }
            nn->hiddenLayer[j] = sigmoid(activation);
        }

        // Compute output layer activation
        for (int j=0; j<numOutputs; j++) {
            double activation = nn->outputLayerBias[j];
            for (int k=0; k<numHiddenNodes; k++) {
                activation += nn->hiddenLayer[k] * nn->outputWeights[k][j];
            }
            nn->outputLayer[j] = sigmoid(activation);
        }

        // Print the results from forward pass
        if(print==1)
            printf("Input:%g %g Output:%g    Expected Output: %g\n",
                training_inputs[i][0], training_inputs[i][1],
                nn->outputLayer[0], training_outputs[i][0]);



        // Backprop

        // Compute change in output weights
        double deltaOutput[numOutputs];
        for (int j=0; j<numOutputs; j++) {
            double errorOutput = (training_outputs[i][j] - nn->outputLayer[j]);
            deltaOutput[j] = errorOutput * dSigmoid(nn->outputLayer[j]);
        }

        // Compute change in hidden weights
        double deltaHidden[numHiddenNodes];
        for (int j=0; j<numHiddenNodes; j++) {
            double errorHidden = 0.0f;
            for(int k=0; k<numOutputs; k++) {
                errorHidden += deltaOutput[k] * nn->outputWeights[j][k];
            }
            deltaHidden[j] = errorHidden * dSigmoid(nn->hiddenLayer[j]);
        }

        // Apply change in output weights
        for (int j=0; j<numOutputs; j++) {
            nn->outputLayerBias[j] += deltaOutput[j] * nn->lr;
            for (int k=0; k<numHiddenNodes; k++) {
                nn->outputWeights[k][j] += nn->hiddenLayer[k] * deltaOutput[j] * nn->lr;
            }
        }

        // Apply change in hidden weights
        for (int j=0; j<numHiddenNodes; j++) {
            nn->hiddenLayerBias[j] += deltaHidden[j] * nn->lr;
            for(int k=0; k<numInputs; k++) {
                nn->hiddenWeights[k][j] += training_inputs[i][k] * deltaHidden[j] * nn->lr;
            }
        }
    }


  }

  
  save_weights_and_biases(outputfile, nn);
  if(print==1)print_weights_and_biases(nn);
}

