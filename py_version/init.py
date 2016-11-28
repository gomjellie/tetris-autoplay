from keyboard import KeyBoard
from observable import Observable
from observer import Observer
from input_handler import InputHandler#Actor

if __name__ == '__main__':
    observable = Observable()
    keyBoard = KeyBoard()
    input_handler = InputHandler()
    observable.register(input_handler)
    while 1:
    	observable.update_observers(c = keyBoard.get_ch())
