#include "ecm_bridge/driverCmd.h"
#include "ecm_bridge/driverState.h"
#include <thread>
#define EC_PACKED(Bytes) __attribute__((aligned(Bytes), packed))
#define dNum 4
#define ENCODER_17BIT 20860.76
typedef struct
{
    uint8_t enable;
    uint8_t opMode;
    double kp;
    double kd;
    double q;
    double qd;
    double cur;
} EC_PACKED(1) driver_cmd;

typedef struct
{
    uint16_t sw;
    uint16_t error;
    uint8_t opMode;
    uint8_t complement;
    double q;
    double qd;
    double cur;
    double dc_v;
} EC_PACKED(1) driver_state;