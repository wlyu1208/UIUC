"""Perceptron model."""

import numpy as np

class Perceptron:
    def __init__(self, n_class: int, lr: float, epochs: int):
        """Initialize a new classifier.

        Parameters:
            n_class: the number of classes
            lr: the learning rate
            epochs: the number of epochs to train for
        """
        self.w = None  # TODO: change this
        self.lr = lr
        self.epochs = epochs
        self.n_class = n_class

    def train(self, X_train: np.ndarray, y_train: np.ndarray):
        """Train the classifier.

        Use the perceptron update rule as introduced in Lecture 3.

        Parameters:
            X_train: a number array of shape (N, D) containing training data;
                N examples with D dimensions
            y_train: a numpy array of shape (N,) containing training labels
        """
        # TODO: implement me
        #print(y_train)
        self.w = np.zeros((self.n_class, X_train.shape[1]))
        print("train start")
        N = X_train.shape[0]
        for passes in range(self.epochs):
            #if passes % 10 == 0:
            #    self.lr = self.lr/2
            #    prediction = self.predict(X_train)
            #    accuracy = np.sum(y_train==prediction)/len(y_train)*100
            #    print('Number of epochs: ', passes, ' accuracy: ', accuracy, 'lr: ', self.lr)
            for cycle in range(N):
                prediction = np.argmax(np.dot(self.w, X_train[cycle].T))
                if prediction != y_train[cycle]:
                    for c in range(self.n_class):
                        if c == y_train[cycle]:
                            self.w[c] = self.w[c] + self.lr * X_train[cycle]
                        else:
                            self.w[c] = self.w[c] - self.lr * X_train[cycle]
        print("train done")
        #print(self.w)                          
        
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
        #print(X_test)
        labels = []
        for example in range(X_test.shape[0]):
            labels.append(np.argmax(np.dot(self.w, X_test[example].T)))
        
        return labels
