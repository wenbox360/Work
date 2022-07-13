#include "uart.h"

int main(void)
{
  // Variable setup

  // Initialize serial
    uart_init(UART2);
    
    int ret;
    char flag[64]
    for (int i = 0; i <64; i++){
        flag[i]=0;
    }
    flag[0] = uart_read(UART2, BLOCKING, &status);
    
    int i = 0;

  // Read characters from serial into a string until a newline is received

    while(flag[i] != '\n') {
        i += 1;
        flag[i] = uart_read(UART2, BLOCKING, &status);
    }
    
  uart_read(UART2, BLOCKING, &status);

  // Send the full string back over serial

  return 0;
}
