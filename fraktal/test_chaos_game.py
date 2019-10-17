from chaos_game import ChaosGame
import nose.tools as nt
import numpy as np

@nt.raises(TypeError, ValueError)
def test_error_n():
    a = ChaosGame(0.1)
    b = ChaosGame(2)

@nt.raises(TypeError, ValueError)
def test_error_r():
    a = ChaosGame(3, 1)
    b = ChaosGame(3, 1.1)

@nt.raises(NameError)
def test_savepng():
    a = ChaosGame(3)
    a.iterate(30000)
    a.savepng('sierpinski.pdf')

def test_equilateral_triangle():
    a = ChaosGame(3)
    edge1 = np.linalg.norm(a.corners[0]-a.corners[1])
    edge2 = np.linalg.norm(a.corners[0]-a.corners[2])
    edge3 = np.linalg.norm(a.corners[1]-a.corners[2])
    nt.assert_almost_equal(edge1, edge2)
    nt.assert_almost_equal(edge1, edge3)

def test_equilateral_square():
    b = ChaosGame(4)
    edge1 = np.linalg.norm(b.corners[0]-b.corners[1])
    edge2 = np.linalg.norm(b.corners[1]-b.corners[2])
    edge3 = np.linalg.norm(b.corners[2]-b.corners[3])
    edge4 = np.linalg.norm(b.corners[3]-b.corners[0])
    nt.assert_almost_equal(edge1, edge2)
    nt.assert_almost_equal(edge1, edge3)
    nt.assert_almost_equal(edge1, edge4)
