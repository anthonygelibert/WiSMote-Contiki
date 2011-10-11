#ifndef __SHT1X_SENSOR_H__
#define __SHT1X_SENSOR_H__

#define SHT1X_SENSOR_TEMP              0
#define SHT1X_SENSOR_HUMIDITY          1
#define SHT1X_SENSOR_BATTERY_INDICATOR 2

/* adr   command  r/w */
#define  STATUS_REG_W   0x06    /* 000    0011    0 */
#define  STATUS_REG_R   0x07    /* 000    0011    1 */
#define  MEASURE_TEMP   0x03    /* 000    0001    1 */
#define  MEASURE_HUMI   0x05    /* 000    0010    1 */
#define  RESET          0x1e    /* 000    1111    0 */


#define BV(bit)   (1 << (bit))

#define EXPORT_SHT1X(var) extern const struct sensors_sensor var;

#define NEW_SHT1X(var, name, sda_port, sda_bit, scl_port, scl_bit, pwr_port, pwr_bit) \
    static void var##_sstart(void) {\
        P##sda_port##DIR &= ~BV(sda_bit); P##scl_port##OUT &= ~BV(scl_bit);\
        _NOP();_NOP(); _NOP();_NOP();\
        P##scl_port##OUT |=  BV(scl_bit);\
        _NOP();_NOP(); _NOP();_NOP();\
        P##sda_port##DIR |=  BV(sda_bit);\
        _NOP();_NOP(); _NOP();_NOP();\
        P##scl_port##OUT &= ~BV(scl_bit);\
        _NOP();_NOP(); _NOP();_NOP();\
        P##scl_port##OUT |=  BV(scl_bit);\
        _NOP();_NOP(); _NOP();_NOP();\
        P##sda_port##DIR &= ~BV(sda_bit);\
        _NOP();_NOP(); _NOP();_NOP();\
        P##scl_port##OUT &= ~BV(scl_bit);\
    }\
    \
    static void var##_sreset(void) {\
        int i;\
        P##sda_port##DIR &= ~BV(sda_bit);\
        P##scl_port##OUT &= ~BV(scl_bit);\
        for(i = 0; i < 9 ; i++) { \
            P##scl_port##OUT |= BV(scl_bit);\
            _NOP();_NOP(); _NOP();_NOP();\
            P##scl_port##OUT &= ~BV(scl_bit);\
        }\
        var##_sstart();\
    }\
    \
    static int var##_swrite(unsigned _c) {\
        int i, ret;\
        for(i = 0; i < 8; i++, _c <<= 1) {\
            if(_c & 0x80) {\
                P##sda_port##DIR &= ~BV(sda_bit);\
            } else {\
                P##sda_port##DIR |=  BV(sda_bit);\
            }\
            P##scl_port##OUT |=  BV(scl_bit);\
            _NOP();_NOP(); _NOP();_NOP();\
            P##scl_port##OUT &= ~BV(scl_bit);\
        }\
        P##sda_port##DIR &= ~BV(sda_bit);\
        P##scl_port##OUT |=  BV(scl_bit);\
        _NOP();_NOP(); _NOP();_NOP();\
        ret = !(P##sda_port##IN & BV(sda_bit));\
        P##scl_port##OUT &= ~BV(scl_bit);\
        return ret;\
    }\
    \
    static unsigned var##_sread(int send_ack) {\
      int i;\
      unsigned char c = 0x00;\
      P##sda_port##DIR &= ~BV(sda_bit);\
      for(i = 0; i < 8; i++) {\
          c <<= 1;\
          P##scl_port##OUT |=  BV(scl_bit);\
          _NOP();_NOP(); _NOP();_NOP();\
          if(P##sda_port##IN & BV(sda_bit)) {\
              c |= 0x1;\
          }\
          P##scl_port##OUT &= ~BV(scl_bit);\
      }\
      if(send_ack) {\
          P##sda_port##DIR |=  BV(sda_bit);\
      }\
      P##scl_port##OUT |=  BV(scl_bit);\
      _NOP();_NOP(); _NOP();_NOP();\
      P##scl_port##OUT &= ~BV(scl_bit);\
      P##sda_port##DIR &= ~BV(sda_bit);\
      return c;\
    }\
    \
    void var##_init(void) { \
        P##pwr_port##DS  |= BV(pwr_bit);\
        P##pwr_port##OUT |= BV(pwr_bit);\
        P##sda_port##DS  |= BV(sda_bit);\
        P##sda_port##OUT &= ~BV(sda_bit);\
        P##scl_port##DS  |= BV(scl_bit);\
        P##scl_port##OUT &= ~BV(scl_bit);\
        P##pwr_port##DIR |= BV(pwr_bit);\
        P##scl_port##DIR |= BV(scl_bit);\
    }\
    \
    void var##_off(void) {\
        P##pwr_port##OUT &= ~BV(pwr_bit);\
        P##sda_port##OUT &= ~BV(sda_bit);\
        P##scl_port##OUT &= ~BV(scl_bit);\
        P##pwr_port##DIR &= ~BV(pwr_bit);\
        P##scl_port##DIR |= BV(scl_bit);\
    }\
    \
    static unsigned int var##_scmd(unsigned cmd) {\
        unsigned long n;\
        if(cmd != MEASURE_HUMI && cmd != MEASURE_TEMP) {\
            return -1;\
        }\
        dint();\
        var##_sstart();\
        if(!var##_swrite(cmd)) {\
            goto fail;\
        }\
        for(n = 0; n < 250000; n++) {\
            if(!(P##sda_port##IN & BV(sda_bit))) {\
                unsigned t0, t1, rcrc;\
                t0 = var##_sread(1);\
                t1 = var##_sread(1);\
                rcrc = var##_sread(0);\
                eint();\
                return (t0 << 8) | t1;\
            }\
        }\
    fail:\
        var##_sreset();\
        eint();\
        return -1;\
    }\
    \
    unsigned int var##_temp(void) {\
        return var##_scmd(MEASURE_TEMP);\
    }\
    \
    unsigned int var##_humidity(void) {\
        return var##_scmd(MEASURE_HUMI);\
    }\
    \
   unsigned var##_sreg(void) {\
       unsigned sreg, rcrc;\
       dint();\
       var##_sstart();\
       if(!var##_swrite(STATUS_REG_R)) {\
           goto fail;\
       }\
       sreg = var##_sread(1);\
       rcrc = var##_sread(0);\
       eint();\
       return sreg;\
   fail:\
      var##_sreset();\
      eint();\
      return -1;\
   }\
