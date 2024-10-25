from laks_build.templates import PeriphInstanceModule

def rules(env):
    periph = env.platform_spec.get('periph', {})

    with PeriphInstanceModule(env, 'laks.periph.exti') as m:
        if 'stm32_exti' in periph:
            m.headers.append('periph/exti/stm32_exti.h')
            for name, data in periph['stm32_exti'].items():
                m.instances.append({
                    'type': 'STM32_EXTI_t<STM32_EXTI_reg_%s_t>' % data['type'],
                    'name': name,
                    'args': [data['offset']],
                })