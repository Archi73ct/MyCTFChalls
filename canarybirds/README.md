# Description
Canarybirds <3
- Canarybirds, they sing when bad stuff is brewing
- `nc canarybirds.hkn 1024`

# Intended solution
By reversing the challenge one can find the `getflag()` function.
This function will open and print the flag, so the goal is to redirect execution to this function.
The program asks for the lenth of the name of the user, as well as the name itself.
This is then printed. But one can realize that the length entered can be way longer than the buffer
containing the username. By using this leak, the user can leak the stack cookie, and gain IP control
using the buffer overflow in the following message.

`POC.py` is the solution.