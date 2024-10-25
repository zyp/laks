from laks_build.templates import PeriphInstanceModule

def rules(env):
    periph = env.platform_spec.get('periph', {})

    with PeriphInstanceModule(env, 'laks.periph.adc') as m:
        if 'stm32_adc' in periph:
            m.headers.append('adc/stm32_adc.h')
            for name, data in periph['stm32_adc'].items():
                m.instances.append({
                    'type': 'STM32_ADC_t<STM32_ADC_reg_%s_t>' % data['type'],
                    'name': name,
                    'args': [data['offset']],
                })

        if 'stm32_adc_common' in periph:
            for name, data in periph['stm32_adc_common'].items():
                m.instances.append({
                    'type': 'STM32_ADC_COMMON_t<STM32_ADC_COMMON_reg_%s_t>' % data['type'],
                    'name': name,
                    'args': [data['offset']],
                })

        if 'stm32_opamp' in periph:
            m.headers.append('adc/stm32_opamp.h')
            for name, data in periph['stm32_opamp'].items():
                m.instances.append({
                    'type': 'STM32_OPAMP_t',
                    'name': name,
                    'args': [data['offset']],
                })
