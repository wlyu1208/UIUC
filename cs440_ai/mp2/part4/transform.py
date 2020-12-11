
# transform.py
# ---------------
# Licensing Information:  You are free to use or extend this projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to the University of Illinois at Urbana-Champaign
# 
# Created by Jongdeog Lee (jlee700@illinois.edu) on 09/12/2018

"""
This file contains the transform function that converts the robot arm map
to the maze.
"""
import copy
from arm import Arm
from maze import Maze
from search import *
from geometry import *
from const import *
from util import *

def transformToMaze(arm, goals, obstacles, window, granularity):
    """This function transforms the given 2D map to the maze in MP1.
    
        Args:
            arm (Arm): arm instance
            goals (list): [(x, y, r)] of goals
            obstacles (list): [(x, y, r)] of obstacles
            window (tuple): (width, height) of the window
            granularity (int): unit of increasing/decreasing degree for angles

        Return:
            Maze: the maze instance generated based on input arguments.

    """

    # row = alpha, col = beta
    # num of position = max - min / grannularity + 1

    # initial alpha, beta = starting point
    # alpha and beta at which the arm tip touches the goals would be the goals in the maze.
    # alpha and beta at which the arm cannot reach would be the walls in the maze.
    # construct walls and goals, you need to use geometry functions defined in Part 1
    #print("start transform")
    limit = arm.getArmLimit()
    start = arm.getArmAngle()
    num_arm = len(limit)
    #print(limit, start)
    #print(limit)

    if num_arm == 1:
        alpha_min = limit[0][0]
        alpha_max = limit[0][1]
        beta_min = 0
        beta_max = 0
        gamma_min = 0
        gamma_max = 0
    elif num_arm == 2:
        alpha_min = limit[0][0]
        alpha_max = limit[0][1]
        beta_min = limit[1][0]
        beta_max = limit[1][1]
        gamma_min = 0
        gamma_max = 0
    else:
        alpha_min = limit[0][0]
        alpha_max = limit[0][1]
        beta_min = limit[1][0]
        beta_max = limit[1][1]
        gamma_min = limit[2][0]
        gamma_max = limit[2][1]
    
    #print("check: ", alpha_max, beta_max, gamma_max)

    map = []

    if num_arm == 1:
        x_axis = int( (alpha_max-alpha_min) / granularity ) + 1
        y_axis = 0
        z_axis = 0
        map = [[[SPACE_CHAR for z in range(1)] for col in range(1)] for row in range(x_axis)]
    elif num_arm == 2:
        x_axis = int( (alpha_max-alpha_min) / granularity ) + 1
        y_axis = int( (beta_max-beta_min) / granularity ) + 1
        z_axis = 0
        map = [[[SPACE_CHAR for z in range(1)] for col in range(y_axis)] for row in range(x_axis)]
    else:
        x_axis = int( (alpha_max-alpha_min) / granularity ) + 1
        y_axis = int( (beta_max-beta_min) / granularity ) + 1
        z_axis = int( (gamma_max-gamma_min) / granularity ) + 1
        map = [[[SPACE_CHAR for z in range(z_axis)] for col in range(y_axis)] for row in range(x_axis)]

    start_point = angleToIdx(start, [alpha_min, beta_min, gamma_min], granularity)
    if num_arm == 1:
        start_index = (start_point[0], 0, 0)
    elif num_arm == 2:
        start_index = (start_point[0], start_point[1], 0)
    else:
        start_index = (start_point[0], start_point[1], start_point[2])
    #print(start_index)
    map[start_index[0]][start_index[1]][start_index[2]] = START_CHAR
    
    if num_arm == 1:
        x_index = alpha_min
        y_index = 0
        z_index = 0
    elif num_arm == 2:
        x_index = alpha_min
        y_index = beta_min
        z_index = 0
    else:
        x_index = alpha_min
        y_index = beta_min
        z_index = gamma_min

    while x_index <= alpha_max:
        while y_index <= beta_max:
            while z_index <= gamma_max:
                current_angle = (x_index, y_index, z_index)
                arm.setArmAngle(current_angle)
                current_idx = angleToIdx(current_angle, (alpha_min, beta_min, gamma_min), granularity)

                if doesArmTouchObjects(arm.getArmPosDist(), obstacles, False):
                    map[current_idx[0]][current_idx[1]][current_idx[2]] = WALL_CHAR
                elif doesArmTouchObjects(arm.getArmPosDist(), goals, True) and not doesArmTipTouchGoals(arm.getEnd(), goals):
                    map[current_idx[0]][current_idx[1]][current_idx[2]] = WALL_CHAR
                elif doesArmTouchObjects(arm.getArmPosDist(), goals, True) and doesArmTipTouchGoals(arm.getEnd(), goals):
                    map[current_idx[0]][current_idx[1]][current_idx[2]] = OBJECTIVE_CHAR
                elif not isArmWithinWindow(arm.getArmPos(), window):
                    map[current_idx[0]][current_idx[1]][current_idx[2]] = WALL_CHAR
                z_index += granularity
            
            y_index += granularity
            z_index = gamma_min
        
        x_index += granularity
        y_index = beta_min
    
    final_maze = Maze(map, (alpha_min, beta_min, gamma_min), granularity)

    return final_maze

    pass