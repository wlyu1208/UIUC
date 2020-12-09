import torch
import torch.nn as nn
import torch.nn.functional as F
from torch.autograd import Variable
import numpy as np

class YoloLoss(nn.Module):
    def __init__(self,S,B,l_coord,l_noobj):
        super(YoloLoss,self).__init__()
        self.S = S
        self.B = B
        self.l_coord = l_coord
        self.l_noobj = l_noobj

    def compute_iou(self, box1, box2):
        '''Compute the intersection over union of two set of boxes, each box is [x1,y1,x2,y2].
        Args:
          box1: (tensor) bounding boxes, sized [N,4].
          box2: (tensor) bounding boxes, sized [M,4].
        Return:
          (tensor) iou, sized [N,M].
        '''
        N = box1.size(0)
        M = box2.size(0)

        lt = torch.max(
            box1[:,:2].unsqueeze(1).expand(N,M,2),  # [N,2] -> [N,1,2] -> [N,M,2]
            box2[:,:2].unsqueeze(0).expand(N,M,2),  # [M,2] -> [1,M,2] -> [N,M,2]
        )

        rb = torch.min(
            box1[:,2:].unsqueeze(1).expand(N,M,2),  # [N,2] -> [N,1,2] -> [N,M,2]
            box2[:,2:].unsqueeze(0).expand(N,M,2),  # [M,2] -> [1,M,2] -> [N,M,2]
        )

        wh = rb - lt  # [N,M,2]
        wh[wh<0] = 0  # clip at 0
        inter = wh[:,:,0] * wh[:,:,1]  # [N,M]
        
        #print(box1[:,2])
        area1 = (box1[:,2]-box1[:,0]) * (box1[:,3]-box1[:,1])  # [N,]
        area2 = (box2[:,2]-box2[:,0]) * (box2[:,3]-box2[:,1])  # [M,]
        area1 = area1.unsqueeze(1).expand_as(inter)  # [N,] -> [N,1] -> [N,M]
        area2 = area2.unsqueeze(0).expand_as(inter)  # [M,] -> [1,M] -> [N,M]

        iou = inter / (area1 + area2 - inter)
        return iou
    #https://medium.com/adventures-with-deep-learning/yolo-v1-part3-78f22bd97de4
    #https://www.programmersought.com/article/32563471568/
    #https://pytorch.org/docs/stable/generated/torch.nn.MSELoss.html#torch.nn.MSELoss
    #https://jonathan-hui.medium.com/real-time-object-detection-with-yolo-yolov2-28b1b93e2088
    #pascal voc: 7x7 (SxS), 2 boxes, 20 classes
    
    
    #https://jonathan-hui.medium.com/real-time-object-detection-with-yolo-yolov2-28b1b93e2088
    #==really helpful
    def get_class_prediction_loss(self, classes_pred, classes_target):
        """
        Parameters:
        classes_pred : (tensor) size (batch_size, S, S, 20)
        classes_target : (tensor) size (batch_size, S, S, 20)

        Returns:
        class_loss : scalar
        """
        
        ##### CODE #####
        #print(classes_pred.size())
        #print(classes_target)
        
        # size: (42, 20) = 42: number of cells        
        #avoid division by n
        
        class_loss = torch.sum((classes_pred - classes_target)**2)
        return class_loss
    
    
    def get_regression_loss(self, box_pred_response, box_target_response):   
        """
        Parameters:
        box_pred_response : (tensor) size (-1, 5)
        box_target_response : (tensor) size (-1, 5)
        Note : -1 corresponds to ravels the tensor into the dimension specified 
        See : https://pytorch.org/docs/stable/tensors.html#torch.Tensor.view_as

        Returns:
        reg_loss : scalar
        
        """
        
        ##### CODE #####
        #print("pred: ", box_pred_response)
        #print("target: ", box_target_response)
        
        #sum of first two lines, S=14 B=2 size: [42, 5]
        #box = [x, y, w, h, c]

        reg_x = torch.sum((box_pred_response[:,:1]-box_target_response[:,:1])**2)
        reg_y = torch.sum((box_pred_response[:,1:2]-box_target_response[:,1:2])**2)
        
        reg_w = torch.sum((torch.sqrt(box_pred_response[:,2:3])-torch.sqrt(box_target_response[:,2:3]))**2)
        reg_h = torch.sum((torch.sqrt(box_pred_response[:,3:4])-torch.sqrt(box_target_response[:,3:4]))**2)

        reg_loss = reg_x + reg_y + reg_w + reg_h
        
        return reg_loss
    
    def get_contain_conf_loss(self, box_pred_response, box_target_response_iou):
        """
        Parameters:
        box_pred_response : (tensor) size ( -1 , 5)
        box_target_response_iou : (tensor) size ( -1 , 5)
        Note : -1 corresponds to ravels the tensor into the dimension specified 
        See : https://pytorch.org/docs/stable/tensors.html#torch.Tensor.view_as

        Returns:
        contain_loss : scalar
        
        """
        
        ##### CODE #####
        contain_loss = torch.sum((box_pred_response[:,4:5] - box_target_response_iou[:,4:5])**2)
        return contain_loss
    
    def get_no_object_loss(self, target_tensor, pred_tensor, no_object_mask):
        """
        Parameters:
        target_tensor : (tensor) size (batch_size, S , S, 30)
        pred_tensor : (tensor) size (batch_size, S , S, 30)
        no_object_mask : (tensor) size (batch_size, S , S, 30)

        Returns:
        no_object_loss : scalar

        Hints:
        1) Create a 2 tensors no_object_prediction and no_object_target which only have the 
        values which have no object. 
        2) Have another tensor no_object_prediction_mask of the same size such that 
        mask with respect to both confidences of bounding boxes set to 1. 
        3) Create 2 tensors which are extracted from no_object_prediction and no_object_target using
        the mask created above to find the loss. 
        """
        
        ##### CODE #####
        
        no_object_prediction = pred_tensor[no_object_mask.bool()].view(-1, 30)
        no_object_target = target_tensor[no_object_mask.bool()].view(-1, 30)
        #print(no_object_prediction, no_object_target)
        
        
        no_object_prediction_mask = torch.zeros(no_object_target.size())
        no_object_prediction_mask[:,4] = 1
        no_object_prediction_mask[:,9] = 1
        #print(no_object_prediction_mask)
        
        no_object_prediction = no_object_prediction[no_object_prediction_mask.bool()]
        no_object_target = no_object_target[no_object_prediction_mask.bool()]
        
        no_object_loss = torch.sum((no_object_prediction - no_object_target)**2)
        
        return no_object_loss
        
    
    
    def find_best_iou_boxes(self, box_target, box_pred):
        """
        Parameters: 
        box_target : (tensor)  size (-1, 5)
        box_pred : (tensor) size (-1, 5)
        Note : -1 corresponds to ravels the tensor into the dimension specified 
        See : https://pytorch.org/docs/stable/tensors.html#torch.Tensor.view_as

        Returns: 
        box_target_iou: (tensor)
        contains_object_response_mask : (tensor)

        Hints:
        1) Find the iou's of each of the 2 bounding boxes of each grid cell of each image.
        2) Set the corresponding contains_object_response_mask of the bounding box with the max iou
        of the 2 bounding boxes of each grid cell to 1.
        3) For finding iou's use the compute_iou function
        4) Before using compute preprocess the bounding box coordinates in such a way that 
        if for a Box b the coordinates are represented by [x, y, w, h] then 
        x, y = x/S - 0.5*w, y/S - 0.5*h ; w, h = x/S + 0.5*w, y/S + 0.5*h
        Note: Over here initially x, y are the center of the box and w,h are width and height. 
        We perform this transformation to convert the correct coordinates into bounding box coordinates.
        5) Set the confidence of the box_target_iou of the bounding box to the maximum iou
        
        """
        
        ##### CODE #####
        #coo_reponse needs to be in tensor form
        
        coo_response_mask = torch.zeros(box_target.size(), dtype=torch.uint8).cuda()
        S = self.S
        box_target_iou = torch.zeros(box_target.size()).cuda()

        #box = [x, y, w, h, c]        
        for i in range(0, box_target.shape[0], 2):
            #x= x/S - 0.5*w
            #y= y/S - 0.5*h
            #w= x/S + 0.5*w
            #h= y/S + 0.5*h
           
            box_1 = box_pred[i:i+2]
            box_1_1 = box_1[0:1]
            box_1_2 = box_1[1:2]
            first_box = torch.zeros(box_1_1.size())
            second_box = torch.zeros(box_1_2.size())
            
            x = box_1_1[:,0:1]
            y = box_1_1[:,1:2]
            w = box_1_1[:,2:3]
            h = box_1_1[:,3:4]
            first_box[:,0:1] = x / S - 0.5 * w
            first_box[:,1:2] = y / S - 0.5 * h
            first_box[:,2:3] = x / S + 0.5 * w
            first_box[:,3:4] = y / S + 0.5 * h
            
            x = box_1_2[:,0:1]
            y = box_1_2[:,1:2]
            w = box_1_2[:,2:3]
            h = box_1_2[:,3:4]
            second_box[:,0:1] = x / S - 0.5 * w
            second_box[:,1:2] = y / S - 0.5 * h
            second_box[:,2:3] = x / S + 0.5 * w
            second_box[:,3:4] = y / S + 0.5 * h
            
            box_2 = box_target[i].view(-1, 5)
            target_box = torch.zeros(box_2.size())
            x = box_2[:,0:1]
            y = box_2[:,1:2]
            w = box_2[:,2:3]
            h = box_2[:,3:4]
            target_box[:,0:1] = x / S - 0.5 * w
            target_box[:,1:2] = y / S - 0.5 * h
            target_box[:,2:3] = x / S + 0.5 * w
            target_box[:,3:4] = y / S + 0.5 * h
            
            first_iou = self.compute_iou(first_box[:,:4], target_box[:,:4])
            second_iou = self.compute_iou(second_box[:,:4], target_box[:,:4])
            
            if second_iou > first_iou:
                max_idx = i+1
                maximum = second_iou
            else:
                max_idx = i
                maximum = first_iou
            coo_response_mask[max_idx] = 1
            box_target_iou[max_idx][4] = maximum.detach()
        #using variabvle is like require_grads=True
        box_target_iou = Variable(box_target_iou).cuda()
        return box_target_iou, coo_response_mask
        
    
    
    
    def forward(self, pred_tensor,target_tensor):
        '''
        pred_tensor: (tensor) size(batchsize,S,S,Bx5+20=30)
                      where B - number of bounding boxes this grid cell is a part of = 2
                            5 - number of bounding box values corresponding to [x, y, w, h, c]
                                where x - x_coord, y - y_coord, w - width, h - height, c - confidence of having an object
                            20 - number of classes
        
        target_tensor: (tensor) size(batchsize,S,S,30)
        
        Returns:
        Total Loss
        '''
        N = pred_tensor.size()[0]
        
        total_loss = None
        
        # Create 2 tensors contains_object_mask and no_object_mask 
        # of size (Batch_size, S, S) such that each value corresponds to if the confidence of having 
        # an object > 0 in the target tensor.
        # size = (24, 14, 14)
        # B = 2
        ##### CODE #####
        
        # https://www.kdnuggets.com/2018/05/pytorch-tensor-basics.html
        # https://github.com/pytorch/pytorch/issues/8994
        #box = [x, y, w, h, c]
        #:,:,:,:, = too many indicies
        # size needs to be in [24, 14, 14, 30]
        # last(c) of x, y, z
        #pass 4 or 5? => fifth value so 4
        contains_object_mask = target_tensor[:, :, :,4] > 0
        no_object_mask = target_tensor[:, :, :,4] == 0
        
