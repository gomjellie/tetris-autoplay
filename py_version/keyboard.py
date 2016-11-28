import curses
import time
    
class KeyBoard(object):
    '''
    on MAC 
        68=>left, 66=>down, 67=>right, 65=>up

    '''
    LEFT = 68
    DOWN = 66
    RIGHT = 67
    UP = 65
    def __init__(self):
    	self.c = 0
    	self.stdscr = curses.initscr()
    	curses.noecho()
    	curses.cbreak()
    	self.stdscr.nodelay(1)
    	
    def get_ch(self):
        try:
            self.c = self.stdscr.getch()
        except KeyboardInterrupt:
            curses.endwin()
            exit()
        if self.c != -1:
            if self.c == KeyBoard.LEFT:
                key_in = 'left'
            elif self.c == KeyBoard.UP:
                key_in = 'up'
            elif self.c == KeyBoard.RIGHT:
                key_in = 'right'
            elif self.c == KeyBoard.DOWN:
                key_in = 'down'
            else:
                key_in = None
#            if self.c is not None and key_in is not None:
                #self.stdscr.addstr(' ' + str(self.c) + ' ' + key_in)
#                self.stdscr.refresh()
#                self.stdscr.move(0, 0)
            return key_in
        return None

