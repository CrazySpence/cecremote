# cecremote
a basic utility for handling navigation of a UI with a CEC capable TV remote

-testkey.c

--can currently detect up down left right, select/enter, quit/exit and 0

--With those inputs it will send KEY_UP,KEY_DOWN, KEY_LEFT,KEY_RIGHT, KEY_ENTER, KEY_ESC and KEY_0

Limitations

-Currently makes use of cecclient which must be installed to /usr/bin

-Currenly shows up as "CECTester"

I also included my original files which used a bash script and simpler C file for reference/educational purposes
