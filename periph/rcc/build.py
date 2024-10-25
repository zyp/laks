from laks_build.templates import PeriphInstanceModule

from erect.lib import Jinja2

def rules(env):
    periph = env.platform_spec.get('periph', {})


    with PeriphInstanceModule(env, 'laks.periph.rcc') as m:
        if 'rcc' in periph:
            buses = env.platform_spec['periph']['rcc']['RCC']['bus']
            env.generated_sources.append(Jinja2(env, 'rcc_enums.cpp', env.laks_path / 'periph/rcc/rcc_enums.cpp.j2', buses = buses))
            m.reexports.append('laks.periph.rcc.enums')

            m.headers.append('periph/rcc/rcc_reg.h')
            for name, data in periph['rcc'].items():
                m.instances.append({
                    'type': 'RCC_t<RCC_reg_%s_t>' % data['type'],
                    'name': name,
                    'args': [data['offset']],
                })

            env.laks_sources.append(env.laks_path / 'periph/rcc/rcc_init.cpp')

    with PeriphInstanceModule(env, 'laks.periph.flash') as m:
        if 'stm32_flash' in periph:
            m.headers.append('periph/rcc/stm32_flash.h')
            for name, data in periph['stm32_flash'].items():
                m.instances.append({
                    'type': 'STM32_FLASH_t<STM32_FLASH_reg_%s_t>' % data['type'],
                    'name': name,
                    'args': [data['offset']],
                })

            env.laks_sources.append(env.laks_path / 'periph/rcc/flash_init.cpp')
