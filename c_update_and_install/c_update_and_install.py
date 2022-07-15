# C update and install
# 
# 
# In this lesson we will be writing the C code for the bootloader that will receive firmware updates from
# the update tool (which you wrote in `protect_and_update`!). You will need to implement the update functionality
# as well as the install functionality!
# ### Challenge Name: c_update_short (/embsec/c_update_and_install/c_update_short)
# 
# 
# For this challenge you will implement a basic bootloader update routine for the design challenge.
# This routine will match the update tool which you wrote in a `protect_and_update`. For this impelementation you don't need
# to worry about validating or securing the bootloader. Here we will just focus on writing an 'insecure' bootloader.
# 
# Some of the code has been written for you. Look for the 'TODOs' and to write the function that reads frames from
# the update tool and writes them to flash!
# 
# You are familiar with the format of the binary blob, since you constructed in with fw_protect.
# The update tool will send frames of the following format:
# 
#       [ 0x2 ]    [ variable ]
#     ----------------------------
#     frame size |   data... 
# 
# Process:
# 
# 1. Wait until you receive a 'U' on UART1
# 2. Send a 'U' on UART1 to confirm
# 3. Receive frames and write them to flash
# 4. Stop when a zero-length frame is received
# 
# The code you have been given has a function that will run to check your work. If you wrote the code
# correctly, you'll get a flag!
# 
# In this challenge you will be send a *short* firmware, so you can quickly test your code (as the update
# process takes a while, and we want you to be able to get some fast feedback). The next challenge is the same
# as this one, but you will be sent a much longer firmware that will check your code for edge cases!
# 
# 
# 
import embsec
import subprocess
from core.util import extract_flag

def c_update_short():
    subprocess.check_output([f'(cd c_update_short && make CHALLENGE=c_update_short)'], shell=True)
    resp0, resp1, resp2 = embsec.grade_emulated(f'./c_update_short/gcc/main.bin', f'/embsec/c_update_and_install/c_update_short')
    stdout, stdin = resp2
    return (extract_flag(stdout))
    
c_update_short()

### Challenge Name: c_update_long (/embsec/c_update_and_install/c_update_long)
# 
# 
# This challenge is exactly the same as `c_update_short`, but you will be sent a *much* larger firmware during
# the update process! This will test your ability to write across flash pages ... spooky!
# 
# Copy in the C code you wrote in the previous challenge, and make edits as needed to fix any bugs you find!
# 
# If the previous challenge works but this one does not, the issue is probably related to writing across pages!
# If you wrote the code correctly, you'll get a flag!
# 
# 
import embsec
import subprocess
from core.util import extract_flag

def c_update_long():
    subprocess.check_output([f'(cd c_update_long && make CHALLENGE=c_update_long)'], shell=True)
    resp0, resp1, resp2 = embsec.grade_emulated(f'./c_update_long/gcc/main.bin', f'/embsec/c_update_and_install/c_update_long')
    stdout, stdin = resp2
    return (extract_flag(stdout))
    
c_update_long()

