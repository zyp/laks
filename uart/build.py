from laks_build.templates import PeriphInstanceModule

def rules(env):
    periph = env.platform_spec.get('periph', {})

    with PeriphInstanceModule(env, 'laks.periph.uart') as m:
        if 'stm32_uart' in periph:
            env.laks_sources.append(env.laks_path / 'uart/stm32.cpp')
            m.imports.append('laks.periph.uart.stm32')
            for name, data in periph['stm32_uart'].items():
                m.instances.append({
                    'type': 'STM32_UART_t<STM32_UART_reg_%s_t>' % data['type'],
                    'name': name,
                    'args': [data['offset']],
                })
