from numpy.linalg import eig, inv, svd
from math import atan2
import numpy as np

def BoundingEllipseOfBestFit(mesh):
    # get the edge bonds
    triangles=mesh.cells[0].data
    x = [[[triangle[0],triangle[1]],[triangle[0],triangle[2]],[triangle[1],triangle[2]] ]   for triangle in triangles]
    flattenedx = [val for sublist in x for val in sublist]
    bonds = [[x[0],x[1]] if x[0]<x[1] else [x[1],x[0]] for x in flattenedx]

    edgebonds=[]
    for elem in bonds:
        if 1==bonds.count(elem):
            edgebonds.append(elem)
   

    bondvertices=[val for bond in edgebonds for val in bond]
    uniqueids= list(set(bondvertices))
    edgepoints= mesh.points[uniqueids]
    
    x=edgepoints[:,0]
    y=edgepoints[:,1]
    return fit_ellipse(x,y)

def TotalArea(mesh):
    TotalArea=0
    
    triangles=mesh.cells[0].data
    for triangle in triangles:
        v1 = mesh.points[triangle[1]]-mesh.points[triangle[0]]
        v2 = mesh.points[triangle[2]]-mesh.points[triangle[0]]
        TriArea= 0.5*np.linalg.norm( (np.cross(v1,v2)))
        TotalArea = TotalArea+TriArea
    return TotalArea

# fit ellipse, taken from https://github.com/ndvanforeest/fit_ellipse/blob/master/run_fit_ellipse.py
def __fit_ellipse(x, y):
    x, y = x[:, np.newaxis], y[:, np.newaxis]
    D = np.hstack((x * x, x * y, y * y, x, y, np.ones_like(x)))
    S, C = np.dot(D.T, D), np.zeros([6, 6])
    C[0, 2], C[2, 0], C[1, 1] = 2, 2, -1
    U, s, V = svd(np.dot(inv(S), C))
    a = U[:, 0]
    return a

def ellipse_center(a):
    b, c, d, f, g, a = a[1] / 2, a[2], a[3] / 2, a[4] / 2, a[5], a[0]
    num = b * b - a * c
    x0 = (c * d - b * f) / num
    y0 = (a * f - b * d) / num
    return np.array([x0, y0])

def ellipse_axis_length(a):
    b, c, d, f, g, a = a[1] / 2, a[2], a[3] / 2, a[4] / 2, a[5], a[0]
    up = 2 * (a * f * f + c * d * d + g * b * b - 2 * b * d * f - a * c * g)
    down1 = (b * b - a * c) * (
        (c - a) * np.sqrt(1 + 4 * b * b / ((a - c) * (a - c))) - (c + a)
    )
    down2 = (b * b - a * c) * (
        (a - c) * np.sqrt(1 + 4 * b * b / ((a - c) * (a - c))) - (c + a)
    )
    res1 = np.sqrt(up / down1)
    res2 = np.sqrt(up / down2)
    return np.array([res1, res2])

def ellipse_angle_of_rotation(a):
    b, c, d, f, g, a = a[1] / 2, a[2], a[3] / 2, a[4] / 2, a[5], a[0]
    return atan2(2 * b, (a - c)) / 2

def fit_ellipse(x, y):
    """@brief fit an ellipse to supplied data points: the 5 params
        returned are:

        M - major axis length
        m - minor axis length
        cx - ellipse centre (x coord.)
        cy - ellipse centre (y coord.)
        phi - rotation angle of ellipse bounding box

    @param x first coordinate of points to fit (array)
    @param y second coord. of points to fit (array)
    """
    a = __fit_ellipse(x, y)
    centre = ellipse_center(a)
    phi = ellipse_angle_of_rotation(a)
    M, m = ellipse_axis_length(a)
    # assert that the major axix M > minor axis m
    if m > M:
        M, m = m, M
    # ensure the angle is betwen 0 and 2*pi
    phi -= 2 * np.pi * int(phi / (2 * np.pi))
    return [M, m, centre[0], centre[1], phi]
