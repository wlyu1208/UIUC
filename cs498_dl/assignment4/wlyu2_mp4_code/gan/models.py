import torch
from spectral_normalization import SpectralNorm
from torch import nn

class Discriminator(torch.nn.Module):
    def __init__(self, input_channels=3):
        super(Discriminator, self).__init__()
        
        #Hint: Hint: Apply spectral normalization to convolutional layers. Input to SpectralNorm should be your conv nn module
        ####################################
        #          YOUR CODE HERE          #
        ####################################       
        self.conv1 = SpectralNorm(nn.Conv2d(input_channels, 128, kernel_size = 4, stride = 2, padding = 1, bias = False))
        self.conv2 = SpectralNorm(nn.Conv2d(128, 256, kernel_size = 4, stride = 2, padding = 1, bias = False))
        self.batch2 = nn.BatchNorm2d(256)
        self.conv3 = SpectralNorm(nn.Conv2d(256, 512, kernel_size = 4, stride = 2, padding = 1, bias = False))
        self.batch3 = nn.BatchNorm2d(512)
        self.conv4 = SpectralNorm(nn.Conv2d(512, 1024, kernel_size = 4, stride = 2, padding = 1, bias = False))
        self.batch4 = nn.BatchNorm2d(1024)
        # 0 or 1 for padding, no relu at the end 
        self.conv5 = SpectralNorm(nn.Conv2d(1024, 1, kernel_size = 4, stride = 1, padding = 0, bias = False))
        ##########       END      ##########
    
    def forward(self, x):
        
        ####################################
        #          YOUR CODE HERE          #
        ####################################
        
        
        ##########       END      ##########
        x = nn.LeakyReLU(0.2, inplace=True)(self.conv1(x))
        x = nn.LeakyReLU(0.2, inplace=True)(self.batch2(self.conv2(x)))
        x = nn.LeakyReLU(0.2, inplace=True)(self.batch3(self.conv3(x)))
        x = nn.LeakyReLU(0.2, inplace=True)(self.batch4(self.conv4(x)))
        x = self.conv5(x)
        return x


class Generator(torch.nn.Module):
    def __init__(self, noise_dim, output_channels=3):
        super(Generator, self).__init__()    
        self.noise_dim = noise_dim
        
        ####################################
        #          YOUR CODE HERE          #
        ####################################
        self.conv1 = nn.ConvTranspose2d(noise_dim, 1024, kernel_size = 4, stride = 1, padding = 0, bias = False)
        self.batch1 = nn.BatchNorm2d(1024)
        self.conv2 = nn.ConvTranspose2d(1024, 512, kernel_size = 4, stride = 2, padding = 1, bias = False)
        self.batch2 = nn.BatchNorm2d(512)
        self.conv3 = nn.ConvTranspose2d(512, 256, kernel_size = 4, stride = 2, padding = 1, bias = False)
        self.batch3 = nn.BatchNorm2d(256)
        self.conv4 = nn.ConvTranspose2d(256, 128, kernel_size = 4, stride = 2, padding = 1, bias = False)
        self.batch4 = nn.BatchNorm2d(128)
        self.conv5 = nn.ConvTranspose2d(128, 3, kernel_size = 4, stride = 2, padding = 1, bias = False)
        self.t = nn.Tanh()
        self.r = nn.ReLU(True)
        ##########       END      ##########
    
    def forward(self, x):
        
        ####################################
        #          YOUR CODE HERE          #
        ####################################
        x = x.view(-1, self.noise_dim, 1, 1)
        x = self.batch1(self.conv1(x))
        x = self.r(x)
        x = self.batch2(self.conv2(x))
        x = self.r(x)
        x = self.batch3(self.conv3(x))
        x = self.r(x)
        x = self.batch4(self.conv4(x))
        x = self.r(x)
        x = self.conv5(x)
        x = self.t(x)
        ##########       END      ##########
        
        return x
    

