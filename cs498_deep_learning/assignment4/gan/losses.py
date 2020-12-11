import torch
from torch.nn.functional import binary_cross_entropy_with_logits as bce_loss
from torch.autograd import Variable
#http://seba1511.net/tutorials/beginner/examples_autograd/two_layer_net_autograd.html
dtype = torch.FloatTensor

def discriminator_loss(logits_real, logits_fake):
    """
    Computes the discriminator loss.
    
    You should use the stable torch.nn.functional.binary_cross_entropy_with_logits 
    loss rather than using a separate softmax function followed by the binary cross
    entropy loss.
    
    Inputs:
    - logits_real: PyTorch Tensor of shape (N,) giving scores for the real data.
    - logits_fake: PyTorch Tensor of shape (N,) giving scores for the fake data.
    
    Returns:
    - loss: PyTorch Tensor containing (scalar) the loss for the discriminator.
    """
    
    loss = None
    
    ####################################
    #          YOUR CODE HERE          #
    ####################################
    
    
    ##########       END      ##########
    # function provides the calculation of both log components in the objective function (i.e. log(D(x)) and log(1−D(G(z)))), generated, real
    #https://pytorch.org/docs/stable/nn.functional.html
#     print(logits_real)
#     print("next: ", logits_fake)
    N = logits_real.size()
    target_real = Variable(torch.ones(N)).type(dtype).cuda()
    target_generate = Variable(torch.zeros(N)).type(dtype).cuda()
    real = bce_loss(logits_real, target_real).cuda()
    generated = bce_loss(logits_fake, target_generate).cuda()
    loss = real + generated
    return loss

def generator_loss(logits_fake):
    """
    Computes the generator loss.
    
    You should use the stable torch.nn.functional.binary_cross_entropy_with_logits 
    loss rather than using a separate softmax function followed by the binary cross
    entropy loss.

    Inputs:
    - logits_fake: PyTorch Tensor of shape (N,) giving scores for the fake data.
    
    Returns:
    - loss: PyTorch Tensor containing the (scalar) loss for the generator.
    """
    
    loss = None
    
    ####################################
    #          YOUR CODE HERE          #
    ####################################
    
    
    ##########       END      ##########
    N  = logits_fake.size()
    target = Variable(torch.ones(N)).type(dtype).cuda()
    loss = bce_loss(logits_fake, target)

    return loss


def ls_discriminator_loss(scores_real, scores_fake):
    """
    Compute the Least-Squares GAN loss for the discriminator.
    
    Inputs:
    - scores_real: PyTorch Tensor of shape (N,) giving scores for the real data.
    - scores_fake: PyTorch Tensor of shape (N,) giving scores for the fake data.
    
    Outputs:
    - loss: A PyTorch Tensor containing the loss.
    """
    
    loss = None
    
    ####################################
    #          YOUR CODE HERE          #
    ####################################
    #https://wiseodd.github.io/techblog/2017/03/02/least-squares-gan/
    N = scores_real.size()
    target = Variable(torch.ones(N)).type(dtype).cuda()
    real = (1/2) * torch.mean( torch.pow(scores_real - target , 2) )
    generated = (1/2) * torch.mean( torch.pow(scores_fake , 2) )
    loss = real + generated
    ##########       END      ##########
    
    return loss

def ls_generator_loss(scores_fake):
    """
    Computes the Least-Squares GAN loss for the generator.
    
    Inputs:
    - scores_fake: PyTorch Tensor of shape (N,) giving scores for the fake data.
    
    Outputs:
    - loss: A PyTorch Tensor containing the loss.
    """
    
    loss = None
    
    ####################################
    #          YOUR CODE HERE          #
    ####################################
    N = scores_fake.size()
    target = Variable(torch.ones(N)).type(dtype).cuda()
    
    loss = (1/2) * torch.mean( torch.pow(scores_fake - target , 2) )
    
    ##########       END      ##########
    
    return loss
