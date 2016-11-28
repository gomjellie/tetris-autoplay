from keyboard import KeyBoard
from observable import Observable
from observer import Observer
from input_handler import InputHandler#Actor
from view import View
import curses

if __name__ == '__main__':
    observable = Observable()
    screen = curses.initscr()
    curses.noecho()
    curses.cbreak()
    keyBoard = KeyBoard(screen)
    input_handler = InputHandler()
    view = View(screen)
    observable.register(input_handler)
    observable.register(view)
    while 1:
    	try:
    	    observable.update_observers(c=keyBoard.get_ch())
    	except KeyboardInterrupt:
    	    curses.endwin()
    	    exit()
