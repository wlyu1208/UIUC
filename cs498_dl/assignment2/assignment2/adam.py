"""Adam Neural network model."""

from typing import Sequence

import numpy as np
import math

class AdamNeuralNetwork:
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
        self.m = {}
        self.v = {}

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
    
    #https://www.programmersought.com/article/2128926390/
    #relu activation
    def relu_grad(self, X:np.ndarray) -> np.ndarray:
        return (X > 0).astype(float)
    
    #https://stackoverflow.com/questions/34968722/how-to-implement-the-softmax-function-in-python
    def softmax_grad(self, X:np.ndarray) -> np.ndarray:
        return np.exp(X) / np.sum(np.exp(X), axis=1, keepdims=True)
    
    #store linear and nonlinear
    def forward(self, X: np.ndarray) -> np.ndarray:
        self.outputs = {}
        self.outputs["relu"+str(0)] = X
        for i in range(1, self.num_layers):
                w, b = self.params.get("W"+str(i)), self.params.get("b"+str(i))
                self.outputs["W"+str(i)] = self.linear(w, self.outputs["relu"+str(i-1)], b)
                self.outputs["relu"+str(i)] = self.relu(self.outputs["W"+str(i)])
        
        w, b = self.params.get("W"+str(self.num_layers)), self.params.get("b"+str(self.num_layers))
        self.outputs["W"+str(self.num_layers)] = self.linear(w, self.outputs["relu"+str(self.num_layers-1)], b)
        self.outputs["relu"+str(self.num_layers)] = self.linear(w, self.outputs["relu"+str(self.num_layers-1)], b)
        result = self.softmax(self.outputs["W"+str(self.num_layers)])
        return result
    
    #https://wiseodd.github.io/techblog/2016/06/22/nn-optimization/
    #cleanest explanation and need to change backward for gradient to have only gradient
    def backward(
        self, X: np.ndarray, y: np.ndarray, lr: float, time: int, beta_one: float, beta_two: float, epil: float, reg: float
    ) -> float:
        self.gradients = {}
        loss = 0.0      
        
        #calculate loss first
        N = X.shape[0]
        output = self.outputs["W"+str(self.num_layers)]
        forward_output = self.softmax_grad(output)
        rearrange = forward_output[np.arange(start=0, stop=N), y]
        loss = np.sum(-np.log(rearrange)) / N
        
        #find loss gradient
        previous = forward_output
        previous[np.arange(start = 0, stop = N), y] -= 1
        previous /= N
        
        loss_gradient = previous
        
        # w_g = dot(output(non-linear-1).T, loss_gradient)
        # w_b = sum of 0th index of upstream(dot(loss_gradient, parameter(linear+1).T)*nonlinear)
        self.gradients["W"+str(self.num_layers)] = np.dot(self.outputs["relu"+str(self.num_layers-1)].transpose(), loss_gradient)
        self.gradients["b"+str(self.num_layers)] = np.sum(loss_gradient, axis = 0)
        
        for index in range(self.num_layers-1, 0, -1):
            derivative =  self.relu_grad(self.outputs["relu"+str(index)])
            grad_wrd = np.dot(loss_gradient, self.params["W"+str(index+1)].transpose())
            upstream = grad_wrd * derivative
            
            self.gradients["W"+str(index)] = np.dot(self.outputs["relu"+str(index-1)].transpose(), upstream) + reg *self.params["W"+str(index)]
            self.gradients["b"+str(index)] = np.sum(upstream, axis=0)
            loss_gradient = upstream
        
        if len(self.m) == 0 and len(self.v) == 0:
            self.m = {key: np.zeros_like(val) for key, val in self.params.items()}
            self.v = {key: np.zeros_like(val) for key, val in self.params.items()}
        
        for key in self.params.keys():
            self.m[key] = beta_one * self.m[key] + (1 - beta_one) * self.gradients[key]
            self.v[key] = beta_two * self.v[key] + (1 - beta_two) * (self.gradients[key] ** 2)
            
            m_hat = self.m[key] / (1.0 - (beta_one**time))
            v_hat = self.v[key] / (1.0 - (beta_two**time))
            
            self.params[key] -= lr * m_hat / (np.sqrt(v_hat)+epil)

            
        return loss
