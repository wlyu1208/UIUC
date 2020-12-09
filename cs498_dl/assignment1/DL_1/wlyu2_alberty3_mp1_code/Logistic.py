"""Logistic regression model."""

import numpy as np
import math


class Logistic:
    def __init__(self, lr: float, epochs: int):
        """Initialize a new classifier.

        Parameters:
            lr: the learning rate
            epochs: the number of epochs to train for
        """
        self.w = None  # TODO: change this
        self.lr = lr
        self.epochs = epochs
        self.threshold = 0.5

    def sigmoid(self, z: np.ndarray) -> np.ndarray:
        """Sigmoid function.

        Parameters:
            z: the input

        Returns:
            the sigmoid of the input
        """
        # TODO: implement me
        return 1 / (1+np.exp(-z))

    def train(self, X_train: np.ndarray, y_train: np.ndarray):
        """Train the classifier.

        Use the logistic regression update rule as introduced in lecture.

        Parameters:
            X_train: a numpy array of shape (N, D) containing training data;
                N examples with D dimensions
            y_train: a numpy array of shape (N,) containing training labels
        """
        # TODO: implement me
        print("train start")
        sum_sig = 0
        self.w = np.zeros(X_train.shape[1])
        num_train = y_train.shape[0]
        for passes in range(self.epochs):
            for cycle in range(num_train):
                if y_train[cycle] == 0:
                    y_train[cycle] = -1
                first = np.dot(self.w, X_train[cycle]) 
                #print(result_sig)
                self.w = self.w + self.lr * self.sigmoid(-y_train[cycle] * first) * np.dot(y_train[cycle], X_train[cycle])
        
        for each in range(y_train.shape[0]):
            if y_train[each] == -1:
                y_train[each] = 0
        print("train done")
        return

    def predict(self, X_test: np.ndarray) -> np.ndarray:
        """Use the trained weights to predict labels for test data points.

        Parameters:
            X_test: a numpy array of shape (N, D) containing testing data;
                N examples with D dimensions

        Returns:
            predicted labels for the data in X_test; a 1-dimensional array of
                length N, where each element is an integer giving the predicted
                class.
        """
        # TODO: implement me
        labels = []
        for example in range(X_test.shape[0]):
            check = np.dot(self.w, X_test[example])
            #print(check)
            if check > 0:
                labels.append(1)
            else:
                labels.append(0)
        return labels
