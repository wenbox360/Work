// Hardware Imports
#include "inc/hw_memmap.h" // Peripheral Base Addresses
#include "inc/lm3s6965.h" // Peripheral Bit Masks and Registers
#include "inc/hw_types.h" // Boolean type
#include "inc/hw_ints.h" // Interrupt numbers

// Driver API Imports
#include "driverlib/flash.h" // FLASH API
#include "driverlib/sysctl.h" // System control API (clock/reset)
#include "driverlib/interrupt.h" // Interrupt API

// Application Imports
#include "uart.h"


// Forward Declarations
void load_initial_firmware(void);
void load_firmware(void);
void boot_firmware(void);
long program_flash(uint32_t, unsigned char*, unsigned int);
void verify_flash_contents(void);


// Firmware Constants
#define METADATA_BASE 0xFC00  // base address of version and firmware size in Flash
#define FW_BASE 0x10000  // base address of firmware in Flash


// FLASH Constants
#define FLASH_PAGESIZE 1024
#define FLASH_WRITESIZE 4


// Protocol Constants
#define OK    ((unsigned char)0x00)
#define ERROR ((unsigned char)0x01)
#define UPDATE ((unsigned char)'U')
#define BOOT ((unsigned char)'B')


// Firmware v2 is embedded in bootloader
extern int _binary_firmware_bin_start;
extern int _binary_firmware_bin_size;


// Device metadata
uint16_t *fw_version_address = (uint16_t *) METADATA_BASE;
uint16_t *fw_size_address = (uint16_t *) (METADATA_BASE + 2);
uint8_t *fw_release_message_address;

// Firmware Buffer
unsigned char data[FLASH_PAGESIZE];


int main(void) {

  // Initialize UART channels
  // 0: Reset
  // 1: Host Connection
  // 2: Debug
  uart_init(UART0);
  uart_init(UART1);
  uart_init(UART2);

  // Enable UART0 interrupt
  IntEnable(INT_UART0);
  IntMasterEnable();

  load_initial_firmware();

  uart_write_str(UART2, "Welcome to the BWSI Vehicle Update Service!\n");
  uart_write_str(UART2, "Send \"U\" to update, and \"B\" to run the firmware.\n");
  uart_write_str(UART2, "Writing 0x20 to UART0 will reset the device.\n");

  int resp;
  while (1){
    uint32_t instruction = uart_read(UART1, BLOCKING, &resp);
    if (instruction == UPDATE){
      uart_write_str(UART1, "U");
      load_firmware();  // This is what we'll be working on, see the TODOs!
      verify_flash_contents();
    } else {
      uart_write_str(UART2, "Unknown command!\n");
    }
  }
}


/*
 * Load initial firmware into flash
 */
void load_initial_firmware(void) {


  if (*((uint32_t*)(METADATA_BASE)) != 0xFFFFFFFF){
    /*
     * Default Flash startup state in QEMU is all zeros since it is
     * secretly a RAM region for emulation purposes. Only load initial
     * firmware when metadata page is all zeros. Do this by checking
     * 4 bytes at the half-way point, since the metadata page is filled
     * with 0xFF after an erase in this function (program_flash()).
     */
    return;
  }

  // Create buffers for saving the release message
  uint8_t temp_buf[FLASH_PAGESIZE];
  char initial_msg[] = "This is the initial release message.";
  uint16_t msg_len = strlen(initial_msg)+1;
  uint16_t rem_msg_bytes;
  
  // Get included initial firmware
  int size = (int)&_binary_firmware_bin_size;
  uint8_t *initial_data = (uint8_t *)&_binary_firmware_bin_start;
  
  // Set version 2 and install
  uint16_t version = 2;
  uint32_t metadata = (((uint16_t) size & 0xFFFF) << 16) | (version & 0xFFFF);
  program_flash(METADATA_BASE, (uint8_t*)(&metadata), 4);
  
  int i;
  
  for (i = 0; i < size / FLASH_PAGESIZE; i++){
       program_flash(FW_BASE + (i * FLASH_PAGESIZE), initial_data + (i * FLASH_PAGESIZE), FLASH_PAGESIZE);
  }
  
  /* At end of firmware. Since the last page may be incomplete, we copy the initial
   * release message into the unused space in the last page. If the firmware fully
   * uses the last page, the release message simply is written to a new page.
   */
  
  uint16_t rem_fw_bytes = size % FLASH_PAGESIZE;
  if (rem_fw_bytes == 0){
    // No firmware left. Just write the release message
    program_flash(FW_BASE + (i*FLASH_PAGESIZE), (uint8_t *)initial_msg, msg_len);
  } else {
    // Some firmware left. Determine how many bytes of release message can fit
    if (msg_len > (FLASH_PAGESIZE-rem_fw_bytes)) {
      rem_msg_bytes = msg_len - (FLASH_PAGESIZE-rem_fw_bytes);
    } else {
      rem_msg_bytes = 0;
    }
    
    // Copy rest of firmware
    memcpy(temp_buf, initial_data + (i*FLASH_PAGESIZE), rem_fw_bytes);
    // Copy what will fit of the release message
    memcpy(temp_buf+rem_fw_bytes, initial_msg, msg_len-rem_msg_bytes);
    // Program the final firmware and first part of the release message
    program_flash(FW_BASE + (i * FLASH_PAGESIZE), temp_buf, rem_fw_bytes+(msg_len-rem_msg_bytes));
    
    // If there are more bytes, program them directly from the release message string
    if (rem_msg_bytes > 0) {
      // Writing to a new page. Increment pointer
      i++;
      program_flash(FW_BASE + (i * FLASH_PAGESIZE), (uint8_t *)(initial_msg+(msg_len-rem_msg_bytes)), rem_msg_bytes);
    }
  }
  
  // Compute release message start address
  fw_release_message_address = (uint8_t*)(FW_BASE+size);
  
}



