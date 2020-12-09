# geometry.py
# ---------------
# Licensing Information:  You are free to use or extend this projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to the University of Illinois at Urbana-Champaign
#
# Created by Jongdeog Lee (jlee700@illinois.edu) on 09/12/2018

"""
This file contains geometry functions that relate with Part1 in MP2.
"""

import math
import numpy as np
from const import *

def computeCoordinate(start, length, angle):
    """Compute the end cooridinate based on the given start position, length and angle.

        Args:
            start (tuple): base of the arm link. (x-coordinate, y-coordinate)
            length (int): length of the arm link
            angle (int): degree of the arm link from x-axis to couter-clockwise

        Return:
            End position (int,int):of the arm link, (x-coordinate, y-coordinate)
    """
    radian = math.radians(angle)

    x = start[0] + int(math.cos(radian)*length)
    y = start[1] - int(math.sin(radian)*length)
    return (x, y)
#reference:  
#http://mathworld.wolfram.com/Circle-LineIntersection.html
#https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
#https://brilliant.org/wiki/dot-product-distance-between-point-and-a-line/
# only got idea from this site
def doesArmTouchObjects(armPosDist, objects, isGoal=False):
    """Determine whether the given arm links touch any obstacle or goal

        Args:
            armPosDist (list): start and end position and padding distance of all arm links [(start, end, distance)]
            objects (list): x-, y- coordinate and radius of object (obstacles or goals) [(x, y, r)]
            isGoal (bool): True if the object is a goal and False if the object is an obstacle.
                           When the object is an obstacle, consider padding distance.
                           When the object is a goal, no need to consider padding distance.
        Return:
            True if touched. False if not.
    """

    for position in armPosDist:
        start_x = position[0][0]
        start_y = position[0][1]
        end_x = position[1][0]
        end_y = position[1][1]
        distance = position[2]

        for object in objects:
            #print(object)
            obj_x = object[0]
            obj_y = object[1]
            padding = object[2]

            if isGoal == True:
                padding += 0
            else:
                padding += distance

            up = obj_x - start_x
            right = obj_y - start_y
            down = end_x - start_x
            left = end_y - start_y
            #print(up, right, down, left)
            
            dy = np.dot(up, down) + np.dot(right, left)
            dx = np.dot(down, down) + np.dot(left, left)
            #print(dy, dx)

            if dx == 0:
                slope = -1
            else:
                slope = dy / dx
            
            if slope >= 0 and slope <= 1:
                x_to_goal = start_x + slope * down
                y_to_goal = start_y + slope * left

            elif slope < 0:
                x_to_goal = start_x
                y_to_goal = start_y

            else:
                x_to_goal = end_x
                y_to_goal = end_y

            #print(obj_x, obj_y, x_to_goal, y_to_goal)

            current = find_dist(obj_x, obj_y, x_to_goal, y_to_goal)
            #print(current, padding)
            if current <= padding:
                return True
    return False

def find_dist(horiz_obj, vert_obj, horiz_goal, vert_goal):
    d_x = horiz_obj - horiz_goal
    d_y = vert_obj - vert_goal

    return math.sqrt(d_x**2 + d_y**2)

def doesArmTipTouchGoals(armEnd, goals):
    """Determine whether the given arm tick touch goals

        Args:
            armEnd (tuple): the arm tick position, (x-coordinate, y-coordinate)
            goals (list): x-, y- coordinate and radius of goals [(x, y, r)]. There can be more than one goal.
        Return:
            True if arm tip touches any goal. False if not.
    """

    for end in goals:
        d_x = end[0] - armEnd[0]
        d_y = end[1] - armEnd[1]
        temp = d_x**2+ d_y**2
        current = math.sqrt(temp)

        if current <= end[2]:
            answer = True
        else:
            answer = False
    return answer


def isArmWithinWindow(armPos, window):
    """Determine whether the given arm stays in the window

        Args:
            armPos (list): start and end positions of all arm links [(start, end)]
            window (tuple): (width, height) of the window

        Return:
            True if all parts are in the window. False if not.
    """
    #print(armPos, window)

    for position in armPos:
        start_x = position[0][0]
        start_y = position[0][1]
        end_x = position[1][0]
        end_y = position[1][1]

        if start_x < 0 or start_x > window[0]:
            return False
        if start_y < 0 or start_y > window[1]:
            return False

        if end_x < 0 or end_x > window[0]:
            return False
        if end_y < 0 or end_y > window[1]:
            return False

    return True


if __name__ == '__main__':
    computeCoordinateParameters = [((150, 190),100,20), ((150, 190),100,40), ((150, 190),100,60), ((150, 190),100,160)]
    resultComputeCoordinate = [(243, 156), (226, 126), (200, 104), (57, 156)]
    testRestuls = [computeCoordinate(start, length, angle) for start, length, angle in computeCoordinateParameters]
    assert testRestuls == resultComputeCoordinate

    testArmPosDists = [((100,100), (135, 110), 4), ((135, 110), (150, 150), 5)]
    testObstacles = [[(120, 100, 5)], [(110, 110, 20)], [(160, 160, 5)], [(130, 105, 10)]]
    resultDoesArmTouchObjects = [
        True, True, False, True, False, True, False, True,
        False, True, False, True, False, False, False, True
    ]
    
    testResults = []
    for testArmPosDist in testArmPosDists:
        for testObstacle in testObstacles:
            testResults.append(doesArmTouchObjects([testArmPosDist], testObstacle))
            #print(testArmPosDist)
            #print(doesArmTouchObjects([testArmPosDist], testObstacle))
    
    print("\n")
    for testArmPosDist in testArmPosDists:
        for testObstacle in testObstacles:
            testResults.append(doesArmTouchObjects([testArmPosDist], testObstacle, isGoal=True))
            #print(testArmPosDist)
            #print(doesArmTouchObjects([testArmPosDist], testObstacle, isGoal=True))
    
    assert resultDoesArmTouchObjects == testResults
    
    testArmEnds = [(100, 100), (95, 95), (90, 90)]
    testGoal = [(100, 100, 10)]
    resultDoesArmTouchGoals = [True, True, False]
    
    testResults = [doesArmTipTouchGoals(testArmEnd, testGoal) for testArmEnd in testArmEnds]
    assert resultDoesArmTouchGoals == testResults
    
    testArmPoss = [((100,100), (135, 110)), ((135, 110), (150, 150))]
    testWindows = [(160, 130), (130, 170), (200, 200)]
    resultIsArmWithinWindow = [True, False, True, False, False, True]
    testResults = []
    for testArmPos in testArmPoss:
        for testWindow in testWindows:
            testResults.append(isArmWithinWindow([testArmPos], testWindow))
    assert resultIsArmWithinWindow == testResults

    print("Test passed\n")
