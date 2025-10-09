//
// Created by PC on 25. 8. 23.
//

#include "IOMCU_B.h"

using namespace IOMCU2;

void IOMCU_B::init()  {
    spi_device = AP_HAL::get_HAL().spi->get_device(HAL_IOMCU_B);
}

void IOMCU_B::sync() {
    //should we have buffer preallocated?
    uint8_t buffer[128];
    uint8_t *cursor = buffer;
    // big type first, aligned anyway
    WRITE_VAR(cursor, rc_out.enable_mask);
    WRITE_ARR(cursor, rc_out.pwm_duration);
    spi_device->set_speed(AP_HAL::Device::SPEED_HIGH);
    spi_device->set_chip_select(true);
    spi_device->transfer_fullduplex(buffer, cursor - buffer);
    spi_device->set_chip_select(false);
}

IOMCU_B iomcu_b{};