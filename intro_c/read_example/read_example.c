#include "uart.h"

int main(void)
{
  // Variable setup

  // Initialize serial
  uart_init(UART2);

  // Read characters from serial into a string until a newline is received
  char mybyte;
  int status;
  mybyte = uart_read(UART2, BLOCKING, &status);

  // Send the full string back over serial

  return 0;
}
