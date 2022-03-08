ifndef KEYSTONE_RUNTIME_DIR
  $(error KEYSTONE_RUNTIME_DIR is undefined)
endif

CC = riscv64-unknown-linux-gnu-gcc
CFLAGS = -I$(KEYSTONE_RUNTIME_DIR) -I$(KEYSTONE_RUNTIME_DIR)/tmplib -Wall -Werror -fPIC -fno-builtin
SRCS = driver.c
DEV_DRV = trusted-dev-drv-example
LINK = riscv64-unknown-linux-gnu-ld
LDFLAGS = -static -nostdlib
DRV_LDS ?= drv.lds

OBJS = $(patsubst %.c,%.o,$(SRCS))

all: $(DEV_DRV) $(OBJS)


$(DEV_DRV): $(OBJS) 
	$(LINK) $(LINKFLAGS) -o $@ $^ $(KEYSTONE_RUNTIME_DIR)/string.o $(KEYSTONE_RUNTIME_DIR)/printf.o $(KEYSTONE_RUNTIME_DIR)/uaccess.o -T $(DRV_LDS)


%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(DEV_FW) *.o
