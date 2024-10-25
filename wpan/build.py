from laks_build.templates import PeriphInstanceModule

def rules(env):
    periph = env.platform_spec.get('periph', {})

    with PeriphInstanceModule(env, 'laks.periph.wpan') as m:
        if 'stm32_hsem' in periph:
            m.headers.append('wpan/stm32_hsem.h')
            for name, data in periph['stm32_hsem'].items():
                real_type = data.get('type', 'v1') # Default
                m.instances.append({
                    'type': 'STM32_HSEM_t<STM32_HSEM_reg_%s_t>' % real_type,
                    'name': name,
                    'args': [data['offset']],
                })

        if 'stm32_ipcc' in periph:
            m.headers.append('wpan/stm32_ipcc.h')
            for name, data in periph['stm32_ipcc'].items():
                real_type = data.get('type', 'v1') # Default
                m.instances.append({
                    'type': 'STM32_IPCC_t<STM32_IPCC_reg_%s_t>' % real_type,
                    'name': name,
                    'args': [data['offset']],
                })
