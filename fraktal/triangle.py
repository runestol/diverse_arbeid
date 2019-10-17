import numpy as np
import matplotlib.pyplot as plt

# Defining corners of an equilateral triangle
c1 = np.asarray((0, 0))
c2 = np.asarray((0, 1))
c3 = np.asarray((np.cos(np.pi/6), np.sin(np.pi/6)))

triangle = [c1, c2, c3]

# Finding a random starting point within our triangle
rands = np.random.random(3)
rands /= sum(rands)
x0 = sum([triangle[i]*rands[i] for i in range(3)])

def sierpinski_monochrome(triangle, n):
    """
    Perform the chaos-game algorithm for a provided triangle
    and plot the results with one color.

    Keyword arguments:
    triangle -- a list of three two-dimensional arrays representing
                the corners of an equilateral triangle.
    n -- the number of iterations for the algorithm.
    """
    x = np.zeros((n, 2))
    x[0] = x0

    for i in range(n-1):
        # Drawing a random triangle corner
        cj = triangle[np.random.randint(3)]
        # Generating a new point and storing it in x
        x[i+1] = (x[i] + cj)/2

    plt.figure('monochrome')
    plt.scatter(*zip(*x[6:]), s=0.2)
    plt.axis('equal')
    plt.axis('off')

def sierpinski_tricolor(triangle, n):
    """
    Perform the chaos-game algorithm for a provided triangle
    and plot the results with colored points depending on which corner
    used to calculate its position.

    Keyword arguments:
    triangle -- a list of three two-dimensional arrays representing
                the corners of an equilateral triangle.
    n -- the number of iterations for the algorithm.
    """
    x = np.zeros((n, 2)); x[0] = x0
    colors = np.zeros(n)

    for i in range(n-1):
        # Drawing a random triangle corner
        r = np.random.randint(3)
        cj = triangle[r]
        colors[i] = r
        # Generating a new point and storing it in x
        x[i+1] = (x[i] + cj)/2

    # Discarding x0 and first 5 generated points
    x = x[6:]
    colors = colors[6:]

    # Assigning colors to points based on the corner it moved towards
    red = x[colors == 0]
    blue = x[colors == 1]
    green = x[colors == 2]

    plt.figure('tricolor')
    plt.scatter(*zip(*red), s=0.1, color='red')
    plt.scatter(*zip(*blue), s=0.1, color='blue')
    plt.scatter(*zip(*green), s=0.1, color='green')
    plt.axis('equal')
    plt.axis('off')

def sierpinski_spectrum(triangle, n):
    """
    Perform the chaos-game algorithm for a provided triangle
    and plot the results with colored points depending on which corner
    used to calculate its position.

    Keyword arguments:
    triangle -- a list of three two-dimensional arrays representing
                the corners of an equilateral triangle.
    n -- the number of iterations for the algorithm.
    """
    x = np.zeros((n, 2)); x[0] = x0
    colors = np.zeros((n, 3));
    rgbvec = np.asarray([(1,0.5,0), (0,1,0.5), (0.5,0,1)])

    for i in range(n-1):
        # Drawing a random triangle corner
        r = np.random.randint(3)
        cj = triangle[r]

        # Generating a new point and storing it in x
        x[i+1] = (x[i] + cj)/2
        # Calculating the new color the same way
        colors[i+1] = (colors[i] + rgbvec[r])/2

    # Discarding x0 and first 5 generated points
    x = x[6:]
    colors = colors[6:]

    plt.figure('rgb-spectrum')
    plt.scatter(*zip(*x), c=colors, s=0.2)
    plt.axis('equal')
    plt.axis('off')

iterations = 30005
sierpinski_monochrome(triangle, iterations)
sierpinski_tricolor(triangle, iterations)
sierpinski_spectrum(triangle, iterations)

plt.show()
