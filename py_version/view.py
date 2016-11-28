import curses
from observer import Observer

class View(Observer):
    def __init__(self, screen):
    	self.scr = screen

    def update(self, *args, **kwargs):
        if not kwargs.get('c') is None:
    	    self.scr.addstr('asdf')
    	    self.scr.refresh()
    	    self.scr.move(0, 0)

