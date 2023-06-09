import numpy as np
import matplotlib.pyplot as plt
class SecondOrderDynamics:
    xp = 0.0
    y,yd = 0.0,0.0
    k1,k2,k3 = 0.0,0.0,0.0
    dt = 0.0
    def __init__(self,f,z,r,x0=0,dt=0.01):
        pi = 3.141592657
        self.k1 = z/pi/f
        self.k2 = 1/((2*pi*f)*(2*pi*f))
        self.k3 = r*z/(2*pi*f)
        self.dt = dt
        self.xp = x0
        self.y = x0
        self.yd = 0
    def Update(self, x):
        xd = (x-self.xp)/self.dt
        self.y = self.y+self.yd*self.dt
        self.yd = self.yd+self.dt*(x+self.k3*xd-self.y-self.k1*self.yd)/self.k2
        return self.y
    


if __name__ =="__main__":
    sys = SecondOrderDynamics(f=8,z=0.8,r=0.)
    times = np.arange(0,500)
    # 期望的状态值
    x_goal = np.zeros_like(times)
    for i in range(0,100):
        x_goal[i] = 0
    for i in range(100,np.size(x_goal,0)):
        x_goal[i] = 135*3.14/180
    y_list = np.zeros_like(times).astype('float32')
    # 经过二阶系统调整后的状态，消除了阶跃
    for i in times:
        y_list[i] = sys.Update(x_goal[i])
    
    plt.plot(times, y_list)
    plt.plot(times, x_goal)
    plt.show()
    
