from laks_build.templates import PeriphInstanceModule

def rules(env):
    periph = env.platform_spec.get('periph', {})

    with PeriphInstanceModule(env, 'laks.periph.pwr') as m:
        if 'stm32_pwr' in periph:
            m.headers.append('pwr/stm32_pwr.h')
            for name, data in periph['stm32_pwr'].items():
                m.instances.append({
                    'type': 'STM32_PWR_t<STM32_PWR_reg_%s_t>' % data['type'],
                    'name': name,
                    'args': [data['offset']],
                })