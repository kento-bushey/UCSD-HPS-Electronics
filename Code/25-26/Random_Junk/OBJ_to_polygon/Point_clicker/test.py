import matplotlib.pyplot as plt

Hpoints = [
    [99, 6], [281, 8], [281, 246], [526, 247], [528, 7], [702, 5],
    [702, 599], [529, 600], [527, 345], [282, 341], [280, 598], [98, 598]
]

x, y = zip(*Hpoints)

plt.figure(figsize=(6, 6))
plt.plot(x + (x[0],), y + (y[0],), marker='o')  # connect points in order, close loop
plt.gca().invert_yaxis()  # optional: flip y-axis if matching typical screen coordinates
plt.title("H Polygon")
plt.show()
