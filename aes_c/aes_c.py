# Aes c
# 
#  In this lesson you will learn how to use the C library
# BearSSL to encrypt and decrypt with the AES algorithm. This
# lesson requires an understanding of C IO, byte manipulation,
# and endianess. Good luck!
# ### Challenge Name: aes_decrypt (/embsec/aes_c/aes_decrypt)
# 
# 
#     The host tool is sending you an encrypted messaged. The message 
#     is in the following format:
#        
#     [ 0x10 ] [      0x2      ] [ variable...]
#     ----------------------------------------
#     |  IV   | Ciphertext Size | Ciphertext |
#     ----------------------------------------
#     
#     Write C code to perform the following sequence. A C file has been provided for you,
#     called 'aes_decrypt.c', which includes beaverssl for your crypto functions!
#     
#     1. Use the AES key defined in the C file
#     2. Read the 16 byte initialization vector (IV) from the serial device on UART2
#     3. Read the ciphertext size (formatted as a little-endian short) from the serial device on UART2
#     4. Read the ciphertext from the serial device on UART2
#     5. Decrypt the ciphertext using 128-bit AES in CBC mode
# 
#     The flag will be in this decrypted text! Send it over UART2, terminated with a newline.
#     HINT: use strstr to find the 'embsec...' stem of the flag!
# 
# 
# 

subprocess.check_output([f'gcc  -I../../lib/uart -I../../lib/stellaris/bearssl -I../../lib/BearSSL/inc {name}.c ../../lib/stellaris/bearssl/beaverssl.c ../../lib/uart/uart_linux.c -o {name} ../../lib/BearSSL/build/libbearssl.a'], shell=True)
stdout, stdin = embsec.grade_c(f'./{name}', f'/embsec/aes_c/{name}')

return extract_flag(stdin)
### Challenge Name: aes_encrypted_echo (/embsec/aes_c/aes_encrypted_echo)
# 
# 
#     The host tool is sending you an encrypted message. Decrypt
#     the message and send it back encrypted with a different IV. The 
#     message needs to be formatted as follows:
#     
#     [ 0x10 ] [      0x2      ] [ variable...]
#     ----------------------------------------
#     |  IV   | Ciphertext Size | Ciphertext |
#     ----------------------------------------
#     
#     1. Use the AES key KEY in the starting C file
#     2. Read the 16 byte initialization vector (IV) from the serial device on UART2
#     3. Read the ciphertext size (formatted as a little-endian short) from the serial device
#     4. Read the ciphertext from the serial device
#     5. Decrypt the ciphertext using 128-bit AES in CBC mode
#     6. Send the new IV NEW_IV to the serial device
#     7. Re-encrypt the plaintext with the same key but a new IV
#     8. Send the ciphertext size (formatted as a little-endian short) to the serial device
#     9. Send the ciphertext to the serial device
#     10. Read the response
# 
# 
# 
import embsec
import subprocess
from core.util import extract_flag

def aes_encrypted_echo():
    subprocess.check_output([f'gcc -I../../lib/uart -I../../lib/stellaris/bearssl -I../../lib/BearSSL/inc aes_encrypted_echo.c ../../lib/stellaris/bearssl/beaverssl.c ../../lib/uart/uart_linux.c -o aes_encrypted_echo ../../lib/BearSSL/build/libbearssl.a'], shell=True)
    stdout, stdin = embsec.grade_c(f'./aes_encrypted_echo', f'/embsec/aes_c/aes_encrypted_echo')
    
    return (extract_flag(stdout))
    
aes_encrypted_echo()

