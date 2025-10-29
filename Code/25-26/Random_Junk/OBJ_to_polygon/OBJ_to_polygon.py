def load_obj(filename):
    vertices = []
    polygons = []
    with open(filename) as f:
        for line in f:
            if line.startswith('v '):
                _, x, y, z = line.split()
                vertices.append((float(x), float(y), float(z)))
            elif line.startswith('f '):
                parts = [int(p.split('/')[0]) - 1 for p in line.split()[1:]]
                polygons.append([vertices[i] for i in parts])
    return polygons
def export_for_arduino(polygons):
    print("float polygons[][3][3] = {")
    for p in polygons:
        print("  {", end="")
        for v in p:
            print(f"{{{v[0]:.3f}, {v[1]:.3f}, {v[2]:.3f}}},", end="")
        print("},")
    print("};")

polygons = load_obj('S.obj')
export_for_arduino(polygons)

