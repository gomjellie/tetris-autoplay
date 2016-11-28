from observer import Observer
from command import *
from block import Block

class InputHandler(Observer):
    def __init__(self):
        self.button_left = Command_Left()
        self.button_right = Command_Right()
        self.button_up = Command_Up()
        self.button_down = Command_Down()
        self.c = 0
        self.block = Block()
    def handle_input(self, key):
        if key == 'right':
            self.button_right.execute(self.block)
        elif key == 'left':
            self.button_left.execute(self.block)
        elif key == 'down':
            self.button_down.execute(self.block)
        elif key == 'up':
            self.button_up.execute(self.block)
        else :
            print('unexpected button pressed')
    def update(self, *args, **kwargs):
        if not kwargs.get('c') is None:
            self.handle_input(kwargs.get('c'))