\
   const struct sensors_sensor var##_sensor;\
\
   static uint8_t var##_state = 0;\
\
   static int var##_value(int type) {\
        int res;\
        switch(type) {\
           case SHT1X_SENSOR_TEMP: \
             return var##_temp();\
           case SHT1X_SENSOR_HUMIDITY: \
             return var##_humidity();\
           case SHT1X_SENSOR_BATTERY_INDICATOR: \
             res = var##_sreg();\
             return res == -1 ? -1 : ((res & 0x40) >> 6);\
       }\
       return 0;\
   }\
\
    static int var##_status(int type) {\
        switch(type) {\
            case SENSORS_ACTIVE:\
            case SENSORS_READY: return (var##_state == 1);\
        }\
        return 0;\
     }\
\
    static int var##_configure(int type, int c) {\
        switch(type) {\
            case SENSORS_ACTIVE:\
            if(c) {\
                if(!var##_status(SENSORS_ACTIVE)) {\
                    rtimer_clock_t t0;\
                    var##_init();\
                    var##_state = 1;\
                    t0 = RTIMER_NOW(); while(RTIMER_CLOCK_LT(RTIMER_NOW(), t0 + RTIMER_SECOND / 100));\
                }\
            } else {\
                var##_off();\
                var##_state = 0;\
            }\
        }\
        return 0;\
     }\
\
  SENSORS_SENSOR(var, name, var##_value, var##_configure, var##_status);

#endif /* __SHT1X_SENSOR_H__ */
