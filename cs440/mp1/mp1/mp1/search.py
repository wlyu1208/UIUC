# search.py
# ---------------
# Licensing Information:  You are free to use or extend this projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to the University of Illinois at Urbana-Champaign
#
# Created by Michael Abir (abir2@illinois.edu) on 08/28/2018

"""
This is the main entry point for MP1. You should only modify code
within this file -- the unrevised staff files will be used for all other
files and classes when code is run, so be careful to not modify anything else.
"""
# Search should return the path.
# The path should be a list of tuples in the form (row, col) that correspond
# to the positions of the path taken by your search algorithm.
# maze is a Maze object based on the maze from the file specified by input filename
# searchMethod is the search method specified by --method flag (bfs,dfs,astar,astar_multi,fast)

from queue import PriorityQueue
import math
import heapq

def search(maze, searchMethod):
    return {
        "bfs": bfs,
        "astar": astar,
        "astar_corner": astar_corner,
        "astar_multi": astar_multi,
        "fast": fast,
    }.get(searchMethod)(maze)

# reference:
# https://www.geeksforgeeks.org/queue-in-python/
# https://pythoninwonderland.wordpress.com/2017/03/18/how-to-implement-breadth-first-search-in-python/
def bfs(maze):
    """
    Runs BFS for part 1 of the assignment.

    @param maze: The maze to execute the search on.

    @return path: a list of tuples containing the coordinates of each state in the computed path
    """
    # TODO: Write your code here
    
    # If maze is nothing return empty path
    if maze is None:
        return []

    #get start and goal tuple
    start = maze.getStart()
    goal = maze.getObjectives()[0]

    # use list as queue and put start tuple in list
    q = []
    q = [[start]]

    # init visit set list 
    visit = []
    visit.append(start)
    # print(visit)

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
        if node == goal:
            break

        #if not visit, mark visit and add neighbor with path
        for neighbor in maze.getNeighbors(node[0], node[1]):
            if neighbor not in visit:
                visit.append(neighbor)
                q.append(path+[neighbor])
        
    #print(path)
    #print(maze.isValidPath(path))
    return path

# reference:
# https://www.educative.io/edpresso/what-is-the-python-priority-queue
# https://www.redblobgames.com/pathfinding/a-star/implementation.html
# https://stackoverflow.com/questions/4159331/python-speed-up-an-a-star-pathfinding-algorithm

def heuristic(a, b):
    return abs(a[0]-b[0]) + abs(a[1]-b[1])

def find_path(end, begin, possible_answer):
    answer = []

    #push end and put node from possbile to answer until start comes out and reverse
    node = end
    answer.append(node)
    while node != begin:
        answer.append(possible_answer[node])
        node = possible_answer[node]
    #print(answer)
    answer.reverse()
    #print(answer)
    #cannot do "return answer.reverse()"
    return answer

def astar(maze):
    """
    Runs A star for part 1 of the assignment.

    @param maze: The maze to execute the search on.

    @return path: a list of tuples containing the coordinates of each state in the computed path
    """
    # TODO: Write your code here

    # return empty if maze is empty
    if maze is None:
        return []

    #get start and goal
    start = maze.getStart()
    goal = maze.getObjectives()[0]

    #use prioirity queue for open, need () inside put or get error on PQ
    frontier = PriorityQueue()
    frontier.put((heuristic(start, goal), start))
    #print(frontier.queue)
    
    # use closed for done and g_list for keep track of changing g_s
    closed = []
    g_list = {}
    g_list[start] = 0

    # list for possible path on astar
    possible_path = {}

    # break when node is goal
    while frontier:
        #print(g_list)
        # get first node on priority queue and put on done state
        node = frontier.get()[1]
        closed.append(node)
        #print(node)

        if(node == goal):
            break

        for neighbor in maze.getNeighbors(node[0], node[1]):
            # fing f = g + h
            next_g = 1 + g_list[node]
            h = heuristic(neighbor, goal)
            f = next_g + h

            # if neighbor never got g or new g is less change the path with g_list
            if neighbor not in closed and neighbor not in g_list or next_g < g_list[neighbor]:
                possible_path[neighbor] = node
                g_list[neighbor] = next_g
                frontier.put((f, neighbor))

    #print(possible_path)
    path = find_path(goal, start, possible_path)
    #print(path)
    return path

