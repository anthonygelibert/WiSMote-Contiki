#ifndef __IOHANDLERS_H__
#define __IOHANDLERS_H__

#include "sys/cc.h"
#include "iohandlers.h"
#include <io.h>

#define HWCONF_PIN(name, port, bit)                                            \
static CC_INLINE void name##_SELECT()      {P##port##SEL &= ~BIT##bit;}        \
static CC_INLINE void name##_SELECT_IO()   {P##port##SEL &= ~BIT##bit;}        \
static CC_INLINE void name##_SELECT_PM()   {P##port##SEL |=  BIT##bit;}        \
static CC_INLINE void name##_SET()         {P##port##OUT |=  BIT##bit;}        \
static CC_INLINE void name##_CLEAR()       {P##port##OUT &= ~BIT##bit;}        \
static CC_INLINE int  name##_READ()        {return (P##port##IN & BIT##bit);}  \
static CC_INLINE void name##_MAKE_OUTPUT() {P##port##DIR |= BIT##bit;}         \
static CC_INLINE void name##_MAKE_INPUT()  {P##port##DIR &= ~BIT##bit;}

#define HWCONF_IRQ(name, port, bit, handler)                                   \
static CC_INLINE void name##_ENABLE_IRQ()       {P##port##IE |= BIT##bit;}     \
static CC_INLINE void name##_DISABLE_IRQ()      {P##port##IE &= ~BIT##bit;}    \
static CC_INLINE int  name##_IRQ_ENABLED()    {return P##port##IE & BIT##bit;} \
static CC_INLINE void name##_IRQ_EDGE_SELECTD() {P##port##IES |= BIT##bit;}    \
static CC_INLINE void name##_IRQ_EDGE_SELECTU() {P##port##IES &= ~BIT##bit;}   \
static CC_INLINE int  name##_CHECK_IRQ()      {return P##port##IFG & BIT##bit;}\
static CC_INLINE int  name##_IRQ_PORT()       {return port;}                   \
static CC_INLINE void name##_SET_HANDLER()      {setHandler(handler,port,P##port##IV_P##port##IFG##bit);}\
static CC_INLINE void name##_RESET_HANDLER()    {resetHandler(port,P##port##IV_P##port##IFG##bit);}

/** IT interruption handler. */
typedef void (*ITHandler)(void);

/**
 * NOT_YET_DOCUMENTED_PTV
 *
 * \param handler NOT_YET_DOCUMENTED_PTV
 * \param port NOT_YET_DOCUMENTED_PTV
 * \param bit NOT_YET_DOCUMENTED_PTV
 */
void setHandler(const ITHandler handler, const uint8_t port, const uint8_t bit);

/** NOT_YET_DOCUMENTED_PTV
 *
 * \param port NOT_YET_DOCUMENTED_PTV
 * \param bit NOT_YET_DOCUMENTED_PTV
 */
void resetHandler(const uint8_t port, const uint8_t bit);
#endif /* __IOHANDLERS_H__ */
