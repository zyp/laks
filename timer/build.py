from laks_build.templates import PeriphInstanceModule

def rules(env):
    periph = env.platform_spec.get('periph', {})

    with PeriphInstanceModule(env, 'laks.periph.timer') as m:
        if 'stm32_timer' in periph:
            m.headers.append('timer/stm32_timer.h')
            for name, data in periph['stm32_timer'].items():
                # ~everyone so far has v1 timers, don't require it until we need it
                real_type = data.get('type', 'v1')
                m.instances.append({
                    'type': 'STM32_TIMER_t<STM32_TIMER_reg_%s_t>' % real_type,
                    'name': name,
                    'args': [data['offset']],
                })
