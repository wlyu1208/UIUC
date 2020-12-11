"""Support Vector Machine (SVM) model."""

import numpy as np


class SVM:
    def __init__(self, n_class: int, lr: float, epochs: int, reg_const: float):
        """Initialize a new classifier.

        Parameters:
            n_class: the number of classes
            lr: the learning rate
            epochs: the number of epochs to train for
            reg_const: the regularization constant
        """
        self.w = None  # TODO: change this
        self.alpha = lr
        self.epochs = epochs
        self.reg_const = reg_const
        self.n_class = n_class

    def calc_gradient(self, X_train: np.ndarray, y_train: np.ndarray) -> np.ndarray:
        """Calculate gradient of the svm hinge loss.

        Inputs have dimension D, there are C classes, and we operate on
        mini-batches of N examples.

        Parameters:
            X_train: a numpy array of shape (N, D) containing a mini-batch
                of data
            y_train: a numpy array of shape (N,) containing training labels;
                y[i] = c means that X[i] has label c, where 0 <= c < C

        Returns:
            the gradient with respect to weights w; an array of the same shape
                as w
        """
        # TODO: implement me
        
        return

    def train(self, X_train: np.ndarray, y_train: np.ndarray):
        """Train the classifier.

        Hint: operate on mini-batches of data for SGD.

        Parameters:
            X_train: a numpy array of shape (N, D) containing training data;
                N examples with D dimensions
            y_train: a numpy array of shape (N,) containing training labels
        """
        # TODO: implement me
        print("train start")
        self.w = np.random.randn(self.n_class, X_train.shape[1])
        num_train = X_train.shape[0]
        for passes in range(self.epochs):
            #if passes % 10 == 0:
            #    prediction = self.predict(X_train)
            #    accuracy = np.sum(y_train==prediction)/len(y_train)*100
            #    print('Number of epochs: ', passes, ' accuracy: ', accuracy, 'lr: ', self.alpha)
            for cycle in range(num_train):
                self.w = (1- self.alpha*self.reg_const/self.n_class) * self.w
                for c in range(self.n_class):
                    #gradient(x, y)
                    if c == y_train[cycle]:
                        self.w[c] = self.w[c] + self.alpha * X_train[cycle]
                    else:
                        w_y_tran = self.w[y_train[cycle]].T
                        compare = np.dot(w_y_tran, X_train[cycle]) - np.dot(self.w[c].T, X_train[cycle])
                        if compare < 1:
                            self.w[c] = self.w[c] - self.alpha * X_train[cycle]  

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
            labels.append(np.argmax(np.dot(self.w, X_test[example])))
        
        return labels
