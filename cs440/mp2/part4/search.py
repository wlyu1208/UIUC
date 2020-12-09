# search.py
# ---------------
# Licensing Information:  You are free to use or extend this projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to the University of Illinois at Urbana-Champaign
#
# Created by Jongdeog Lee (jlee700@illinois.edu) on 09/12/2018

"""
This file contains search functions.
"""
# Search should return the path and the number of states explored.
# The path should be a list of tuples in the form (alpha, beta, gamma) that correspond
# to the positions of the path taken by your search algorithm.
# Number of states explored should be a number.
# maze is a Maze object based on the maze from the file specified by input filename
# searchMethod is the search method specified by --method flag (bfs,astar)
# You may need to slight change your previous search functions in MP1 since this is 3-d maze

from collections import deque
from heapq import heappop, heappush

def search(maze, searchMethod):
    return {
        "bfs": bfs,
    }.get(searchMethod, [])(maze)

def bfs(maze):
    # Write your code here
    """
    This function returns optimal path in a list, which contains start and objective.
    If no path found, return None. 
    """


     # If maze is nothing return empty path
    if maze is None:
        return []

    #get start and goal tuple
    start = maze.getStart()

    # use list as queue and put start tuple in list
    q = []
    q = [[start]]

    # init visit set list 
    visit = []
    visit.append(start)
    # print(visit)
    flag = 0
    while q:
        # get first path
        # print(q)
        path = q[0]
        q.pop(0)
        # print(path)

        #get last node of path
        node = path[len(path)-1]
        # print(node)
        #if last node is the goal, end bfs
        #print("here?")
        #print(maze.isObjective(node[0], node[1]))
        if maze.isObjective(node[0], node[1], node[2]):
            flag = 1
            break

        #if not visit, mark visit and add neighbor with path
        for neighbor in maze.getNeighbors(node[0], node[1], node[2]):
            if neighbor not in visit:
                visit.append(neighbor)
                q.append(path+[neighbor])
    
                
    if flag == 0:
        return None
    #print(path)
    #print(maze.isValidPath(path))
    return path

    return []
