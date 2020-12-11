"""Neural network model."""

from typing import Sequence

import numpy as np
import math

class NeuralNetwork:
    """A multi-layer fully-connected neural network. The net has an input
    dimension of N, a hidden layer dimension of H, and performs classification
    over C classes. We train the network with a cross-entropy loss function and
    L2 regularization on the weight matrices.

    The network uses a nonlinearity after each fully connected layer except for
    the last. The outputs of the last fully-connected layer are passed through
    a softmax, and become the scores for each class."""

    def __init__(
        self,
        input_size: int,
        hidden_sizes: Sequence[int],
        output_size: int,
        num_layers: int,
    ):
        """Initialize the model. Weights are initialized to small random values
        and biases are initialized to zero. Weights and biases are stored in
        the variable self.params, which is a dictionary with the following
        keys:

        W1: 1st layer weights; has shape (D, H_1)
        b1: 1st layer biases; has shape (H_1,)
        ...
        Wk: kth layer weights; has shape (H_{k-1}, C)
        bk: kth layer biases; has shape (C,)

        Parameters:
            input_size: The dimension D of the input data
            hidden_size: List [H1,..., Hk] with the number of neurons Hi in the
                hidden layer i
            output_size: The number of classes C
            num_layers: Number of fully connected layers in the neural network
        """
        self.input_size = input_size
        self.hidden_sizes = hidden_sizes
        self.output_size = output_size
        self.num_layers = num_layers

        assert len(hidden_sizes) == (num_layers - 1)
        sizes = [input_size] + hidden_sizes + [output_size]

        self.params = {}
        for i in range(1, num_layers + 1):
            self.params["W" + str(i)] = np.random.randn(
                sizes[i - 1], sizes[i]
            ) / np.sqrt(sizes[i - 1])
            self.params["b" + str(i)] = np.zeros(sizes[i])

    def linear(self, W: np.ndarray, X: np.ndarray, b: np.ndarray) -> np.ndarray:
        """Fully connected (linear) layer.

        Parameters:
            W: the weight matrix
            X: the input data
            b: the bias

        Returns:
            the output
        """
        # TODO: implement me
        return np.dot(X, W) + b

    def relu(self, X: np.ndarray) -> np.ndarray:
        """Rectified Linear Unit (ReLU).

        Parameters:
            X: the input data

        Returns:
            the output
        """
        # TODO: implement me
        return np.maximum(X, 0)

    def softmax(self, X: np.ndarray) -> np.ndarray:
        """The softmax function.

        Parameters:
            X: the input data

        Returns:
            the output
        """
        # TODO: implement me
        exp_x = np.exp(X)
        return exp_x / np.sum(exp_x)
    
    def relu_grad(self, X:np.ndarray) -> np.ndarray:
        return (X > 0).astype(float)
    
    #https://stackoverflow.com/questions/34968722/how-to-implement-the-softmax-function-in-python
    def softmax_grad(self, X:np.ndarray) -> np.ndarray:
        return np.exp(X) / np.sum(np.exp(X), axis=1, keepdims=True)
    
    def forward(self, X: np.ndarray) -> np.ndarray:
        """Compute the scores for each class for all of the data samples.

        Hint: this function is also used for prediction.

        Parameters:
            X: Input data of shape (N, D). Each X[i] is a training or
                testing sample

        Returns:
            Matrix of shape (N, C) where scores[i, c] is the score for class
                c on input X[i] outputted from the last layer of your network
        """
        self.outputs = {}
        
        
        #W+ have int, relu have matrix
        self.outputs["W"+str(0)] = 1
        self.outputs["relu"+str(0)] = X
        
        #just initialize result with softmax
        result = self.softmax(X)
        if self.num_layers == 2:
            for i in range(1, self.num_layers):
                w, b = self.params.get("W"+str(i)), self.params.get("b"+str(i))
                self.outputs["W"+str(i)] = self.linear(w, self.outputs["relu"+str(i-1)], b)
                self.outputs["relu"+str(i)] = self.relu(self.outputs["W"+str(i)])
                #final layer: dont pass to non-linear activation(relu)
            
                w, b = self.params.get("W"+str(self.num_layers)), self.params.get("b"+str(self.num_layers))
                self.outputs["W"+str(self.num_layers)] = self.linear(w, self.outputs["relu"+str(self.num_layers-1)], b)
                self.outputs["relu"+str(self.num_layers)] = self.linear(w, self.outputs["relu"+str(self.num_layers-1)], b)
        else:
            for i in range(1, self.num_layers+1):
                w, b = self.params.get("W"+str(i)), self.params.get("b"+str(i))
                self.outputs["W"+str(i)] = self.linear(w, self.outputs["relu"+str(i-1)], b)
                self.outputs["relu"+str(i)] = self.relu(self.outputs["W"+str(i)])
        
        result = self.softmax(self.outputs["W"+str(self.num_layers)])
        return result

    def backward(
        self, X: np.ndarray, y: np.ndarray, lr: float, reg: float
    ) -> float:
        """Perform back-propagation and update the parameters using the
        gradients.

        Parameters:
            X: Input data of shape (N, D). Each X[i] is a training sample
            y: Vector of training labels. y[i] is the label for X[i], and each
                y[i] is an integer in the range 0 <= y[i] < C
            lr: Learning rate
            reg: Regularization strength

        Returns:
            Total loss for this batch of training samples
        """
        self.gradients = {}
        loss = 0.0
        # TODO: implement me. You'll want to store the gradient of each layer
        # in self.gradients if you want to be able to debug your gradients
        # later. You can use the same keys as self.params. You can add
        # functions like self.linear_grad, self.relu_grad, and
        # self.softmax_grad if it helps organize your code.
        N = X.shape[0]
        #print(score)
        output = self.outputs["W"+str(self.num_layers)]
        forward_output = self.softmax_grad(output)
        #print(output, forward_output)
        rearrange = forward_output[np.arange(start=0, stop=N), y]
        #print(rearrange)
        loss = np.sum(-np.log(rearrange)) / N
        
        
        #downstream gradient update
        previous = forward_output
        #print(previous)
        previous[np.arange(start = 0, stop = N), y] -= 1
        #print(previous)
        previous /= N
        #print(previous)
        
        self.gradients["grad"+str(self.num_layers)] = previous
        current_gradient = previous

        self.gradients["grad"+str(self.num_layers-1)] = np.dot(current_gradient, self.params["W"+str(self.num_layers)].T)
        
        gradient_deriv = self.outputs["W"+str(self.num_layers-1)]
        gradient_list = self.gradients["grad"+str(self.num_layers-1)]
        gradient_list[gradient_deriv <= 0] = 0
        
        self.params["W"+str(self.num_layers)] = (1-(lr*reg/N)) * self.params["W"+str(self.num_layers)] - lr * np.dot(self.outputs["relu"+str(self.num_layers-1)].T, current_gradient)
        
        self.params["b"+str(self.num_layers)] -= lr * np.sum(current_gradient.T, axis = 1)
        
        for index in range(self.num_layers-1, 0, -1):
            current_gradient = self.gradients["grad"+str(index)]

            self.gradients["grad"+str(index-1)] = np.dot(current_gradient, self.params["W"+str(index)].T)
            
            
            gradient_deriv = self.outputs["W"+str(index-1)]
            gradient_list = self.gradients["grad"+str(index-1)]
            gradient_list[gradient_deriv <= 0] = 0
            
            #https://jamesmccaffrey.wordpress.com/2017/02/19/l2-regularization-and-back-propagation/
            #dc / dw = derivative of the error function (also called the cost function)
            self.params["W"+str(index)] = (1-(lr*reg/N)) * self.params["W"+str(index)] - lr * np.dot(self.outputs["relu"+str(index-1)].T, current_gradient)
            
            self.params["b"+str(index)] -= lr * np.sum(current_gradient.T, axis = 1)
        return loss
