import scipy.integrate as sp
import numpy as np
import math
import matplotlib.pyplot as plt
import matplotlib.animation as animation

g = 9.81        # gravitational acceleration [m/s**2]

class DoublePendulum():
    '''
    Class for describing the behaviour of a double pendulum
    '''
    def __init__(self, M1=1, L1=1, M2=1, L2=1):
        # mass of ball and length of rope for pendulum 1 an 2, respectively
        self.M1, self.M2, self.L1, self. L2 = M1, M2, L1, L2
        self._t = None

    def __call__(self, t, y):
        '''
        t: an array of time steps from (0,T)
        y: a tuple of (theta1, omega1, theta2, omega2)
        '''
        M1, M2, L1, L2 = self.M1, self.M2, self.L1, self.L2

        # time derivatives of theta1 and theta2
        dthetadt1 = y[1]
        dthetadt2 = y[3]

        # theta2 - theta1
        dtheta = y[2] - y[0]

        # time derivatives of omega1 and omega2
        domegadt1 = (M2*L1*y[1]**2*np.sin(dtheta)*np.cos(dtheta)
                    + M2*g*np.sin(y[2])*np.cos(dtheta)
                    + M2*L2*y[3]**2*np.sin(dtheta)
                    - (M1 + M2)*g*np.sin(y[0])) / ((M1
                    + M2)*L1 - M2*L1*np.cos(dtheta)**2)
        domegadt2 = (-M2*L2*y[3]**2*np.sin(dtheta)*np.cos(dtheta)
                    + (M1 + M2)*g*np.sin(y[0])*np.cos(dtheta)
                    - (M1 + M2)*L1*y[1]**2*np.sin(dtheta)
                    - (M1 + M2)*g*np.sin(y[2])) / ((M1
                    + M2)*L2 - M2*L2*np.cos(dtheta)**2)

        return (dthetadt1, domegadt1, dthetadt2, domegadt2)

    def solve(self, y0, T, dt, angles='rad'):
        '''
        method for solving initial value problem for motion of a double pendulum

        y0:     A tuple of (theta1_0, omega1_0, theta2_0, omega2_0)
        T:      Timespan
        angles: Optional keyword argument, by default 'rad' for radians
                or set to 'deg' for degrees
        '''
        self.dt = dt
        time = np.linspace(0, T+1, dt)

        # determining 'angles'-keyword functionality
        if angles == 'deg':
            math.radians(y0[0])
            math.radians(y0[2])
        elif angles == 'rad':
            pass
        else:
            raise NameError('Keyword angles must be rad or deg as string.')

        solution = sp.solve_ivp(self, (0,T+1), y0, t_eval=time, method='Radau')
        self._t = solution.t
        self._theta1, self._omega1, self._theta2, self._omega2 = solution.y

    # making properties of t, theta1, theta2, x1, y1, x2, y2, v, potential
    # and kinetic energy
    @property
    def t(self):
        return self._t

    @property
    def theta1(self):
        return self._theta1
    @property
    def theta2(self):
        return self._theta2

    @property
    def x1(self):
        self._x1 = self.L1*np.sin(self.theta1)
        return self._x1
    @property
    def y1(self):
        self._y1 = -self.L1*np.cos(self.theta1)
        return self._y1
    @property
    def x2(self):
        self._x2 = self.x1 + self.L2*np.sin(self.theta2)
        return self._x2
    @property
    def y2(self):
        self._y2 = self.y1 - self.L2*np.cos(self.theta2)
        return self._y2

    @property
    def v(self):
        vx1 = np.gradient(self.x1, self.t)
        vy1 = np.gradient(self.y1, self.t)
        vx2 = np.gradient(self.x2, self.t)
        vy2 = np.gradient(self.y2, self.t)
        self._v = np.asarray([vx1, vy1, vx2, vy2])
        return self._v

    @property
    def potential(self):
        P1 = self.M1*g*(self.y1 + self.L1)
        P2 = self.M2*g*(self.y2 + self.L1 + self.L2)
        self._P = P1 + P2
        return self._P

    @property
    def kinetic(self):
        K1 = 0.5*self.M1*(self.v[0]**2 + self.v[1]**2)
        K2 = 0.5*self.M2*(self.v[2]**2 + self.v[3]**2)
        self._K = K1 + K2
        return self._K

    # animation methods
    def init_frame(self):
        # Create empty figure
        fig = plt.figure()

        # Configure figure
        plt.axis('equal')
        plt.axis('off')
        plt.axis((-5, 5, -5, 5))

        # Make an "empty" plot object to be updated throughout the animation
        self.pendulums, = plt.plot([], [], 'o-', lw=2)

        # Call FuncAnimation
        self.animation = animation.FuncAnimation(fig,
                                                 self._next_frame,
                                                 frames=range(len(self.x1)),
                                                 repeat=None,
                                                 interval=1000*self.dt,
                                                 blit=True)

    def _next_frame(self, i):
        self.pendulums.set_data((0, self.x1[i], self.x2[i]), \
                                (0, self.y1[i], self.y2[i]))
        return self.pendulums,

    def show_animation(self):
        plt.show()

    def save_animation(self, filename):
        self.animation.save(filename, fps=60)


if __name__ == '__main__':
    '''
    a = DoublePendulum()
    a.solve((1,1,1,1), 10, 1001)

    plt.plot(a.t, a.kinetic, color='violet')
    plt.plot(a.t, a.potential, color='darkmagenta')
    plt.plot(a.t, a.kinetic + a.potential, color='skyblue')
    plt.legend(['Kinetic', 'Potential', 'Sum'], loc = 'best')
    plt.figure('motion')
    plt.plot(a.t, a.theta2, 'k')
    plt.plot(a.t, a.theta1, 'cyan')
    plt.show()
    '''

    a = DoublePendulum(L1=1, L2=2, M1=2, M2=1)
    a.solve((np.pi/4,1,np.pi/3,4), 10, 600)
    a.init_frame()
    #a.show_animation()
    a.save_animation('example_simulation.mp4')
