from movable import Movable
from board import Board

class Block(Movable):
    def __init__(self):
    	self.x = Board.X_LENGTH / 2
    	self.y = Board.Y_LENGTH / 2
    	self.matrix = [4][4]
    
    def move_right(self):
    	print('move right')
    
    def move_left(self):
    	print('move left')
    
    def move_down(self):
    	print('move down')
    
    def move_up(self):
    	print('move up')

    def move_bottom(self):
    	print('move bottom')

