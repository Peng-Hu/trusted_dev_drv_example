#include "sbi.h"
#include "eapp_utils.h"
#include <asm-generic/ioctl.h>
#include "printf.h"
#include "uaccess.h"

#define CPU_RESET_OFFSET 0x20000
#define CPU_DATA_BASEADDR_OFFSET 0x30000
#define CPU_ENDFLAG_OFFSET 0x40000

struct rv32_cpu_testbench_info{
  uint64_t start_pa; //testbench在内存的开始地址
  size_t len; //长度
};

#define TRUSTED_FPGA_ACC_IOC_MAGIC  0xCC
//复位CPU
#define TRUSTED_FPGA_ACC_IOC_RESET_CPU \
  _IO(TRUSTED_FPGA_ACC_IOC_MAGIC, 0x00)
//释放复位
#define TRUSTED_FPGA_ACC_IOC_RELEASE_RESET \
  _IO(TRUSTED_FPGA_ACC_IOC_MAGIC, 0x01)
//设置访存开始基地址和长度
#define TRUSTED_FPGA_ACC_IOC_MEMDATA_SET \
  _IOW(TRUSTED_FPGA_ACC_IOC_MAGIC, 0x02, struct rv32_cpu_testbench_info)
//testbench运行结束
#define TRUSTED_FPGA_ACC_IOC_RESULT_POLL \
  _IOR(TRUSTED_FPGA_ACC_IOC_MAGIC, 0x03, uint32_t)

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
    long ret = 0;
    //printf("trusted device driver ioctl ...\r\n");
    switch(cmd){
      case TRUSTED_FPGA_ACC_IOC_RESET_CPU:
      {
        ret = SBI_CALL_3(SBI_KEYSTONE_SM, SBI_SM_DEV_REG_WRITEL, fd, CPU_RESET_OFFSET, 0);
        if(ret==0)
          printf("[trusted device driver] reset cpu successfully\r\n");
        else
          printf("[trusted device driver] failed to reset cpu\r\n");
        break;
      }
      case TRUSTED_FPGA_ACC_IOC_RELEASE_RESET:
      {
        ret = SBI_CALL_3(SBI_KEYSTONE_SM, SBI_SM_DEV_REG_WRITEL, fd, CPU_RESET_OFFSET, 1);
        if(ret==0)
          printf("[trusted device driver] release reset successfully\r\n");
        else
          printf("[trusted device driver] failed to release reset\r\n");
        break;
      }
      case TRUSTED_FPGA_ACC_IOC_MEMDATA_SET:
      {
        struct rv32_cpu_testbench_info testbench_info_t;
        copy_from_user(&testbench_info_t, (void*)arg, sizeof(struct rv32_cpu_testbench_info));
        //printf("trusted device driver has received memdata start_pa:0x%lx, len:0x%lx bytes\r\n",testbench_info_t.start_pa,testbench_info_t.len);
        uint32_t memdata_start_pa_low32 = testbench_info_t.start_pa;
        uint32_t memdata_start_pa_high32 = (testbench_info_t.start_pa>>32);
        ret = SBI_CALL_3(SBI_KEYSTONE_SM, SBI_SM_DEV_REG_WRITEL, fd, CPU_DATA_BASEADDR_OFFSET, memdata_start_pa_low32);
		    ret = SBI_CALL_3(SBI_KEYSTONE_SM, SBI_SM_DEV_REG_WRITEL, fd, CPU_DATA_BASEADDR_OFFSET+0x8, memdata_start_pa_high32);
        if(ret==0)
          printf("[trusted device driver] set testbench buffer start_pa successfully\r\n");
        else
          printf("[trusted device driver] failed to set testbench buffer start_pa\r\n");
		    break;
      }
      case TRUSTED_FPGA_ACC_IOC_RESULT_POLL:
      {
        printf("[trusted device driver] poll result\r\n");
        uint32_t result = -1U;
        result = SBI_CALL_2(SBI_KEYSTONE_SM, SBI_SM_DEV_REG_READL, fd, CPU_ENDFLAG_OFFSET);
        //printf("trusted device driver poll result is:0x%x\r\n",result);
        copy_to_user((void*) arg, &result, sizeof(uint32_t));
        break;
      }
      default:
      printf("[trusted device driver] unkownd cmd...\n");
      return -78;
  }
  return ret;
}

long release(unsigned long fd, unsigned long a1, unsigned long a2)
{
  printf("[trusted device driver] release ops test\r\n"); 
  return 0;
}
