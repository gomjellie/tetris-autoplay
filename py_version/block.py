from movable import Movable
from board import Board

class Block(Movable):
    def __init__(self):
        self.x = Board.X_LENGTH / 2
        self.y = Board.Y_LENGTH / 2
        self.matrix = None

    def set_board_ref(self, board):
        self.board = board.matrix

    def move_right(self):
        self.x = self.x - 1
        print('move right')

    def move_left(self):
        self.x = self.x + 1
        print('move left')

    def move_down(self):
        self.y = self.y + 1
        print('move down')

    def move_up(self):
        self.y = self.y - 1
        print('move up')

    def move_bottom(self):
        while(self.is_movable()):
            self.y = self.y + 1
        print('move bottom')

    def is_movable(self):
        for y in range(len(self.matrix)):
            for x in range(len(self.matrix[0])):
                return not self.board[y+self.y][x+self.x]

