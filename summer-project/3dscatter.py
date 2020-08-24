from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import argparse
import functools




def plot(points):
    fig = plt.figure()
    scatter = fig.add_subplot(111, projection='3d')

    for point in points:
        xs, zs, ys = point
        scatter.scatter(xs, ys, zs, marker='o')

    scatter.set_xlabel('X')
    scatter.set_ylabel('Z')
    scatter.set_zlabel('Y')
    plt.show()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='a script for 3d view of points')
    parser.add_argument('--name', type=str, help='the file name which points you want to see', default="D:\\points.txt")

    args = parser.parse_args()
    separators = [',', '[', ']']
    with open(args.name,"r") as fpoints:
        points = [tuple(map(float, functools.reduce(lambda s, sep: s.replace(sep, ' '), separators, line).split()))
                  for line in fpoints.read().split('\n') if line]
        plot(points)
