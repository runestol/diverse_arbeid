import numpy as np
import matplotlib.pyplot as plt

class AffineTransform():
    """Compute an affine transformation"""

    def __init__(self, a=0, b=0, c=0, d=0, e=0, f=0):
        """
        Initiate affine transformation matrix and vector

        Keyword arguments:
        a, b, c, d -- matrix elements
        e, f -- vector elements
        """
        self.a, self.b, self.c, self.d, self.e, self.f = a, b, c, d, e, f

    def __call__(self, x, y):
        """
        Compute the transformation

        Keyword arguments:
        x, y -- 2d point coordinates
        """
        return (self.a*x + self.b*y + self.e, self.c*x + self.d*y + self.f)


if __name__ == '__main__':
    f1 = AffineTransform(d=0.16)
    f2 = AffineTransform(0.85, 0.04, -0.04, 0.85, 0, 1.6)
    f3 = AffineTransform(0.2, -0.26, 0.23, 0.22, 0, 1.6)
    f4 = AffineTransform(-0.15, 0.28, 0.26, 0.24, 0, 0.44)
    functions = [f1, f2, f3, f4]

    p = [0.01, 0.85, 0.07, 0.07]
    p_cumulative = [sum(p[:i+1]) for i in range(len(p))]

    def choose_func():
        j = 0
        r = np.random.random()
        for p in p_cumulative:
            if r < p:
                return functions[j]
            j += 1

    iterations = 50000
    x = np.zeros((iterations, 2))
    for i in range(iterations-1):
        f = choose_func()
        x[i+1,0], x[i+1,1] = f(x[i,0], x[i,1])

    plt.scatter(*zip(*x), color='green', s=0.01)
    plt.axis('equal')
    plt.axis('off')
    plt.savefig('figures/barnsley_fern.png')
