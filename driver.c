#include "sbi.h"
#include "eapp_utils.h"
#include "printf.h"
#include "uaccess.h"

long EAPP_ENTRY read(unsigned long fd, unsigned long a1, unsigned long a2)
{
  printf("[trusted device driver] read ops test\r\n"); 
  return 0;
}

long write(unsigned long fd, unsigned long a1, unsigned long a2)
{
  printf("[trusted device driver] write ops test\r\n"); 
  return 0;
}

long ioctl(unsigned long fd, unsigned long cmd, unsigned long arg)
{
  printf("[trusted device driver] ioctl ops test\r\n"); 
  return 0;
}

long release(unsigned long fd, unsigned long a1, unsigned long a2)
{
  printf("[trusted device driver] release ops test\r\n"); 
  return 0;
}
