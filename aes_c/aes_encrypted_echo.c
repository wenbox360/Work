
        #include "uart.h"
        #include "beaverssl.h"
        #include <stdlib.h>
        #include <string.h>

        #define IV_LEN 0x10
        #define KEY_LEN 0x10
        #define KEY "thistheembseckey"
        #define NEW_IV "thisisanewnextiv"

        int main() {
            uart_init(UART2);
            int ret;

            char * key = KEY;
            unsigned char iv[IV_LEN];

            // TODO read a 16 byte IV from UART2
            

            // TODO read the ciphertext size (little endian short) from UART2, and allocate space for the ciphertext
            
            
            // TODO read the ciphertext from the serial device
            

            // Set up a new IV
            char new_iv[IV_LEN];
            memcpy(new_iv, "thisisanewnextiv", IV_LEN);

            // TODO decrypt the ciphertext
            

            // TODO send the IV over serial
            

            // TODO re-encrypt the data with the new IV
            

            // TODO send the ciphertext size (as a little endian short)
            

            // TODO send the ciphertext
            

            // Gets the flag for you
            char flag[256];

            for(int i = 0; i < 256; i++) {
                flag[i] = uart_read(UART2, BLOCKING, &ret);
                if (flag[i] == '\n')
                    break;
            }
            return 0;
        }
        