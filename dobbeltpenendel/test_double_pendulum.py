import nose.tools as nt
from double_pendulum import DoublePendulum
import numpy as np

def test_radius1():
    a = DoublePendulum()
    a.solve((0,0.1, 3,0.5),10,101)
    r2 = a.x1**2 + a.y1**2
    assert np.isclose(1**2, np.all(r2))

def test_radius2():
    a = DoublePendulum()
    a.solve((0,0.1, 3,0.5),10,101)
    r2 = a.x2**2 + a.y2**2
    assert np.isclose(1**2, np.all(r2))

def test_arrays_zero():
    '''
    test-function for checking if initial conditions y0=(0,0) yields zero-filled arrays
    '''
    a = DoublePendulum()
    a.solve((0.0, 0, 0.0, 0), 10, 1001)
    assert np.all(a.theta1 == 0) and np.all(a.theta2 == 0), \
           'initial conditions y0=(0,0) does not yield arrays with zeros'
