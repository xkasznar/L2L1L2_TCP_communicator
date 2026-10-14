Requirements: Linux OS, GNU Radio Companion, Python 3

Warning: Apply Message Hotfix/Enter Message Code Fault 
Occasionally, when the user applies message hotfix to conclude all C terminal client 
operations (AUTO_MSG_HOTFIX==0) or enters message to conclude all C terminal client 
operations (AUTO_MSG_HOTFIX==1), the QT GUI window will not manually close, the C 
terminal client will close and the GNU Radio pass itself will continue to be unresponsive to any 
relaunched client input and the pass will need to be manually closed and after that the pass is to 
be restarted to continue normal functionality.

Open three instances of Terminal.
a. In second instance of Terminal: type „./term_server 1 30“
b. In third instance: type „python3 gr_pass.py“
c. In first instance of Terminal: type „./term_client 0 30“
d. Answer C terminal client‘s prompt for a message to relay: e.g.: „Hello world“
e. View bits of the message - both those that have passed through the model (blue Passed Physical Bits in top right corner) and those that haven’t (green Skipped Physical Bits in top right corner) in QT GUI window of GNU Radio Companion.
f. Press Return key in C terminal client and the message will appear in C terminal server.
Note: when pressing Return key, the QT GUI window will sometimes need to be closed manually after step f and then function may be resumed from step b. See section 6.1 for more information.
g. Go to step b.
