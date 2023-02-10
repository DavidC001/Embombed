import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd
import numpy as np
from stl import mesh

# Carica i dati dal file CSV
df = pd.read_csv('data.csv')

# Crea il grafico 3D
fig = plt.figure()
ax = Axes3D(fig)

# Per ogni classe, disegna i punti con un colore diverso
for class_name, class_df in df.groupby('class'):
    # Use the correct column names to access the data
    ax.scatter(class_df['X'], class_df['Y'], class_df['Z'], label=class_name)

    # OR use numeric indices to access the data
    # ax.scatter(class_df.iloc[:, 0], class_df.iloc[:, 1], class_df.iloc[:, 2], label=class_name)

# Mostra la legenda del grafico
ax.legend()

#add axes labels
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

#add title
ax.set_title('gyroscope data')

#remove grid
ax.grid(False)

#add arrows to show the direction of the axes
#get bigger values for the axes
x_max = df['X'].max()
y_max = df['Y'].max()
z_max = df['Z'].max()
#ax.quiver(0, 0, 0, x_max, 0, 0, color='black', arrow_length_ratio=0.1)
#ax.quiver(0, 0, 0, 0, y_max, 0, color='black', arrow_length_ratio=0.1)
#ax.quiver(0, 0, 0, 0, 0, z_max, color='black', arrow_length_ratio=0.1)


# show the plot
plt.show()
