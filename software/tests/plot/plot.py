import serial
import time
import matplotlib.pyplot as plt

# --- CONFIGURATION ---
COM_PORT = "COM3"  # Change this to your Nano's actual COM port
BAUD_RATE = 9600  # Must match the Serial.begin(9600) in your Arduino code
COLLECTION_TIME = 10.0  # Duration to record data in seconds

# --- DATA STORAGE ---
timestamps = []
voltages = []

print(f"Connecting to {COM_PORT} at {BAUD_RATE} baud...")

try:
    # Open serial port
    ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Allow Arduino Nano to reset after connecting

    print("\n>>> Connection successful! Recording numbers for 10 seconds...")

    start_time = time.time()

    while (time.time() - start_time) < COLLECTION_TIME:
        if ser.in_waiting > 0:
            try:
                # Read line, decode bytes, strip whitespace/newlines
                raw_line = ser.readline().decode("utf-8").strip()

                if raw_line:  # Make sure the line isn't empty
                    voltage_val = float(raw_line)
                    elapsed_time = time.time() - start_time

                    timestamps.append(elapsed_time)
                    voltages.append(voltage_val)

                    print(f"[{elapsed_time:.1f}s] Logged: {voltage_val}")

            except ValueError:
                # Ignores any partial or garbled lines during boot up
                pass

    ser.close()
    print("\n>>> 10 Seconds complete. Connection closed safely.")

except serial.SerialException:
    print(
        f"\n[Error] Could not open serial port {COM_PORT}. Is the Arduino Serial Monitor open?"
    )
    exit()

# --- PLOTTING ---
# --- PLOTTING ---
if len(voltages) > 0:
    print("Generating high-precision plot...")

    import matplotlib.ticker as ticker

    fig, ax = plt.subplots(figsize=(12, 6))

    # Plot the raw data points
    ax.plot(
        timestamps,
        voltages,
        color="blue",
        marker="o",
        linestyle="-",
        linewidth=1.5,
        label="Raw Readings",
    )

    # 1. HIGH-PRECISION Y-AXIS FORMATTING
    # Forces the Y-axis labels to always show exactly 2 decimal places (e.g., 12.05)
    ax.yaxis.set_major_formatter(ticker.FormatStrFormatter("%.2fV"))

    # 2. GRANULAR GRID INTERVALS
    # Automatically creates tight, precise spacing for labels on both axes
    ax.xaxis.set_major_locator(
        ticker.MaxNLocator(nbins=10)
    )  # ~10 neat steps over 10 seconds
    ax.yaxis.set_major_locator(
        ticker.MaxNLocator(nbins=15)
    )  # Dense horizontal steps to catch small jumps

    # 3. LABELS AND STYLING
    ax.set_title(
        "High-Precision Voltage Stability Analysis",
        fontsize=14,
        fontweight="bold",
        pad=15,
    )
    ax.set_xlabel("Elapsed Time (Seconds)", fontsize=11)
    ax.set_ylabel("Measured Voltage", fontsize=11)

    # Turn on both major and minor grid lines for micro-analysis
    ax.grid(True, which="both", linestyle="--", alpha=0.5, color="gray")

    # Tight layout ensures labels don't get cut off at the edges
    plt.tight_layout()
    plt.show()
else:
    print("\n[Warning] No data was captured to plot.")
