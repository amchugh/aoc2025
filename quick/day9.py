
with open('resources/day8.txt') as f:
    data = f.read().splitlines()

print(data)

squares = []
biggest = 0

for line in data:
    x, y = map(int, line.split(','))
    for sq in squares:
        dx = sq[0] - x + 1
        dy = sq[1] - y + 1
        biggest = max(biggest, dx * dy)
    squares.append((x, y))

print(biggest)
