#include <zephyr/init.h>
#include <zephyr/kernel.h>

static int board_init_msg(void)
{
    printk("Board Initialized\n");
    return 0;
}

/* Run before main() */
SYS_INIT(board_init_msg, APPLICATION, 0);
