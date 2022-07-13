# Io c
# 
# In this lesson you will learn about some of the AVR IO 
# functionality. This will enable you to read and write to Flash, EEPROM
# memories, as well as read and write to serial devices such 
# as the host tools you are designing during this course. 
# A series of challenges follow which will require you to 
# read C documentation as well as other online resources. 
# Good luck!
# 
# Note: These challenges are similar to the IO Python
# challenges. You are implementing the devices which you
# were "previously speaking" to.### Challenge Name: echo_int (/embsec/io_c/echo_int)
# 
# 
#         1. Read a big-endian short from the host tool
#         2. Increment the integer by 1
#         3. Send the integer as a big-endian short back to the host tool
#     
#     Resources:
#     
#     <https://en.wikipedia.org/wiki/Endianness>
#     
#     <https://en.wikipedia.org/wiki/Integer_%28computer_science%29>
#     
#     <https://docs.python.org/3/library/struct.html>
#     
#     
# 
import embsec
import subprocess
from core.util import extract_flag

def echo_int():
    subprocess.check_output([f'gcc -I../../lib/uart -I../../lib/stellaris/bearssl -I../../lib/BearSSL/inc echo_int.c ../../lib/stellaris/bearssl/beaverssl.c ../../lib/uart/uart_linux.c -o echo_int ../../lib/BearSSL/build/libbearssl.a'], shell=True)
    stdout, stdin = embsec.grade_c(f'./echo_int', f'/embsec/io_c/echo_int')
    
    return (extract_flag(stdout))
    
echo_int()

### Challenge Name: recv_file (/embsec/io_c/recv_file)
# 
# 
# 
#         This challenge is the "grader" or "C" side of the send_file challenge in "io_python," so you will be reading in
#         from the UART. 
# 
#         1. Receive the little endian short representing the size of the incoming file from UART2
#         2. Read in the file based on the size
#         3. Send back the file to UART2
# 
#         The serial device will contain a little-endian short indicating the size of the
#         incoming data and then size bytes of data. The format is represented below:
# 
#         [ 0x02 ]  [ variable ]
#         ---------------------
#         | Length |  Data... |
#         ---------------------
# 
#     Resources:
# 
#     <https://en.wikipedia.org/wiki/Endianness>
# 
#     <https://en.wikipedia.org/wiki/Integer_%28computer_science%29>
# 
#     <https://docs.python.org/3/library/struct.html>
# 
#     <https://docs.python.org/3/tutorial/inputoutput.html>
#     
# 
import embsec
import subprocess
from core.util import extract_flag

def recv_file():
    subprocess.check_output([f'gcc -I../../lib/uart -I../../lib/stellaris/bearssl -I../../lib/BearSSL/inc recv_file.c ../../lib/stellaris/bearssl/beaverssl.c ../../lib/uart/uart_linux.c -o recv_file ../../lib/BearSSL/build/libbearssl.a'], shell=True)
    stdout, stdin = embsec.grade_c(f'./recv_file', f'/embsec/io_c/recv_file')
    
    return (extract_flag(stdout))
    
recv_file()

### Challenge Name: recv_large_file (/embsec/io_c/recv_large_file)
# 
# 
#         1. Repeatedly read in frames by reading the length followed by the data from UART2
#         2. Terminate once a zero-length frame is received
#         3. Locate the flag within the entire file (Will start with 'embsec{')
#         4. Send the entire flag back over UART2
# 
#         The serial device contains a frames that begin with a little-endian short
#         indicating the size of the frame and then frame data. The maximum frame
#         size is 16 bytes. The frame format is represented below:
# 
#         [ 0x02 ]  [ up to 0xE bytes ]
#         ----------------------------
#         | Length |      Data...    |
#         ----------------------------
# 
#     Resources:
#     <https://en.wikipedia.org/wiki/Endianness>
#     <https://en.wikipedia.org/wiki/Integer_%28computer_science%29>
#     <https://docs.python.org/3/library/struct.html>
#     <https://docs.python.org/3/tutorial/inputoutput.html>
#     
# 
import embsec
import subprocess
from core.util import extract_flag

def recv_large_file():
    subprocess.check_output([f'gcc -I../../lib/uart -I../../lib/stellaris/bearssl -I../../lib/BearSSL/inc recv_large_file.c ../../lib/stellaris/bearssl/beaverssl.c ../../lib/uart/uart_linux.c -o recv_large_file ../../lib/BearSSL/build/libbearssl.a'], shell=True)
    stdout, stdin = embsec.grade_c(f'./recv_large_file', f'/embsec/io_c/recv_large_file')
    
    return (extract_flag(stdout))
    
recv_large_file()

