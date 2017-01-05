from postable import Postable

class Board(Postable):
    X_LENGTH = 12
    Y_LENGTH = 22
    def __init__(self):
    	self.matrix = [Board.X_LENGTH][Board.Y_LENGTH]

    def post(self, block):
        for y in range(len(block.matrix)):
            for x in range(len(block.matrix[0])):
            	self.matrix[y+block.y][x+block.x] = block.matrix[y][x]

    def is_postable(self, block):
    	for y in range(len(block.matrix)):
            for x in range(len(block.matrix[0])):
            	if y+block.y < 0 or y+block.y > Board.Y_LENGTH or x + block.x < 0 or x + block.x > Board.X_LENGTH:
                        return false

