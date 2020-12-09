import torch
import torch.nn as nn
from torch.autograd import Variable
import torch.nn.functional as F
from torch import optim
import numpy as np

NUM_CLASSES = 21

class SimpleClassifier(nn.Module):
    def __init__(self):
        super(SimpleClassifier, self).__init__()
        self.conv1 = nn.Conv2d(3, 64, 5)
        self.conv2 = nn.Conv2d(64, 32, 3)
        self.conv3 = nn.Conv2d(32, 16, 3)
        self.pool = nn.MaxPool2d(2, 2)
        self.fc1 = nn.Linear(16 * 26 * 26, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, NUM_CLASSES)

    def forward(self, x):
        x = self.pool(F.relu(self.conv1(x)))
        x = self.pool(F.relu(self.conv2(x)))
        x = self.pool(F.relu(self.conv3(x)))
        x = x.view(x.size()[0], 16 * 26 * 26)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

class Classifier(nn.Module):
    # TODO: implement me
    def __init__(self):
        super(Classifier, self).__init__()
        self.conv1 = nn.Conv2d(3, 64, kernel_size = 11, stride = 4, padding = 2)
        self.batch1 = nn.BatchNorm2d(64)
        self.conv2 = nn.Conv2d(64, 128, kernel_size = 5, padding = 1)
        self.batch2 = nn.BatchNorm2d(128)
        self.conv3 = nn.Conv2d(128, 256, kernel_size = 3, padding = 1)
        self.batch3 = nn.BatchNorm2d(256)
        self.conv4 = nn.Conv2d(256, 512, kernel_size = 3, padding = 1)
        self.batch4 = nn.BatchNorm2d(512)
        self.conv5 = nn.Conv2d(512, 1024, kernel_size = 3, padding = 1)
        self.batch5 = nn.BatchNorm2d(1024)
        self.conv6 = nn.Conv2d(1024, 2048, kernel_size = 3, padding = 1)
        self.batch6 = nn.BatchNorm2d(2048)
        self.conv7 = nn.Conv2d(2048, 512, kernel_size = 3, padding = 1)
        self.batch7 = nn.BatchNorm2d(512)
        self.conv8 = nn.Conv2d(512, 384, kernel_size = 3, padding = 1)
        self.batch8 = nn.BatchNorm2d(384)
        self.conv9 = nn.Conv2d(384, 256, kernel_size = 3, padding = 1)
        self.batch9 = nn.BatchNorm2d(256)
        self.conv10 = nn.Conv2d(256, 256, kernel_size = 3, padding = 1)
        self.batch10 = nn.BatchNorm2d(256)
        self.pool = nn.MaxPool2d(2, 2)
        self.avgpool2d = nn.AdaptiveAvgPool2d((6, 6))
        self.dropout = nn.Dropout(0.1)
        self.fc1 = nn.Linear(256 * 6 * 6, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, NUM_CLASSES)
        return
    
    def forward(self, x):
        x = self.pool(F.relu(self.batch1(self.conv1(x))))
        x = self.pool(F.relu(self.batch2(self.conv2(x))))
        x = F.relu(self.batch3(self.conv3(x)))
        x = F.relu(self.batch4(self.conv4(x)))
        x = F.relu(self.batch5(self.conv5(x)))
        x = F.relu(self.batch6(self.conv6(x)))
        x = F.relu(self.batch7(self.conv7(x)))
        x = F.relu(self.batch8(self.conv8(x)))
        x = F.relu(self.batch9(self.conv9(x)))
        x = F.relu(self.batch10(self.conv10(x)))
        x = self.avgpool2d(self.pool(x))
        x = x.view(x.size()[0], -1)
        x = F.relu(self.dropout(self.fc1(x)))
        x = F.relu(self.fc2(x))
        x = self.dropout(self.fc3(x))
        return x


