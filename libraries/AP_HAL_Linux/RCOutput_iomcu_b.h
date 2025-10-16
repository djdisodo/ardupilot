#pragma once

#include <AP_HAL/RCOutput.h>

#include "IOMCU_B.h"

#define CHANNELS 16

namespace IOMCU2 {

class RCOutput_iomcu_b: public AP_HAL::RCOutput {
	//state
	bool corked = false;

	// RC Output
	uint32_t enable_mask = 0;
	uint16_t pwm_duration[CHANNELS] = {};
public:
    explicit RCOutput_iomcu_b();

	void init() override;

	void set_freq(uint32_t chmask, uint16_t freq_hz) override;

	uint16_t get_freq(uint8_t chan) override;

    void enable_ch(uint8_t ch) override;

    void disable_ch(uint8_t ch) override;

    void write(uint8_t ch, uint16_t val) override;

    void cork() override;

    void push() override;

    uint16_t read(uint8_t ch) override;

    void read(uint16_t* period_us, uint8_t len) override;

};

}