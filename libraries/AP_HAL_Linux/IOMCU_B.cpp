//
// Created by PC on 25. 8. 23.
//

#include "IOMCU_B.h"

using namespace IOMCU2;

void IOMCU_B::init()  {
    spi_device = AP_HAL::get_HAL().spi->get_device(HAL_IOMCU_B);
    spi_device->set_speed(AP_HAL::Device::SPEED_HIGH);
    spi_device->register_periodic_callback(1000000UL / 2, FUNCTOR_BIND_MEMBER(&IOMCU_B::sync, void));
}

void IOMCU_B::sync() {
    if (rc_semaphore.take(1)) {

        SpiFrame frame {
            1,
            {},
            rc_serialized
        };
        spi_device->transfer_fullduplex((uint8_t *)&frame, sizeof(frame));
        rc_semaphore.give();
    }
}



IOMCU_B iomcu_b{};