from laks_build.templates import PeriphInstanceModule

def rules(env):
    periph = env.platform_spec.get('periph', {})

    with PeriphInstanceModule(env, 'laks.periph.rtc') as m:
        if 'stm32_rtc' in periph:
            m.headers.append('periph/rtc/stm32_rtc.h')
            for name, data in periph['stm32_rtc'].items():
                # Default to version 2, with subseconds, the most common form.
                real_type = data.get('type', 'v2ss')
                m.instances.append({
                    'type': 'STM32_RTC_t<STM32_RTC_reg_%s_t>' % real_type,
                    'name': name,
                    'args': [data['offset']],
                })