# reference: 
# https://www.tutorialspoint.com/prim-s-minimum-spanning-tree-mst-algorithm
# https://www.tutorialspoint.com/How-to-implement-Python-lt-gt-custom-overloaded-operators
#  

class NODE:
    def __init__(self, co, g, h, previous_node):
        self.co = co
        self.g = g
        self. h = h
        self.f = g+h
        self.previous_node = previous_node
        self.goals = []

    def __lt__(self, other):
        return self.f < other.f

    def print_node_info(self):
        print(self.co, self.g, self.h, self.f, self.previous_node, self.goals)

# same as astar on above but modifed for astar_corner
def astar_mst(maze, index):
    start = index[0]
    goal = index[1]

    frontier = PriorityQueue()
    frontier.put((heuristic(start, goal), start))
    
    closed = []
    g_list = {}
    g_list[start] = 0

    possible_path = {}

    while frontier:
        node = frontier.get()[1]
        closed.append(node)

        if(node == goal):
            break

        for neighbor in maze.getNeighbors(node[0], node[1]):
            next_g = 1 + g_list[node]
            h = heuristic(neighbor, goal)
            f = next_g + h

            if neighbor not in closed and neighbor not in g_list or next_g < g_list[neighbor]:
                possible_path[neighbor] = node
                g_list[neighbor] = next_g
                frontier.put((f, neighbor))

    path = []

    # do not append "goal" like astar or duplicate in astar_corner
    while node != start:
        path.append(possible_path[node])
        node = possible_path[node]
    path.reverse()
    #print(path)
    #print("astar_help: ", path)
    return path

def make_mst_list(maze):
    #from the start to each node in mst, make tree based on length with astar
    #skip same co-ord or give error of above length by duplicate
    for begin in mst_nodes:
        for end in mst_nodes:
            if begin == end:
                continue
            index = (begin, end)
            mst_path[index] = astar_mst(maze, index)
            mst_list[index] = len(mst_path[index])
            #print("path: ", mst_path)
            #print("list: ", mst_list)

# Return heuristic based on length on manhattan distance from start to each goal
def find_heuristic2(nodes):
    start = nodes[0]
    manhattan_result = 0

    for node in nodes:
        manhattan_result += heuristic(start, node)

    #print(nodes)
    #print(manhattan_result)
    return manhattan_result

def find_heuristic(nodes):
    #print("enter heuristic")
    #first one is start in nodes
    start = nodes[0]

    # mark all visit false except the start
    visit = {}
    for i in nodes:
        if i == start:
            visit[i] = True
        else:
            visit[i] = False

    #start is done node, start acts like inf
    done = start

    find = {}
    
    # need to init to 0 or get error when adding to itself
    mst_result = 0
    manhattan_result = 0

    while visit:
        if all(visit.values()) == True:
            break
        # make new list of find with no lowest and not visited
        for node in nodes:
            if node != done and visit[node] != True:
                find[(done, node)] = mst_list[(done, node)]
        #print(find)
        #print(min(find.values()))
        for i in find:
            if(find[i] == min(find.values())):
               short_edge = i
               short_edge_goal = i[1]

        # if goal was never visited, mark visit and add length of mst and remove the done goal
        if visit[short_edge_goal] == False:
            visit[short_edge_goal] = True
            mst_result += mst_list[short_edge]
            done = short_edge_goal
            #print(length_result)
            #print(short_edge)
        del find[short_edge]
    
    for node in nodes:
        manhattan_result += heuristic(start, node)

    #print(nodes)
    #print(manhattan_result, mst_result)
    result = min(manhattan_result, mst_result)
    return mst_result

def create_path(cur_node, start):
    answer = []

    while cur_node != start:
        #cur_node.print_node_info()
        before = cur_node.previous_node
        answer += mst_path[(cur_node.co, before.co)]
        cur_node = cur_node.previous_node
        #print(answer)

    answer.append(start.co)
    answer.reverse()
    return answer

