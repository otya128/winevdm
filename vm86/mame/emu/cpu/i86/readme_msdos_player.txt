Based on MAME 0.149.

i8086_state/i80286_state are removed and all their members are changed to
global variables.
All registers can be accessed directly without cpustate->.

Timing codes are removed.
