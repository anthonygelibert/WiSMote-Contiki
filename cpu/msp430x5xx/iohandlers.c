#include <stdio.h>
#include <signal.h>

#include "contiki-conf.h"
#include "iohandlers.h"
#include "sys/energest.h"
#include "dev/watchdog.h"

/** Enable IOHandlers debug. */
#define DEBUG_IOHANDLERS 0

/**
 * Do nothing.
 */
void
dummy_handler(void)
{
#if DEBUG_IOHANDLERS
  printf("I'm the dummy handler.\n");
#endif /* DEBUG_IOHANDLERS */
}

/**
 * The IT handlers.
 */
static ITHandler handlers[17][2] = { { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler },
                                      { dummy_handler, dummy_handler } };

interrupt(PORT1_VECTOR)
irq_p1(void)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);
  watchdog_start();

#if DEBUG_IOHANDLERS
  printf("Port vector 1\n");
#endif

  handlers[P1IV][0]();

#if DEBUG_IOHANDLERS
  printf("End of port vector 1\n");
#endif

  watchdog_stop();
  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}

interrupt(PORT2_VECTOR)
irq_p2(void)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);
  watchdog_start();

#if DEBUG_IOHANDLERS
  printf("Port vector 2\n");
#endif

  handlers[P2IV][1]();

#if DEBUG_IOHANDLERS
  printf("End of port vector 2\n");
#endif

  watchdog_stop();
  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}

/**
 * NOT_YET_DOCUMENTED_PTV
 *
 * \param handler NOT_YET_DOCUMENTED_PTV
 * \param port NOT_YET_DOCUMENTED_PTV
 * \param bit NOT_YET_DOCUMENTED_PTV
 */
void
setHandler(const ITHandler handler, const uint8_t port, const uint8_t bit)
{
#if DEBUG_IOHANDLERS
  printf("I set handlers[%d][%d]\n", bit, port - 1);
#endif
  handlers[bit][port - 1] = handler;
}

/** NOT_YET_DOCUMENTED_PTV
 *
 * \param port NOT_YET_DOCUMENTED_PTV
 * \param bit NOT_YET_DOCUMENTED_PTV
 */
void
resetHandler(const uint8_t port, const uint8_t bit)
{
#if DEBUG_IOHANDLERS
  printf("I reset handlers[%d][%d]\n", bit, port - 1);
#endif
  handlers[bit][port - 1] = dummy_handler;
}
