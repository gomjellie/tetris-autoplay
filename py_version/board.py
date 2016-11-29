from postable import Postable

class Board(Postable):
    X_LENGTH = 12
    Y_LENGTH = 22
    def __init__(self):
    	self.matrix = [Board.X_LENGTH][Board.Y_LENGTH]

    def post(self, block):
    	pass

    def is_postable(self, block):
    	pass


