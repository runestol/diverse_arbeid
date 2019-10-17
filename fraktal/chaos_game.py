import numpy as np
import matplotlib.pyplot as plt

class ChaosGame():
    """Play the Chaos Game"""

    def __init__(self, n, r=0.5):
        """
        Initialize object and generate n-gon.

        Keyword arguments:
        n -- dimension of n-gon
        r -- ratio between two points to move between (default 1/2)
        """
        if type(n) != int:
            raise TypeError('n must be integer.')
        elif n < 3:
            raise ValueError('n must be larger than or equal to 3.')
        self.n = n

        if type(r) != float:
            raise TypeError('r must be float.')
        elif r < 0 or r > 1:
            raise ValueError('r must be in the interval (0,1)')
        self.r = r

        self._generate_ngon()

    def _generate_ngon(self):
        """Generate an equilateral n-gon by specifying its corners."""
        angles = np.linspace(0, 2*np.pi, self.n+1)
        self.corners = [np.asarray((np.sin(i), np.cos(i))) for i in angles[:-1]]

    def _starting_point(self):
        """Pick a random starting point x0 within the n-gon."""
        rands = np.random.random(self.n)
        rands /= sum(rands)
        self.x0 = sum([self.corners[i]*rands[i] for i in range(self.n)])

    def iterate(self, steps, discard=5):
        """
        Perform the chaos game-algorithm while discarding the first few
        generated points.

        Keyword arguments:
        steps -- number of iterations
        discard -- number of points at the start to discard (default 5)
        """
        self._starting_point()

        xi = self.x0.copy()
        for i in range(discard):
            cj = self.corners[np.random.randint(self.n)]
            xi = self.r*xi + (1-self.r)*cj

        x = np.zeros((steps, 2)); x[0] = xi
        for i in range(steps-1):
            cj = self.corners[np.random.randint(self.n)]
            x[i+1] = self.r*x[i] + (1-self.r)*cj

        self.x = x

    def plot_ngon(self):
        """Plot the n-gon"""
        plt.figure('{}-gon'.format(self.n))
        plt.scatter(*zip(*self.corners))
        plt.axis('equal')
        plt.axis('off')
        plt.show()

    def show(self):
        """Plot the game after playing"""
        plt.figure('Chaos game for a {}-gon'.format(self.n))
        plt.scatter(*zip(*self.x), s=0.5, color='black')
        plt.axis('equal')
        plt.axis('off')
        plt.show()

    def savepng(self, outfile):
        """
        Save a png-file of the plot.

        Keyword arguments:
        outfile -- the name of the png-file
        """
        if outfile[-4:] == '.png':
            filename = outfile
        elif '.' in outfile:
            raise NameError('File ending must be .png or none')
        else:
            filename = outfile + '.png'

        plt.figure('Chaos game for a {}-gon'.format(self.n))
        plt.scatter(*zip(*self.x), s=0.5, color='black')
        plt.axis('equal')
        plt.axis('off')

        plt.savefig(filename, transparent=True, dpi=300)
        plt.close()


if __name__ == '__main__':
    n = [3, 4, 5, 5, 6]
    r = [1./2, 1./3, 1./3, 3./8, 1./3]
    for i in range(len(n)):
        a = ChaosGame(n[i], r[i])
        a.iterate(30000)
        a.savepng('figures/chaos{}'.format(i+1))
