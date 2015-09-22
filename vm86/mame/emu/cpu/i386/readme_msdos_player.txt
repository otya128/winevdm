Based on MAME 0.152.
Fixes in MAME 0.154 to 0.160 are applied.

i386_state are removed and all its members are changed to global variables.
All registers can be accessed directly without cpustate->.

cycle_table_rm/pm are changed from dynamic array to static array.
