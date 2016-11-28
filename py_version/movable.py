from abc import ABCMeta
from abc import abstractmethod

class Movable(metaclass=ABCMeta):
    @abstractmethod
    def move_right():
    	pass
    
    @abstractmethod
    def move_left():
    	pass

    @abstractmethod
    def move_up():
    	pass

    @abstractmethod
    def move_down():
    	pass

