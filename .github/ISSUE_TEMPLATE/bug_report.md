---
name: Bug report
about: Create a report to help us improve
title: ''
labels: ''
assignees: ''

---

Try the latest version first.
Go [here](https://ci.appveyor.com/project/otya128/winevdm) and click `Environment: THIS_BUILD_IS_RECOMMENDED__VCXPROJ_BUILD=1`-> `Artifacts 1`-> `zip` to download the latest version.


**Describe the bug**
A clear and concise description of what the bug is.

**Expected behavior**
A clear and concise description of what you expected to happen.

**Screenshots**
If applicable, add screenshots to help explain your problem.

**Environment (please complete the following information):**
 - OS: [e.g. Windows 10 1903]
 - Version [e.g. version: 1523]

**Additional context**
Add any other context about the problem here.


It is very helpful to provide a trace file if possible.
Please note that this file contains detailed information about the program.
**Procedure**
1. Run the command prompt.
2. Set the environment variable:
```cmd
set WINEDEBUG=+all,-snoop,-ldt,-fixup,-module,-global,-local,-disasm,-syslevel,-thunk
```
3.
```cmd
*old-executable-file* 2> trace.txt
```
4. Upload trace.txt here.