# reference: https://www.cs.mcgill.ca/~ytamit/files/mst_sp.pdf
def astar_corner2(maze):
    """
    Runs A star for part 2 of the assignment in the case where there are four corner objectives.

    @param maze: The maze to execute the search on.

    @return path: a list of tuples containing the coordinates of each state in the computed path
        """
    # TODO: Write your code here

    # return empty if maze is empty
    if maze is None:
        return []

    #get start and goal
    start = maze.getStart()
    goal = maze.getObjectives()
    #print(goal)

    frontier = PriorityQueue()

    #make variables related to mst to global
    global mst_list
    global mst_path
    global mst_nodes

    mst_list = {}
    mst_path = {}
    mst_nodes = []

    #put goals to mst nodes and start on the front
    mst_nodes.append(start)
    for i in maze.getObjectives():
        mst_nodes.append(i)
    #print(mst_nodes)


    #make mst tree based on length
    make_mst_list(maze)

    #print(mst_path)
    #print(mst_list)

    #make start node and put goal to neighbor
    start_node = NODE(start, 0, 0, NODE((-10, -10), 0, 0, []))
    #start_node.print_node_info()

    for i in goal:
        start_node.goals.append(i)
    #print(start_node.neighbor)
    #start_node.print_node_info()
    
    frontier.put(start_node)

    while frontier:
        current = frontier.get()

        #current.print_node_info()
        if current.goals == []:
            break
        #use mst tree to change g and h
        for each in current.goals:
            #print(each, current.co)
            g = current.g + mst_list[(current.co, each)]
            h = find_heuristic(current.goals)
            #print("after heuristic")
            
            # make next node with current node in goals and put infos about it
            next_node = NODE(each, g, h, current)
            next_node.previous_node = current
            
            # check each goal and put in goals for not same one
            for others in current.goals:
                #print("check: ", negihbor_node, current.neighbor)
                if next_node.co != others:
                    next_node.goals.append(others)

            #next_node.print_node_info()
            frontier.put(next_node)

    # create path
    result_path = create_path(current, start_node)

    #print(result_path)
    #print(maze.isValidPath(result_path))
    return result_path

# same as astar corner
def astar_multi(maze):
   # return empty if maze is empty
    if maze is None:
        return []

    #get start and goal
    start = maze.getStart()
    goal = maze.getObjectives()
    #print(goal)

    frontier = PriorityQueue()

    #make variables related to mst to global
    global mst_list
    global mst_path
    global mst_nodes

    mst_list = {}
    mst_path = {}
    mst_nodes = []

    #put goals to mst nodes and start on the front
    mst_nodes.append(start)
    for i in maze.getObjectives():
        mst_nodes.append(i)
    #print(mst_nodes)


    #make mst tree based on length
    make_mst_list(maze)

    #print(mst_path)
    #print(mst_list)

    #make start node and put goal to neighbor
    start_node = NODE(start, 0, 0, NODE((-10, -10), 0, 0, []))
    #start_node.print_node_info()

    for i in goal:
        start_node.goals.append(i)
    #print(start_node.neighbor)
    #start_node.print_node_info()
    
    frontier.put(start_node)

    while frontier:
        current = frontier.get()

        #current.print_node_info()
        if current.goals == []:
            break
        #use mst tree to change g and h
        for each in current.goals:
            #print(each, current.co)
            g = current.g + mst_list[(current.co, each)]
            h = find_heuristic(current.goals)
            #print("after heuristic")
            
            # make next node with current node in goals and put infos about it
            next_node = NODE(each, g, h, current)
            next_node.previous_node = current
            
            # check each goal and put in goals for not same one
            for others in current.goals:
                #print("check: ", negihbor_node, current.neighbor)
                if next_node.co != others:
                    next_node.goals.append(others)

            #next_node.print_node_info()
            frontier.put(next_node)

    # create path
    result_path = create_path(current, start_node)

    #print(result_path)
    #print(maze.isValidPath(result_path))
    return result_path

def bfs_help(maze, index):

    start = index[0]
    goal = index[1]

    q = []
    q = [[start]]

    visit = []
    visit.append(start)

    while q:

        path = q[0]
        q.pop(0)

        node = path[len(path)-1]

        if node == goal:
            #remove goal in path before return
            path.pop()
            break

        for neighbor in maze.getNeighbors(node[0], node[1]):
            if neighbor not in visit:
                visit.append(neighbor)
                q.append(path+[neighbor])
    #print("bfs path: ", path, type(path))    
    return path

