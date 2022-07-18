# Protect and update
# 
# 
# This lesson is intended to prepare you for the design challenge by having you write
# the host-side (python) tools that communicate with the embedded bootloader. When you
# begin the actual design challenge we will provide you with a complete set of tools, but
# by implementing them yourself first (in this lesson) you will be able to understand how
# they work at a low level. Practice is the best way to learn!
# ### Challenge Name: fw_protect (/embsec/protect_and_update/fw_protect)
# 
# The purpose of this tool will be to secure the firmware. Eventually,
# you will want to keep the firmware confidential and be able to verify its integrity and authenticity. But for now, 
# you will implement a firmware protect tool that does not add security. This tool should simply:
# 
#     1. Pack the version "0" into a little-endian short
#     2. Load the firmware binary from firmware.bin
#     3. Pack the size of the firmware into a little-endian short
#     4. Append the release message "First version of firmware." to end of firmware (make sure it is null-terminated)
#     5. Build and send a binary blob in the following format to the grader:
#     
#         [ 0x2 ]      [ 0x2 ]      [ variable ]           [ variable ]
#         -------------------------------------------------------------------------
#         version | firmware size |   firmware   |   release message + null-byte   
# 
# 
# 
from embsec import Serial

def fw_protect():
    ser = Serial("/embsec/protect_and_update/fw_protect")
    # Your code goes here!

fw_protect()
### Challenge Name: fw_update (/embsec/protect_and_update/fw_update)
# 
# For this lesson you will implement a basic update tool for the design challenge.
# The tool will be used to update the device firmware. You should focus on reading in the firmware blob file, 
# and constructing a set of frames to send to the bootloader. Pay attention to the structure of these 
# frames, as the bootloader is expecting a certain size and format.
# 
# The bootloader is expecting frames of the following format:
# 
#       [ 0x2 ]    [ variable ]
#     ----------------------------
#     frame size |   data... 
# 
# Process:
# 
# 1. Read in binary blob
# 2. Construct frames from blob
# 3. Send a 'U' for update mode & receive a confirmation 'U'
# 4. Send frames sequentially to boot loader
# 
# 
from embsec import Serial

def fw_update():
    ser = Serial("/embsec/protect_and_update/fw_update")
    # Your code goes here!

fw_update()
