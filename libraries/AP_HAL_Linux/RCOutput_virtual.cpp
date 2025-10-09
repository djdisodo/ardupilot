#include "RCOutput_virtual.h"

#include <AP_HAL/AP_HAL.h>
#include <AP_Math/AP_Math.h>

using namespace IOMCU2;

RCOutput_virtual::RCOutput_virtual() = default;

void RCOutput_virtual::init() {
    iomcu_b.init();
}

void RCOutput_virtual::set_freq(uint32_t chmask, uint16_t freq_hz) {

}

uint16_t RCOutput_virtual::get_freq(uint8_t chan) {
    return 50;
}


void RCOutput_virtual::enable_ch(uint8_t chan)
{}

void RCOutput_virtual::disable_ch(uint8_t chan)
{}

void RCOutput_virtual::write(uint8_t chan, uint16_t period_us)
{
    iomcu_b.rc_out.pwm_duration[chan] = period_us;
}
void RCOutput_virtual::cork() {

}

void RCOutput_virtual::push() {

}



uint16_t RCOutput_virtual::read(uint8_t chan)
{
    return iomcu_b.rc_out.pwm_duration[chan];
}

void RCOutput_virtual::read(uint16_t* period_us, uint8_t len)
{
    for (uint8_t i = 0; i < MIN(len, CHANNELS); i++) {
        period_us[i] = iomcu_b.rc_out.pwm_duration[i];
    }
}