/*
 * Load the firmware into flash.
 */
void load_firmware(void)
{
  int frame_length = 0;
  int read = 0;
  uint32_t rcv = 0;
  
  uint32_t data_index = 0;
  uint32_t page_addr = FW_BASE;
  uint32_t version = 0;
  uint32_t size = 0;
  uint32_t received_fw_bytes = 0;


  // Get version.
  rcv = uart_read(UART1, BLOCKING, &read);
  version = (uint32_t)rcv;
  rcv = uart_read(UART1, BLOCKING, &read);
  version |= (uint32_t)rcv << 8;

  uart_write_str(UART2, "Received Firmware Version: ");
  uart_write_hex(UART2, version);
  nl(UART2);

  // TODO: Read the firmware size from the fw_update tool

  // Compare to old version and abort if older (note special case for version 0).
  uint16_t old_version = *fw_version_address;

  if (version != 0 && version < old_version) {
    uart_write(UART1, ERROR); // Reject the metadata.
    SysCtlReset(); // Reset device
    return;
  } else if (version == 0) {
    // If debug firmware, don't change version
    version = old_version;
  }

  // Write new firmware size and version to Flash
  // Create 32 bit word for flash programming, version is at lower address, size is at higher address
  uint32_t metadata = ((size & 0xFFFF) << 16) | (version & 0xFFFF);
  program_flash(METADATA_BASE, (uint8_t*)(&metadata), 4);

  uart_write(UART1, OK); // Acknowledge the metadata.

  // TODO: Load the firmware into flash memory at 0x10000

}


/*
 * Program a stream of bytes to the flash.
 * This function takes the starting address of a 1KB page, a pointer to the
 * data to write, and the number of bytes to write.
 *
 * This functions performs an erase of the specified flash page before writing
 * the data.
 */
long program_flash(uint32_t page_addr, unsigned char *data, unsigned int data_len)
{
  uint32_t word = 0;
  int ret;
  int i;

  // Erase next FLASH page
  FlashErase(page_addr);

  // Clear potentially unused bytes in last word
  // If data not a multiple of 4 (word size), program up to the last word
  // Then create temporary variable to create a full last word
  if (data_len % FLASH_WRITESIZE){
    // Get number of unused bytes
    int rem = data_len % FLASH_WRITESIZE;
    int num_full_bytes = data_len - rem;
    
    // Program up to the last word
    ret = FlashProgram((unsigned long *)data, page_addr, num_full_bytes);
    if (ret != 0) {
      return ret;
    }
    
    // Create last word variable -- fill unused with 0xFF
    for (i = 0; i < rem; i++) {
      word = (word >> 8) | (data[num_full_bytes+i] << 24); // Essentially a shift register from MSB->LSB
    }
    for (i = i; i < 4; i++){
      word = (word >> 8) | 0xFF000000;
    }
    
    // Program word
    return FlashProgram(&word, page_addr+num_full_bytes, 4);
  } else{
    // Write full buffer of 4-byte words
    return FlashProgram((unsigned long *)data, page_addr, data_len);
  }
}


/*
 * This will spot-check flash, and communicate w/ the grader, printing a flag if you
 * wrote load_firmware() correctly!
 */
void verify_flash_contents(void)
{
  // read the version and size from flash
  uint16_t rb_version = *fw_version_address;
  uint16_t rb_size = *fw_size_address;
  // calculate the number of pages
  uint16_t num_pages = (uint16_t) (((uint32_t) rb_size) / FLASH_PAGESIZE);  // we have 256KB flash, 1K pages, thus 256 max pages, so need 2 bytes unsigned
  if ( ( (uint32_t) rb_size) % FLASH_PAGESIZE)
    num_pages++;
  // send this information to the grader, make sure its correct
  // sending version little endian...
  uart_write(UART1, (uint8_t) rb_version);
  uart_write(UART1, (uint8_t) (rb_version >> 8));
  // sending size little endian...
  uart_write(UART1, (uint8_t) rb_size);
  uart_write(UART1, (uint8_t) (rb_size >> 8));
  // sending number of pages little endian...
  uart_write(UART1, (uint8_t) num_pages);
  uart_write(UART1, (uint8_t) (num_pages >> 8));
  // sending the third byte of every page ...
  uint16_t i;
  for (i = 0; i < num_pages; i++){
    uint8_t third_byte = * ((uint8_t *) (FW_BASE + (i * FLASH_PAGESIZE) + 2));
    uart_write(UART1, third_byte);
  }
  // read until we get a newline, display the response!
  uint8_t resp_buff[35];
  i = 0;
  int resp;
  do {
    resp_buff[i] = uart_read(UART1, BLOCKING, &resp);
    i++;
  }
  while (resp_buff[i-1] != '\n');
  resp_buff[i] = '\0';
  uart_write_str(UART1, "Got the following response from the grader:\n");
  uart_write_str(UART1, resp_buff);
}