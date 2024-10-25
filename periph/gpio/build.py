from laks_build.templates import PeriphInstanceModule

def rules(env):
    periph = env.platform_spec.get('periph', {})

    with PeriphInstanceModule(env, 'laks.periph.gpio') as m:
        if 'stm32_gpio' in periph:
            m.headers.append('periph/gpio/stm32_gpio.h')
            for name, data in periph['stm32_gpio'].items():
                # Default to v2, ie, everyone but f1
                real_type = data.get('type', 'v2')
                m.instances.append({
                    'type': 'STM32_GPIO_%s_t' % real_type,
                    'name': name,
                    'args': [data['offset']],
                })

                m.type_aliases['Pin'] = 'STM32_GPIO_%s_t::Pin' % real_type
                m.type_aliases['PinArray'] = 'STM32_GPIO_%s_t::PinArray' % real_type
