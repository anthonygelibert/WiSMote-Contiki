#include "uart0.h"
#include "dev/slip.h"

/** NOT_YET_DOCUMENTED_PTV */
void
slip_arch_writeb(unsigned char c)
{
  uart0_writeb(c);
}

/**
 * Initialize the RS232 port and the SLIP driver.
 *
 * \param ubr NOT_YET_DOCUMENTED_PTV
 */
void
slip_arch_init(unsigned long ubr)
{
  /* XXX_PTV: Support ubr */
  uart0_set_input(slip_input_byte);
}

