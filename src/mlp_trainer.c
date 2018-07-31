/*
Author: Manohar Mukku
Date: 20.07.2018
Desc: Train the parameter weights on the train data set
GitHub: https://github.com/manoharmukku/multilayer-perceptron-in-c
*/

#include "mlp_trainer.h"

void initialize_weights(parameters* param, int n_layers, int* layer_sizes) {
    srand(time(0));

    // epsilon = sqrt(6/(layer_size[i] + layer_size[i+1])) used for random initialization
    double* epsilon = (double*)calloc(n_layers-1, sizeof(double));
    int i;
    for (i = 0; i < n_layers-1; i++)
        epsilon[i] = sqrt(6.0 / (layer_sizes[i] + layer_sizes[i+1]));

    // Random initialization between [-epsilon[i], epsilon[i]] for weight[i]
    int j, k;
    for (i = 0; i < n_layers-1; i++)
        for (j = 0; j < layer_sizes[i]+1; j++)
            for (k = 0; k < layer_sizes[i+1]; k++)
                param->weight[i][j][k] = -epsilon[i] + ((double)rand() / ((double)RAND_MAX / (2.0 * epsilon[i])));

    // Free the memory allocated in Heap for epsilon array
    free(epsilon);
}

void mlp_trainer(parameters* param, int* layer_sizes) {
    // Total number of layers
    int n_layers = param->n_hidden + 2;

    // Create memory for arrays of inputs to the layers
    double** layer_inputs = (double**)calloc(n_layers, sizeof(double*));

    int i;
    for (i = 0; i < n_layers; i++)
        layer_inputs[i] = (double*)calloc(layer_sizes[i], sizeof(double));

    // Create memory for arrays of outputs from the layers
    double** layer_outputs = (double**)calloc(n_layers, sizeof(double*));

    for (i = 0; i < n_layers; i++)
        layer_outputs[i] = (double*)calloc(layer_sizes[i]+1, sizeof(double));

    // Initialize the weights
    initialize_weights(param, n_layers, layer_sizes);

    // Train the MLP
    int training_example;
    for (i = 0; i < param->n_iterations_max; i++) {
        for (training_example = 0; training_example < param->train_sample_size; training_example++) {
            // Perform forward propagation on the jth training example
            forward_propagation(param, training_example, n_layers, layer_sizes, layer_inputs, layer_outputs);

            // Calculate the error

            // Perform back propagation and update weights
            back_propagation(param, training_example, n_layers, layer_sizes, layer_inputs, layer_outputs);
        }   
    }

    // Free the memory allocated in Heap
    for (i = 0; i < n_layers; i++)
        free(layer_outputs[i]);

    free(layer_outputs);

    for (i = 0; i < n_layers; i++)
        free(layer_inputs[i]);

    free(layer_inputs);
}