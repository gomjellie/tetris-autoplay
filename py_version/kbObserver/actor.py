from observer import Observer
class Actor(Observer):
    def __init__(self):
    	self.c = 0
    def update(self, *args, **kwargs):
        if not kwargs.get('c') is None:
            print('{0} '.format(kwargs.get('c')))

