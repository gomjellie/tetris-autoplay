from abc import ABCMeta
from abc import abstractmethod

class Command(metaclass=ABCMeta):
    @abstractmethod
    def execute(self):
    	pass

class Command_Right(Command):
    def execute(self, block):
    	block.move_right()

class Command_Left(Command):
    def execute(self, block):
    	block.move_left()

class Command_Up(Command):
    def execute(self, block):
    	block.move_up()

class Command_Down(Command):
    def execute(self, block):
    	block.move_down()

class Command_Space(Command):
    def execute(self, block):
    	block.move_bottom()
