#!/usr/bin/env python3

import matplotlib.pyplot as plt

import sys

if (len(sys.argv) == 1):
    print("inputs: point_file_path tour_file_path")
    sys.exit()

point_file_path = sys.argv[1]

coordinates = []
with open(point_file_path, "r") as f:
    for line in f:
        if "NODE_COORD_SECTION" in line:
            break
    for line in f:
        line = line.strip()
        if "EOF" in line or not line:
            break
        fields = line.strip().split()
        coordinates.append((float(fields[1]), float(fields[2])))

x = [c[0] for c in coordinates]
y = [c[1] for c in coordinates]
plt.plot(x, y, "xk")

def read_and_plot_tour(tour_file_path, color):
    tour = []
    with open(tour_file_path, "r") as f:
        for line in f:
            if "TOUR_SECTION" in line:
                break
        for line in f:
            line = line.strip()
            if "-1" in line or "EOF" in line or not line:
                break
            fields = line.strip().split()
            tour.append((int(fields[0])))

    for i in range(len(tour) - 1):
        c = coordinates[tour[i] - 1]
        n = coordinates[tour[i + 1] - 1]
        plt.plot([c[0], n[0]], [c[1], n[1]], color)
    c = coordinates[tour[-1] - 1]
    n = coordinates[tour[0] - 1]
    plt.plot([c[0], n[0]], [c[1], n[1]], color)
    plt.axis("equal")

colors = ["b", "r:"]
for i in range(2, len(sys.argv)):
    read_and_plot_tour(sys.argv[i], colors[i % len(colors)])

plt.show()
