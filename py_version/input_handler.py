from observer import Observer
from abc import ABCMeta
from abc import abstractmethod

class Command(metaclass=ABCMeta):
    @abstractmethod
    def execute(self):
    	pass

class Command_Right(Command):
    def execute(self):
    	print('right')

class Command_Left(Command):
    def execute(self):
    	print('left')

class Command_Up(Command):
    def execute(self):
    	print('up')

class Command_Down(Command):
    def execute(self):
    	print('down')

class InputHandler(Observer):
    def __init__(self):
        self.button_left = Command_Left()
        self.button_right = Command_Right()
        self.button_up = Command_Up()
        self.button_down = Command_Down()
        self.c = 0
    def handle_input(self, key):
        if key == 'right':
            self.button_right.execute()
        elif key == 'left':
            self.button_left.execute()
        elif key == 'down':
            self.button_down.execute()
        elif key == 'up':
            self.button_up.execute()
        else :
            print('unexpected button pressed')
    def update(self, *args, **kwargs):
        if not kwargs.get('c') is None:
            self.handle_input(kwargs.get('c'))

