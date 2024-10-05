#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//nb of inputs neurons
#define numInputs 2 

//nb of Hidden neurons
#define numHiddenNodes 2

//nb of Outputs neurons
#define numOutputs 1

//nb of Training set
#define numTrainingSets 4

//macro for printing neurons result
#define PR(a1,a2,a3,a4) printf("In: %g %g Out: %g Predict: %g\n",a1,a2,a3,a4);
double sigmoid(double x){
  return 1/ (1 + exp(-x));
}

double dSigmoid(double x){
  return x * (1-x);
}

double init_weights() {
  return ((double)rand()) / ((double)RAND_MAX);
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

// Fonction pour sauvegarder les poids et biais
void save_weights_and_biases(const char* filename, 
                              double hiddenWeights[numInputs][numHiddenNodes],
                              double outputWeights[numHiddenNodes][numOutputs],
                              double hiddenLayerBias[numHiddenNodes],
                              double outputLayerBias[numOutputs]) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Unable to open file for saving");
        return;
    }
    
    // Sauvegarde des poids cachés
    fwrite(hiddenWeights, sizeof(double), numInputs * numHiddenNodes, file);
    // Sauvegarde des poids de sortie
    fwrite(outputWeights, sizeof(double), numHiddenNodes * numOutputs, file);
    // Sauvegarde des biais cachés
    fwrite(hiddenLayerBias, sizeof(double), numHiddenNodes, file);
    // Sauvegarde des biais de sortie
    fwrite(outputLayerBias, sizeof(double), numOutputs, file);
    
    fclose(file);
}

// Fonction pour charger les poids et biais
void load_weights_and_biases(const char* filename, 
                             double hiddenWeights[numInputs][numHiddenNodes],
                             double outputWeights[numHiddenNodes][numOutputs],
                             double hiddenLayerBias[numHiddenNodes],
                             double outputLayerBias[numOutputs]) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Unable to open file for loading");
        return;
    }
    
    // Chargement des poids cachés
    if (fread(hiddenWeights, sizeof(double), numInputs * numHiddenNodes, file) != numInputs * numHiddenNodes) {
        perror("Error reading hiddenWeights");
        fclose(file);
        return;
    }
    
    // Chargement des poids de sortie
    if (fread(outputWeights, sizeof(double), numHiddenNodes * numOutputs, file) != numHiddenNodes * numOutputs) {
        perror("Error reading outputWeights");
        fclose(file);
        return;
    }
    
    // Chargement des biais cachés
    if (fread(hiddenLayerBias, sizeof(double), numHiddenNodes, file) != numHiddenNodes) {
        perror("Error reading hiddenLayerBias");
        fclose(file);
        return;
    }
    
    // Chargement des biais de sortie
    if (fread(outputLayerBias, sizeof(double), numOutputs, file) != numOutputs) {
        perror("Error reading outputLayerBias");
        fclose(file);
        return;
    }
    
    fclose(file);
}


int main(void){
  const double lr = 0.1f; //learning rate
  
  double hiddenLayer[numHiddenNodes];
  double outputLayer[numOutputs];

  double hiddenLayerBias[numHiddenNodes];
  double outputLayerBias[numOutputs];

  double hiddenWeights[numInputs][numHiddenNodes];
  double outputWeights[numHiddenNodes][numOutputs];


  double training_inputs[numTrainingSets][numInputs] = {{0.0f,0.0f},
                                                        {1.0f,0.0f},
                                                        {0.0f,1.0f},
                                                        {1.0f,1.0f}};


  double training_outputs[numTrainingSets][numOutputs] = {{1.0f},
                                                          {0.0f},
                                                          {0.0f},
                                                          {1.0f}};

  for(int i = 0; i< numHiddenNodes; i++){
    for(int j = 0 ; j<numOutputs; j++){
      outputWeights[i][j] = init_weights();
    }
  }
  
  for(int j = 0; j<numOutputs; j++){
    outputLayerBias[j] = init_weights();
  }

  load_weights_and_biases("weights_and_biases.bin", hiddenWeights, outputWeights, hiddenLayerBias, outputLayerBias);

  int trainingSetOrder[] = {0,1,2,3};

  long int numberOfEpochs = 10000;

  // Train the neural Network
  
  for(int epoch = 0; epoch < numberOfEpochs; epoch++){

    shuffle(trainingSetOrder, numTrainingSets);

    for(int x = 0; x<numTrainingSets; x++){
      int i = trainingSetOrder[x];

      //Forward Pass

      //Compute hidden layer activation
      for(int j = 0; j< numHiddenNodes; j++){
        double activation = hiddenLayerBias[j];

        for(int k = 0; k < numInputs; k++){
          activation += training_inputs[i][k] * hiddenWeights[k][j];
        }

        hiddenLayer[j] = sigmoid(activation);
      }

      //Compute output layer activation
      for(int j = 0; j< numOutputs; j++){
        double activation = outputLayerBias[j];

        for(int k = 0; k < numHiddenNodes; k++){
          activation += hiddenLayer[k] * outputWeights[k][j];
        }

        outputLayer[j] = round(sigmoid(activation));
      }
      printf(" %d sur %ld    :" , epoch, numberOfEpochs);
      PR(training_inputs[i][0], training_inputs[i][1],outputLayer[0],
          training_outputs[i][0]);

      // Backprop
      
      //Compute change in output weight
      
      double deltaOutput[numOutputs];
      
      for( int j = 0; j < numOutputs; j++){
        double error = (training_outputs[i][j] - outputLayer[j]);
        deltaOutput[j] = error * dSigmoid(outputLayer[j]);
      }

      //Compute change in hidden weights
      
      double deltaHidden[numHiddenNodes];
      for(int j = 0; j < numHiddenNodes; j++){
        double error = 0.0f;
        for(int k = 0; k<numOutputs; k++){
          error += deltaOutput[k] * outputWeights[j][k];
        }
        deltaHidden[j] = error * dSigmoid(hiddenLayer[j]);
      }


      // Apply change in output weights

      for(int j = 0; j< numOutputs; j++){
        outputLayerBias[j] += deltaOutput[j] * lr;
        for(int k = 0; k<numHiddenNodes; k++){
          outputWeights[k][j] += hiddenLayer[k] * deltaOutput[j] * lr;
        }
      }

      
      // Apply change in hidden weights
      
      for(int j = 0; j< numHiddenNodes; j++){
        hiddenLayerBias[j] += deltaHidden[j] * lr;
        for(int k = 0; k<numInputs; k++){
          hiddenWeights[k][j] += training_inputs[i][k] * deltaHidden[j] * lr;
        }
      }

    }
  }
  
  //save_weights_and_biases("weights_and_biases.bin", hiddenWeights, outputWeights, hiddenLayerBias, outputLayerBias);

  return 0;
}
