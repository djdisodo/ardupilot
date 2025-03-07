#pragma once

#include "AP_HAL_Linux.h"
#include "PWM_Sysfs.h"

namespace Linux {


class RCOutput_Sysfs_Table : public AP_HAL::RCOutput {
public:

    typedef struct ch_t {
        uint8_t chip;
        uint8_t channel;
    } RCOutput;

    RCOutput_Sysfs_Table(ch_t* channel_table, uint8_t channel_count);
    ~RCOutput_Sysfs_Table();

    static RCOutput_Sysfs_Table *from(AP_HAL::RCOutput *rcoutput)
    {
        return static_cast<RCOutput_Sysfs_Table *>(rcoutput);
    }

    void init() override;
    void set_freq(uint32_t chmask, uint16_t freq_hz) override;
    uint16_t get_freq(uint8_t ch) override;
    void enable_ch(uint8_t ch) override;
    void disable_ch(uint8_t ch) override;
    void write(uint8_t ch, uint16_t period_us) override;
    uint16_t read(uint8_t ch) override;
    void read(uint16_t *period_us, uint8_t len) override;
    void cork(void) override;
    void push(void) override;

private:
    const ch_t *_channel_table;
    const uint8_t _channel_count;
    PWM_Sysfs_Base **_pwm_channels;

    // for handling cork()/push()
    bool _corked;
    uint16_t *_pending;
    uint32_t _pending_mask;
};

}

