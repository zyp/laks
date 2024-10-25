from laks_build.templates import PeriphInstanceModule

def rules(env):
    periph = env.platform_spec.get('periph', {})

    with PeriphInstanceModule(env, 'laks.periph.syscfg') as m:
        if 'stm32_syscfg' in periph:
            m.headers.append('syscfg/stm32_syscfg.h')
            for name, data in periph['stm32_syscfg'].items():
                # allow overrides, but default to family for syscfg, it's a little special
                ptype = data.get('type', env.platform_spec['meta']['family'])
                m.instances.append({
                    'type': 'STM32_SYSCFG_t<STM32_SYSCFG_reg_%s_t>' % ptype,
                    'name': name,
                    'args': [data['offset']],
                })