#         print(contains_object_mask.size(), no_object_mask.size())
        #https://piazza.com/class/kdyxzd9ldz23vn?cid=503
        #unsqueeze make 24 14 14 => 24 14 14 1
        #size now [24, 14, 14, 1]
        #make to [24, 14, 14, 30]
        contains_object_mask = contains_object_mask.unsqueeze(-1).expand_as(target_tensor)
        no_object_mask = no_object_mask.unsqueeze(-1).expand_as(target_tensor)

        
        # Create a tensor contains_object_pred that corresponds to 
        # to all the predictions which seem to confidence > 0 for having an object
        # Split this tensor into 2 tensors :
        # 1) bounding_box_pred : Contains all the Bounding box predictions of all grid cells of all images
        # 2) classes_pred : Contains all the class predictions for each grid cell of each image
        # Hint : Use contains_object_mask
        
        ##### CODE #####
        
        #10 = self.B * 5 = 10
        #need contiguous to change the memory also
        #need shape of 30 for contain and bound for 5
        
        col_num = 5
        num_class = 20
        size_last_dim = self.B * col_num + num_class
        bound_box_index = self.B * col_num
        
        #size? =>flatten to 30 and 5
        contains_object_pred = pred_tensor[contains_object_mask.bool()].view(-1, size_last_dim)
        bounding_box_pred = contains_object_pred[:,:bound_box_index].contiguous().view(-1, col_num)
        classes_pred = contains_object_pred[:,bound_box_index:]
        
        # Similarly as above create 2 tensors bounding_box_target and
        # classes_target.
        
        ##### CODE #####
        #flatten to 30 and 5
        contains_object_target = target_tensor[contains_object_mask.bool()].view(-1, size_last_dim)
        bounding_box_target = contains_object_target[:,:bound_box_index].reshape((-1, col_num))
        classes_target = contains_object_target[:,bound_box_index:]
        
        
        # Compute the No object loss here
        
        ##### CODE #####
        #def get_no_object_loss(self, target_tensor, pred_tensor, no_object_mask):
        no_object_loss = self.get_no_object_loss(target_tensor, pred_tensor, no_object_mask.bool())
        
        
        # Compute the iou's of all bounding boxes and the mask for which bounding box 
        # of 2 has the maximum iou the bounding boxes for each grid cell of each image.
        
        ##### CODE #####
        #def find_best_iou_boxes(self, box_target, box_pred):
        #return box_target_iou, coo_response_mask
        box_target_iou, coo_response_mask = self.find_best_iou_boxes(bounding_box_target, bounding_box_pred)
        
        
        # Create 3 tensors :
        # 1) box_prediction_response - bounding box predictions for each grid cell which has the maximum iou
        # 2) box_target_response_iou - bounding box target ious for each grid cell which has the maximum iou
        # 3) box_target_response -  bounding box targets for each grid cell which has the maximum iou
        # Hint : Use contains_object_response_mask
        
        ##### CODE #####
        #flatten to 5
        box_prediction_response = bounding_box_pred[coo_response_mask.bool()].view(-1, col_num)
        box_target_response_iou = box_target_iou[coo_response_mask.bool()].view(-1, col_num)
        box_target_response = bounding_box_target[coo_response_mask.bool()].view(-1, col_num)
        
        
        # Find the class_loss, containing object loss and regression loss  
#         def get_class_prediction_loss(self, classes_pred, classes_target):
#         def get_regression_loss(self, box_pred_response, box_target_response):
#         def get_contain_conf_loss(self, box_pred_response, box_target_response_iou):
        
        ##### CODE #####
        class_loss = self.get_class_prediction_loss(classes_pred, classes_target)
        containing_object_loss = self.get_contain_conf_loss(box_prediction_response, box_target_response_iou)
        regression_loss = self.get_regression_loss(box_prediction_response, box_target_response)
        
        total_loss = self.l_coord * regression_loss + containing_object_loss + self.l_noobj * no_object_loss  + class_loss
        total_loss /= N
        return total_loss




