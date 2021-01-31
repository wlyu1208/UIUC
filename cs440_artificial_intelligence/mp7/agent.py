import numpy as np
import utils
import random
import math

# 480 x 480 = 12 x 12 where grid = 40
min_grid = 1
max_grid = 12

class Agent:
    
    def __init__(self, actions, Ne, C, gamma):
        self.actions = actions
        self.Ne = Ne # used in exploration function
        self.C = C
        self.gamma = gamma

        # Create the Q and N Table to work with
        self.Q = utils.create_q_table()
        self.N = utils.create_q_table()

        self.reset()
        
    def train(self):
        self._train = True
        
    def eval(self):
        self._train = False

    # At the end of training save the trained model
    def save_model(self,model_path):
        utils.save(model_path, self.Q)

    # Load the trained model for evaluation
    def load_model(self,model_path):
        self.Q = utils.load(model_path)

    def reset(self):
        self.points = 0
        self.s = None
        self.a = None

    def discretize(self, state):
        snake_head_x = state[0]
        snake_head_y = state[1]
        snake_body = state[2]
        food_x = state[3]
        food_y = state[4]
        
        grid = utils.GRID_SIZE
        
        # 1 wall on left , 2 wall on right
        adjoining_wall_x = 0
        if snake_head_x / grid == min_grid:
            adjoining_wall_x = 1
        elif snake_head_x / grid == max_grid:
            adjoining_wall_x = 2
        
        adjoining_wall_y = 0
        if snake_head_y / grid == min_grid:
            adjoining_wall_y = 1
        elif snake_head_y / grid == max_grid:
            adjoining_wall_y = 2
        
        # 1 food left to snake, 2 foor right to snake
        # 1 food < snake, 2 food > snake
        food_dir_x = 0
        if food_x < snake_head_x:
            food_dir_x = 1
        elif food_x > snake_head_x:
            food_dir_x = 2
        
        food_dir_y = 0
        if food_y < snake_head_y:
            food_dir_y = 1
        elif food_y > snake_head_y:
            food_dir_y = 2
        
        # adjoin square on snake body = 1
        # top = x, y - grid
        # bottom = x, y + grid
        # left = x - grid, y
        # right = x + grid, y
        
        adjoining_body_top = 0
        adjoining_body_bottom = 0
        adjoining_body_left = 0
        adjoining_body_right = 0
        
        for each_body in snake_body:
            current_x = each_body[0]
            current_y = each_body[1]
            
            if snake_head_x == current_x and snake_head_y - grid == current_y:
                adjoining_body_top = 1
            if snake_head_x == current_x and snake_head_y + grid == current_y:
                adjoining_body_bottom = 1
            if snake_head_x - grid == current_x and snake_head_y == current_y:
                adjoining_body_left = 1
            if snake_head_x + grid == current_x and snake_head_y == current_y:
                adjoining_body_right = 1
        
        return adjoining_wall_x, adjoining_wall_y, food_dir_x, food_dir_y, adjoining_body_top, adjoining_body_bottom, adjoining_body_left, adjoining_body_right
    
    def act(self, state, points, dead):
        '''
        :param state: a list of [snake_head_x, snake_head_y, snake_body, food_x, food_y] from environment.
        :param points: float, the current points from environment
        :param dead: boolean, if the snake is dead
        :return: the index of action. 0,1,2,3 indicates up,down,left,right separately
        :ties: right > left > down > up : 3 > 2 > 1 > 0

        TODO: write your function here.
        Return the index of action the snake needs to take, according to the state and points known from environment.
        Tips: you need to discretize the state to the state space defined on the webpage first.
        (Note that [adjoining_wall_x=0, adjoining_wall_y=0] is also the case when snake runs out of the 480x480 board)

        '''
        # print("state: ", state)
        # print("points: ", points)
        # print("dead: ", dead)

        current_state = self.discretize(state)
        
        #find rewards
        if dead:
            rewards = -1
        elif points - self.points > 0:
            rewards = 1
        else:
            rewards = -0.1

        #update q_table
        #Q(s,a) += alpha(reward + gamma * max(curr round s, curr round a) - Q(s,a))
        #Q(s,a) : 
        if self.s is not None and self.a is not None and self._train:
            max_q = -math.inf
            prev_s = self.s

            #max act on current state with q table | find max based on current q table
            for each_act in self.actions:
                #act 0 - 3
                #print(each_act)
                current_q = self.Q[current_state[0], current_state[1],current_state[2], current_state[3],current_state[4], current_state[5],current_state[6], current_state[7], each_act]
                if current_q > max_q:
                    max_q = current_q
            
            #alpha = C / (C+N(s, a))
            alpha = self.C / (self.C + self.N[prev_s[0], prev_s[1],prev_s[2], prev_s[3],prev_s[4], prev_s[5],prev_s[6], prev_s[7], self.a])
            
            self.Q[prev_s[0], prev_s[1],prev_s[2], prev_s[3],prev_s[4], prev_s[5],prev_s[6], prev_s[7], self.a] += alpha * (rewards + self.gamma * max_q - self.Q[prev_s[0], prev_s[1],prev_s[2], prev_s[3],prev_s[4], prev_s[5],prev_s[6], prev_s[7], self.a])
            
        #if dead, update q table and return, not update N table    
        if dead:
            self.reset()
            return
        
        #exploration policy
        index_action = -1
        max_q = -math.inf
        
        for each_act in self.actions:
            current_n = self.N[current_state[0], current_state[1],current_state[2], current_state[3],current_state[4], current_state[5],current_state[6], current_state[7], each_act]

            #if n is less than Ne, return 1 or return u:Q(s,a)
            if current_n < self.Ne:
                max_q = 1
                index_action = each_act
            else:
                current_q = self.Q[current_state[0], current_state[1],current_state[2], current_state[3],current_state[4], current_state[5],current_state[6], current_state[7], each_act]
                if current_q >= max_q:
                    max_q = current_q
                    index_action = each_act
                    
        self.N[current_state[0], current_state[1],current_state[2], current_state[3],current_state[4], current_state[5],current_state[6], current_state[7], index_action] += 1
        self.s = current_state
        self.points = points
        self.a = index_action
           
        return index_action
