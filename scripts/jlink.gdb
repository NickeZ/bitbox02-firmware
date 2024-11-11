# Connect to jlink gdb server
target extended-remote :2331

# load the firmware into ROM
load

# Reset the CPU
monitor reset

# start running
continue
