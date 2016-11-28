from movable import Movable

class Block(Movable):
    def __init__(self):
    	self.x = 0
    	self.y = 0
    
    def move_right(self):
    	print('move right')
    
    def move_left(self):
    	print('move left')
    
    def move_down(self):
    	print('move down')
    
    def move_up(self):
    	print('move up')

