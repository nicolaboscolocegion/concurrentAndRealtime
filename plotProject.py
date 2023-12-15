import matplotlib.pyplot as plt

# Read data from the file
with open('filename.txt', 'r') as file:
    lines = file.read().splitlines()

# Extract maximum y-value and other y-values
max_y = int(lines[0])
y_values = list(map(int, lines[1:]))

# Plot the graph
x_values = list(range(1, len(y_values) + 1))

plt.plot(x_values, y_values, marker='o', linestyle='-', color='b')
plt.title('Graph from File Data')
plt.xlabel('X-axis')
plt.ylabel('Y-axis')
plt.ylim(0, max_y)  # Set y-axis limit based on the maximum y-value
plt.yticks(range(0, max_y + 1, max_y // 10))
plt.grid(axis='y', linestyle='--', alpha=0.7)

plt.show()