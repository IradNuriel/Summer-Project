from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import argparse
import functools


def init_scatter():
    fig = plt.figure()
    scatter = fig.add_subplot(111, projection='3d')
    scatter.set_xlabel('X')
    scatter.set_ylabel('Z')
    scatter.set_zlabel('Y')
    return scatter

def plot(scatter, points, marker, s=1):
    for point in points:
        xs, zs, ys = point
        scatter.scatter(xs, ys, zs, marker=marker, s=s)




def read_file(file):
    separators = [',', '[', ']']
    return [tuple(map(float, functools.reduce(lambda s, sep: s.replace(sep, ' '), separators, line).split()))
                  for line in fpoints.read().split('\n') if line]


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='a script for 3d view of points')
    parser.add_argument('--name', type=str, help='the file name which points you want to see', default="points.txt")
    parser.add_argument('--avgname', type=str, help='the file name which avg points you want to see', default="")

    args = parser.parse_args()
    scatter = init_scatter()
    with open(args.name,"r") as fpoints:
        plot(scatter, read_file(fpoints), 'o', 2)
    if args.avgname:
        with open(args.avgname, "r") as fpoints:
            plot(scatter, read_file(fpoints), '^', 30)
    plt.show()
