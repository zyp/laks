from laks_build.templates import PeriphInstanceModule

def rules(env):
    periph = env.platform_spec.get('periph', {})

    with PeriphInstanceModule(env, 'laks.periph.display') as m:
        if 'stm32_ltdc' in periph:
            m.headers.append('periph/display/stm32_ltdc.h')
            for name, data in periph['stm32_ltdc'].items():
                m.instances.append({
                    'type': 'STM32_LTDC_t',
                    'name': name,
                    'args': [data['offset']],
                })
