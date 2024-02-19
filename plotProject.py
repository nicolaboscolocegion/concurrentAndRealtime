import matplotlib.pyplot as plt

'''
the memUsage.log has as it's first line the size of the queue
the and for the rest of the lines are the various logs of memory occupation
'''

# Read data from the file
with open('memUsage.log', 'r') as file:
    lines = file.read().splitlines()

# Extract maximum y-value and other y-values
max_y = int(lines[0])
y_values = list(map(int, lines[1:]))

# Plot the graph
x_values = list(range(1, len(y_values) + 1))

plt.plot(x_values, y_values, marker='o', linestyle='-', color='b')
plt.title('Mem usage')
plt.xlabel('time (unit deppends on the code)')
plt.ylabel('queue occupation')
plt.ylim(0, max_y)  # Set y-axis limit based on the maximum y-value
plt.yticks(range(0, max_y + 1, max_y // 10))
plt.grid(axis='y', linestyle='--', alpha=0.7)

plt.show()
