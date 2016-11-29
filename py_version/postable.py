from abc import ABCMeta
from abc import abstractmethod

class Postable(metaclass=ABCMeta):
    @abstractmethod
    def post():
    	pass

    @abstractmethod
    def is_postable():
    	pass
   
