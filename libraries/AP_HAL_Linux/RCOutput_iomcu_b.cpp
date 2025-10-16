#include "RCOutput_iomcu_b.h"

#include <AP_HAL/AP_HAL.h>
#include <AP_Math/AP_Math.h>

using namespace IOMCU2;

RCOutput_iomcu_b::RCOutput_iomcu_b() = default;

void RCOutput_iomcu_b::init() {
    iomcu_b.init();
}

void RCOutput_iomcu_b::set_freq(uint32_t chmask, uint16_t freq_hz) {

}

uint16_t RCOutput_iomcu_b::get_freq(uint8_t chan) {
    return 50;
}


void RCOutput_iomcu_b::enable_ch(uint8_t chan) {
    enable_mask |= 1 << chan;
    if (!corked && iomcu_b.rc_semaphore.take(1)) {
        iomcu_b.rc_serialized.enabled_mask = enable_mask;
        iomcu_b.rc_semaphore.give();
    }
}

void RCOutput_iomcu_b::disable_ch(uint8_t chan) {
    enable_mask &= ~(1 << chan);
    if (!corked && iomcu_b.rc_semaphore.take(1)) {
        iomcu_b.rc_serialized.enabled_mask = enable_mask;
        iomcu_b.rc_semaphore.give();
    }
}

void RCOutput_iomcu_b::write(uint8_t chan, uint16_t period_us)
{
    if (chan >= CHANNELS) return;
    pwm_duration[chan] = period_us;
    if (!corked && iomcu_b.rc_semaphore.take(1)) {
        uint16_t high, low;
        iomcu_b.rc_serialized.channels[chan / 2].get_pair(high, low);
        if (chan % 2)
            low = period_us;
        else
            high = period_us;
        iomcu_b.rc_serialized.channels[chan / 2].set_pair(high, low);
        iomcu_b.rc_semaphore.give();
    }
}
void RCOutput_iomcu_b::cork() {
    corked = true;
}

void RCOutput_iomcu_b::push() {
    if (iomcu_b.rc_semaphore.take(1)) {
        iomcu_b.rc_serialized.enabled_mask = enable_mask;
        iomcu_b.rc_semaphore.give();
        uint16_t *cursor = pwm_duration;
        for (auto &channel_pair: iomcu_b.rc_serialized.channels) {
            auto high = *cursor++, low = *cursor++;
            channel_pair.set_pair(high, low);
        }
        iomcu_b.rc_semaphore.give();
    }
}



uint16_t RCOutput_iomcu_b::read(uint8_t chan)
{
    if (chan >= CHANNELS) return 0;
    return pwm_duration[chan];
}

void RCOutput_iomcu_b::read(uint16_t* period_us, uint8_t len)
{
    for (uint8_t i = 0; i < MIN(len, CHANNELS); i++) {
        period_us[i] = pwm_duration[i];
    }
}