#reference: https://en.wikipedia.org/wiki/Fringe_search
#modified fringe search for fast calcualte path find
def fast(maze):
    """
    Runs suboptimal search algorithm for part 4.

    @param maze: The maze to execute the search on.

    @return path: a list of tuples containing the coordinates of each state in the computed path
    """
    # TODO: Write your code here

    start = maze.getStart()
    goals = maze.getObjectives()
    
    #start from closest goals
    goals.reverse()

    backtrack = {}

    node = start

    while goals:
        fmin = math.inf
        for each in goals:
            h = heuristic(node, each)
            #print(current, each, h, fmin)
            if h < fmin:
                fmin = h
                next = each
        backtrack[next] = node
        node = next
        goals.remove(next)
    
    #print("back: ", backtrack)
    
    connect_backtrack = []
    #print(backtrack)
    while backtrack:
        connect_backtrack.append(node)
        temp = node
        node = backtrack[node]
        backtrack.pop(temp)

    connect_backtrack.append(start)
    
    #print("list: ", connect_backtrack, type(connect_backtrack))
    
    path = []
    #print(len(connect_backtrack))
    for index, tuple in enumerate(connect_backtrack):
        #print(index, tuple)
        if index == len(connect_backtrack)-1:
            break
        path += bfs_help(maze, (connect_backtrack[index], connect_backtrack[index+1]))

    #print("before add start: ", path)
    path.append(start)
    #print("after add start: ", final_path)
    path.reverse()
    #print(path)
    #print(maze.isValidPath(path))
    return path


class Node:
    def __init__(self, position):
        self.position = position
        self.parent = None
        self.gScore = float("inf")
        self.hScore = 0
        self.fScore = float("inf")


def astar_corner(maze):
    """
    Runs A star for part 2 of the assignment in the case where there are four corner objectives.

    @param maze: The maze to execute the search on.

    @return path: a list of tuples containing the coordinates of each state in the computed path
        """
    # TODO: Write your code here
    start = maze.getStart()
    # if start == (2, 4):
    #     return testPath(maze)
    goal = maze.getObjectives()
    goal_list = {}
    for i in range(len(goal)):
        goal_list[goal[i]] = Node(goal[i])
    flag = 0
    foundGoal = ()
    temp = []
    path = []
    while goal_list:
        temp = astar_corner_helper(maze, start, goal_list)
        if flag:
            path.extend(temp[1:])
        else:
            path.extend(temp)
        flag += 1
        foundGoal = path[-1]
        goal_list.pop(foundGoal)
        start = foundGoal
    return path


def astar_corner_helper(maze, startpoint, goalset):
    frontier = []
    start = startpoint
    goal_dict = goalset
    start_node = Node(start)
    explored_set = {}
    start_node.gScore = 0
    start_node.hScore = manhattan(start, findNearestGoal(start_node, goal_dict))
    start_node.fScore = start_node.gScore + start_node.hScore
    explored_set[start] = start_node.fScore
    heapq.heappush(frontier, (start_node.fScore, start_node.position, start_node))
    while frontier:
        curr_node = (heapq.heappop(frontier))[2]
        if curr_node.position in goal_dict:
            return reconstruct_path(curr_node)
        neighbors = maze.getNeighbors(curr_node.position[0], curr_node.position[1])
        for node in neighbors:
            each_neighbor = Node(node)
            calculateScore(each_neighbor, curr_node, findNearestGoal(each_neighbor, goal_dict))
            if node not in explored_set:
                explored_set[each_neighbor.position] = each_neighbor.fScore
                heapq.heappush(frontier, (each_neighbor.fScore, each_neighbor.position, each_neighbor))
            else:
                if explored_set[node] > each_neighbor.fScore:
                    explored_set[node] = each_neighbor.fScore

    return []

def reconstruct_path(node):
    path = []
    while node.parent:
        path.append(node.position)
        node = node.parent
    path.append(node.position)
    return path[::-1]


def calculateScore(node, parent, goal):
    node.gScore = parent.gScore + 1
    node.hScore = manhattan(node.position, goal)
    node.fScore = node.gScore + node.hScore
    node.parent = parent
    return

def manhattan(curr, goal):
    return abs(curr[0] - goal[0]) + abs(curr[1] - goal[1])

def findNearestGoal(curr, goal_dict):
    min = float("inf")
    pos = (0, 0)
    for key in goal_dict:
        heuristic = manhattan(curr.position, key)
        if heuristic < min:
            pos = key
            min = heuristic

    return pos