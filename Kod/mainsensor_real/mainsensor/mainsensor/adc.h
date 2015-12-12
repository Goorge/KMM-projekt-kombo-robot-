#ifndef ADC_H
#define ADC_H

void adc_init();
uint8_t read_adc(uint8_t choose_adc_pin);
uint16_t read_adc_gyro();

#define START_AD_CONVERSION		ADCSRA |= (1<<ADSC);
#define WAIT_FOR_AD_CONVERSION	while(ADCSRA & (1<<ADSC));

#endif