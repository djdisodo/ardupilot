#pragma once
#include <hwdef.h>
#ifdef HAL_IOMCU_B

//
// Created by PC on 25. 8. 23.
//
#include <AP_HAL/AP_HAL.h>
#include <AP_HAL/SPIDevice.h>

#define CHANNELS 32


//TODO assert little-endian

#define WRITE(cur, data, len) memcpy(cur, data, len), cur += len

#define WRITE_ARR(cur, arr) WRITE(cur, arr, sizeof(arr))

#define WRITE_VAR(cur, var) WRITE(cur, &var, sizeof(var))



namespace IOMCU2 {

class IOMCU_B {
public:
    // RC Output
    struct RcOut {
        uint32_t enable_mask;
        uint16_t pwm_duration[CHANNELS];
    } rc_out = {};
    void init();

    void sync();
private:
    AP_HAL::OwnPtr<AP_HAL::SPIDevice> spi_device;
};

} // IOMCU2

extern IOMCU2::IOMCU_B iomcu_b;

#endif