#!venv/bin/python3
import rpyc

class Robot(object):
    """
        rob = Robot(ip = '192.168.0.14')

    """
    def __init__(self, ip):
        self.conn = rpyc.classic.connect(ip) # host name or IP address of the EV3
        self.ev3 = conn.modules['ev3dev.ev3']      # import ev3dev.ev3 remotely
        self.left_motor = ev3.LargeMotor('outB')
        self.right_motor = ev3.LargeMotor('outC')
        

    def __del__(self):
    	pass

    def go_straight(self):
        self.left_motor.run_timed(time_sp=1000, speed_sp=600)
        self.right_motor.run_timed(time_sp=1000, speed_sp=600)
    
    def turn_right(self):
    	self.left_motor.run_timed(time_sp=1000, speed_sp=600)
    	self.right_motor.stop()
    
    def turn_left(self):
    	self.left_motor.stop()
    	self.right_motor.run_timed(time_sp=1000, speed_sp=600)

    def go_back(self):
    	self.left_motor.run_timed(time_sp=1000,speed_sp=-600)
    	self.right_motor.run_timed(time_sp=1000,speed_sp=-600)
