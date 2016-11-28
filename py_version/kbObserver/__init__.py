from keyboard import KeyBoard
from observable import Observable
from observer import Observer
from actor import Actor

if __name__ == '__main__':
    observable = Observable()
    keyBoard = KeyBoard()
    actor = Actor()
    observable.register(actor)
    while 1:
    	observable.update_observers(c = keyBoard.get_ch())
