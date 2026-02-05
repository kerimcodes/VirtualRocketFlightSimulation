import pandas as pd
import matplotlib.pyplot as plt

path = "C:\\software\\C_Python_Projects\\VirtualFlightSimulation\\data\\sensor.csv"

try:
    df = pd.read_csv(path)
except FileNotFoundError:
    print("Error: File not found! Please check the path.")
    exit()

fig, axs = plt.subplots(3, 3, figsize=(16, 10), sharex=True)

# --- X AXIS ---
axs[0, 0].plot(df['Time'], df['Ax'], color='lightblue', alpha=0.5, label="Raw (Noisy)")
axs[0, 0].plot(df['Time'], df['FilteredAx'], color='blue', linewidth=2, label="Filtered (Clean)")
axs[0, 0].set_title('X Axis Analysis')
axs[0, 0].set_ylabel('Acceleration ($m/s^2$)')
axs[0, 0].legend(loc="upper right")
axs[0, 0].grid(True, linestyle='--', alpha=0.6)

axs[1, 0].plot(df['Time'], df['Vx'], color='blue', linestyle='--')
axs[1, 0].set_ylabel('Velocity ($m/s$)')
axs[1, 0].grid(True, linestyle='--', alpha=0.6)

axs[2, 0].plot(df['Time'], df['Px'], color='blue', linewidth=2)
axs[2, 0].set_ylabel('Position ($m$)')
axs[2, 0].grid(True, linestyle='--', alpha=0.6)


# --- Y AXIS (Green Theme) ---
axs[0, 1].plot(df['Time'], df['Ay'], color='lightgreen', alpha=0.5, label="Raw")
axs[0, 1].plot(df['Time'], df['FilteredAy'], color='green', linewidth=2, label="Filtered")
axs[0, 1].set_title('Y Axis Analysis')
axs[0, 1].legend(loc="upper right")
axs[0, 1].grid(True, linestyle='--', alpha=0.6)

axs[1, 1].plot(df['Time'], df['Vy'], color='green', linestyle='--')
axs[1, 1].grid(True, linestyle='--', alpha=0.6)

axs[2, 1].plot(df['Time'], df['Py'], color='green', linewidth=2)
axs[2, 1].grid(True, linestyle='--', alpha=0.6)


# --- Z AXIS ---
axs[0, 2].plot(df['Time'], df['Az'], color='salmon', alpha=0.5, label="Raw")
axs[0, 2].plot(df['Time'], df['FilteredAz'], color='red', linewidth=2, label="Filtered")
axs[0, 2].set_title('Z Axis Analysis (Altitude)')
axs[0, 2].legend(loc="upper right")
axs[0, 2].grid(True, linestyle='--', alpha=0.6)

axs[1, 2].plot(df['Time'], df['Vz'], color='red', linestyle='--')
axs[1, 2].grid(True, linestyle='--', alpha=0.6)

axs[2, 2].plot(df['Time'], df['Pz'], color='black', alpha=0.3, linewidth=1, label="Trajectory")

# 0: IDLE, 1: ASCENT, 2: DESCENT, 3: LANDED
colors = {0: 'gray', 1: 'blue', 2: 'red', 3: 'green'}
labels = {0: 'IDLE', 1: 'ASCENT', 2: 'DESCENT', 3: 'LANDED'}

# Loop through each state to plot colored dots
for state_id, color_name in colors.items():
    mask = df['State'] == state_id
    if mask.any():
        axs[2, 2].scatter(df.loc[mask, 'Time'], df.loc[mask, 'Pz'], 
                          color=color_name, s=15, label=labels[state_id], zorder=5)

axs[2, 2].set_xlabel('Time (s)')
axs[2, 2].legend(loc="upper left")
axs[2, 2].grid(True, linestyle='--', alpha=0.6)
axs[2, 2].set_title("Altitude & Flight States")

plt.tight_layout()
plt.savefig(r"C:\software\C_Python_Projects\VirtualFlightSimulation\images\flight_analysis.png")
plt.show()