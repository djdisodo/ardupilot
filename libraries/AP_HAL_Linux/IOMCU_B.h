#pragma once
#include <hwdef.h>
#ifdef HAL_IOMCU_B

//
// Created by PC on 25. 8. 23.
//
#include <algorithm>
#include <array>
#include <AP_HAL/AP_HAL.h>
#include <AP_HAL/SPIDevice.h>

#define CHANNELS 16


//TODO assert little-endian

#define WRITE(cur, data, len) memcpy(cur, data, len), cur += len

#define WRITE_ARR(cur, arr) WRITE(cur, arr, sizeof(arr))

#define WRITE_VAR(cur, var) WRITE(cur, &var, sizeof(var))


namespace IOMCU2 {

class IOMCU_B {
public:

    struct Packed12Pair {
        uint8_t data[3];
        Packed12Pair() = default;
        Packed12Pair(uint16_t high, uint16_t low) {
            this->set_pair(high, low);
        }
        void set_pair(uint16_t high_val, uint16_t low_val) {
            low_val  = std::min<uint16_t>(low_val,  0x0FFF);
            high_val = std::min<uint16_t>(high_val, 0x0FFF);

            uint32_t full = (static_cast<uint32_t>(high_val) << 12) | low_val;
            memcpy(data, &full, 3);
        }

        // Get both values
        void get_pair(uint16_t &high_val, uint16_t &low_val) const {
            uint32_t full = 0;
            memcpy(&full, data, 3);
            low_val  =  full        & 0x0FFF;
            high_val = (full >> 12) & 0x0FFF;
        }
    };
    struct RCSerialized {
        uint32_t enabled_mask;
        Packed12Pair channels[8];
    };

    struct SpiFrame {
        uint8_t version;
        uint8_t reserved[3];
        RCSerialized serialized;
    };

    RCSerialized rc_serialized;

    HAL_Semaphore rc_semaphore;

    struct Analog {
        uint16_t analog_raw[4];
    };
    HAL_Semaphore analog_semaphore;

    void init();

    void sync();

private:
    AP_HAL::OwnPtr<AP_HAL::SPIDevice> spi_device;
};

} // IOMCU2

extern IOMCU2::IOMCU_B iomcu_b;

#endif