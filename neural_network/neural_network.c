#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <err.h>
#include <string.h>

//nb of inputs neurons
#define numInputs 2 

//nb of Hidden neurons
#define numHiddenNodes 2

//nb of Outputs neurons
#define numOutputs 1

//nb of Training set
#define numTrainingSets 4



double init_weights() {
  return ((double)rand()) / ((double)RAND_MAX);
}

typedef struct
{
  double lr; //learning rate
  
  double hiddenLayer[numHiddenNodes];
  double outputLayer[numOutputs];

  double hiddenLayerBias[numHiddenNodes];
  double outputLayerBias[numOutputs];

  double hiddenWeights[numInputs][numHiddenNodes];
  double outputWeights[numHiddenNodes][numOutputs];
}neural_network;

void init_neural_network(neural_network * nn){
  nn->lr = 0.1;

  for (int i = 0; i < numHiddenNodes; i++) {
        for (int j = 0; j < numOutputs; j++) {
            nn->outputWeights[i][j] = init_weights();
        }
    }
    for (int i = 0; i < numHiddenNodes; i++) {
        nn->hiddenLayerBias[i] = init_weights();
    }
    for (int i = 0; i < numOutputs; i++) {
        nn->outputLayerBias[i] = init_weights();
    }
    for (int i = 0; i < numInputs; i++) {
        for (int j = 0; j < numHiddenNodes; j++) {
            nn->hiddenWeights[i][j] = init_weights();
        }
    }

}


//macro for printing neurons result
#define PR(a1,a2,a3,a4) printf("In: %g %g Out: %g Predict: %g\n",a1,a2,a3,a4);

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

// Fonction pour sauvegarder les poids et biais
void save_weights_and_biases(const char* filename, neural_network *nn) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Unable to open file for saving");
        return;
    }
    
    // Sauvegarde des poids cachés
    fwrite(nn->hiddenWeights, sizeof(double), numInputs * numHiddenNodes, file);
    // Sauvegarde des poids de sortie
    fwrite(nn->outputWeights, sizeof(double), numHiddenNodes * numOutputs, file);
    // Sauvegarde des biais cachés
    fwrite(nn->hiddenLayerBias, sizeof(double), numHiddenNodes, file);
    // Sauvegarde des biais de sortie
    fwrite(nn->outputLayerBias, sizeof(double), numOutputs, file);
    
    fclose(file);
}

// Fonction pour charger les poids et biais
void load_weights_and_biases(const char* filename, neural_network *nn) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Unable to open file for loading");
        return;
    }
    
    // Chargement des poids cachés
    if (fread(nn->hiddenWeights, sizeof(double), numInputs * numHiddenNodes, file) != numInputs * numHiddenNodes) {
        perror("Error reading hiddenWeights");
        fclose(file);
        return;
    }
    
    // Chargement des poids de sortie
    if (fread(nn->outputWeights, sizeof(double), numHiddenNodes * numOutputs, file) != numHiddenNodes * numOutputs) {
        perror("Error reading outputWeights");
        fclose(file);
        return;
    }
    
    // Chargement des biais cachés
    if (fread(nn->hiddenLayerBias, sizeof(double), numHiddenNodes, file) != numHiddenNodes) {
        perror("Error reading hiddenLayerBias");
        fclose(file);
        return;
    }
    
    // Chargement des biais de sortie
    if (fread(nn->outputLayerBias, sizeof(double), numOutputs, file) != numOutputs) {
        perror("Error reading outputLayerBias");
        fclose(file);
        return;
    }
    
    fclose(file);
}

void print_weights_and_biases(neural_network *nn ) {
    printf("Hidden Weights:\n");
    for (int i = 0; i < numInputs; i++) {
        for (int j = 0; j < numHiddenNodes; j++) {
            printf("%f ", nn->hiddenWeights[i][j]);
        }
        printf("\n");
    }
    
    printf("\nOutput Weights:\n");
    for (int i = 0; i < numHiddenNodes; i++) {
        for (int j = 0; j < numOutputs; j++) {
            printf("%f ", nn->outputWeights[i][j]);
        }
        printf("\n");
    }
    
    printf("\nHidden Layer Biases:\n");
    for (int i = 0; i < numHiddenNodes; i++) {
        printf("%f ", nn->hiddenLayerBias[i]);
    }
    printf("\n");
    
    printf("\nOutput Layer Biases:\n");
    for (int i = 0; i < numOutputs; i++) {
        printf("%f ", nn->outputLayerBias[i]);
    }
    printf("\n");
}



void train(neural_network * nn){




  double training_inputs[numTrainingSets][numInputs] = {{0.0f,0.0f},
                                                        {1.0f,0.0f},
                                                        {0.0f,1.0f},
                                                        {1.0f,1.0f}};


  double training_outputs[numTrainingSets][numOutputs] = {{1.0f},
                                                          {0.0f},
                                                          {0.0f},
                                                          {1.0f}};

  
  load_weights_and_biases("weights_and_biases.bin", nn);

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
        double activation = nn->hiddenLayerBias[j];

        for(int k = 0; k < numInputs; k++){
          activation += training_inputs[i][k] * nn->hiddenWeights[k][j];
        }

        nn->hiddenLayer[j] = sigmoid(activation);
      }

      //Compute output layer activation
      for(int j = 0; j< numOutputs; j++){
        double activation = nn->outputLayerBias[j];

        for(int k = 0; k < numHiddenNodes; k++){
          activation += nn->hiddenLayer[k] * nn->outputWeights[k][j];
        }

        nn->outputLayer[j] = round(sigmoid(activation));
      }
      printf(" %d sur %ld    :" , epoch, numberOfEpochs);
      
      PR(training_inputs[i][0], training_inputs[i][1],nn->outputLayer[0],
          training_outputs[i][0]);

      // Backprop
      
      //Compute change in output weight
      
      double deltaOutput[numOutputs];
      
      for( int j = 0; j < numOutputs; j++){
        double error = (training_outputs[i][j] - nn->outputLayer[j]);
        deltaOutput[j] = error * dSigmoid(nn->outputLayer[j]);
      }

      //Compute change in hidden weights
      
      double deltaHidden[numHiddenNodes];
      for(int j = 0; j < numHiddenNodes; j++){
        double error = 0.0f;
        for(int k = 0; k<numOutputs; k++){
          error += deltaOutput[k] * nn->outputWeights[j][k];
        }
        deltaHidden[j] = error * dSigmoid(nn->hiddenLayer[j]);
      }


      // Apply change in output weights

      for(int j = 0; j< numOutputs; j++){
        nn->outputLayerBias[j] += deltaOutput[j] * nn->lr;
        for(int k = 0; k<numHiddenNodes; k++){
          nn->outputWeights[k][j] += nn->hiddenLayer[k] * deltaOutput[j] * nn->lr;
        }
      }

      
      // Apply change in hidden weights
      
      for(int j = 0; j< numHiddenNodes; j++){
        nn->hiddenLayerBias[j] += deltaHidden[j] * nn->lr;
        for(int k = 0; k<numInputs; k++){
          nn->hiddenWeights[k][j] += training_inputs[i][k] * deltaHidden[j] * nn->lr;
        }
      }

    }
  }
  
  save_weights_and_biases("weights_and_biases.bin", nn);
  print_weights_and_biases(nn);
}

double predict(double a, double b){
  if(!((a==1 || a==0) && (b==1 || b==0)))
    errx(1,"valeur d'entré de predict pas bonne");
  printf("fais la fonction connard\n entrée 1 : %g entrée 2 : %g\n",a , b );
  return a;
}



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
