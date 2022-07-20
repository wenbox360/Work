
        #include "uart.h"
        #include "beaverssl.h"
        #include "beaverssl"

        #include <stdlib.h>
        #include <string.h>

        #define IV_LEN 0x10
        #define KEY_LEN 0x10
        #define KEY "thistheembseckey"

    //1.Use the AES key defined in the C file
    //2.Read the 16 byte initialization vector (IV) from the serial device on UART2
    //3.Read the ciphertext size (formatted as a little-endian short) from the serial device on UART2
    //4.Read the ciphertext from the serial device on UART2
    //5.Decrypt the ciphertext using 128-bit AES in CBC mode

        int main() {
            uart_init(UART2);
            int r;
            

            // TODO...
            char initv[16];
            for(int i =0; i<16; i++){
            initv[i]=uart_read(UART2,BLOCKING, &ret);
            }
            
            short size = uart_read(UART2,BLOCKING,&ret) | (uart_read(UART2, BLOCKING, &ret)<<8);
            char ciphertext[size];
            
            rcv = uart_read(UART1, BLOCKING, &read);
            size = (uint32_t) rcv;
            rcv = uart_read(UART1, BLOCKING, &read);
            size |= (uint32_t) rcv << 8;
  

            uart_write_str(UART2, "Received Firmware Size: ");
            uart_write_hex(UART2, size);
            nl(UART2);

            int aes_decrypt(char* key, char* iv, char* data, int len);
            

            return 0;
        }
        