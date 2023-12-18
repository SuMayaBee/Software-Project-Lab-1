# Handwritten Digit Recognition with Neural Networks

This project aims to build a fundamental understanding of neural networks by implementing a simple network from scratch in C++. The network is trained to recognize handwritten digits using the MNIST dataset and is then tested with custom hand-drawn digits.

## Data Preprocessing

The MNIST dataset of handwritten digits is used to train and test the neural network. The dataset is loaded and preprocessed for use in the network, which involves reading the data from its binary format, normalizing it to be in a range suitable for the network, and reshaping it depending on the network design.

## Neural Network Design

The neural network consists of an input layer, one or more hidden layers, and an output layer. Each layer is fully connected to the next. The input layer has as many neurons as there are input features (pixels in an image), and the output layer has 10 neurons, each representing one of the 10 possible digits (0-9).

## Training

The network is trained using the training portion of the MNIST dataset. The training process involves feedforward propagation of inputs through the network to produce outputs, calculation of a loss function comparing the outputs to the actual values, and backpropagation of errors to adjust the weights and biases in the network.

## Testing and Evaluation

The trained network is evaluated using the test portion of the MNIST dataset. This provides an objective measure of how well the network performs on data it has not seen before.

## Custom Input Drawing

A simple interface is created for drawing custom digit inputs to feed into the network. This allows for interactive testing of the network's performance.

## Code Snippet

Here is a code snippet from the main.cpp file that demonstrates how the network makes predictions:

```cpp
for (int j = 0; j < COLS; j++)
{
    printf("%d ", image[i][j]);
}
printf("\n");

Matrix *input_data = image_to_matrix(image);

Img *img = (Img*)malloc(sizeof(Img));
img->img_data = input_data;
img->label = -1; 

Matrix* prediction = network_predict_img(net, img);

int predicted_label = 0;
double max_value = prediction->entries[0][0];

for (int i = 1; i < prediction->rows; i++)
{
    if (prediction->entries[i][0] > max_value)
    {
        max_value = prediction->entries[i][0];
        predicted_label = i;
    }
}

printf("The predicted label is: %d\n", predicted_label);
```

## Project Structure

The project is implemented in C++ and uses a custom matrix library for operations. The code is organized into separate modules for different functionalities:

- **Image Processing:** The `util/img.c` file contains functions for loading and preprocessing images.

- **Matrix Operations:** The `matrix/matrix.c` and `matrix/ops.c` files contain functions for creating and manipulating matrices, which are used throughout the project.

- **Neural Network:** The `neural/nn.c` and `neural/activations.c` files contain the implementation of the neural network, including the feedforward and backpropagation algorithms, as well as various activation functions.

  

https://github.com/Sumaiya-369/Software-Project-Lab-1/assets/126413802/e1c8f992-73cb-48bc-8859-4d7a55cb5e6c


