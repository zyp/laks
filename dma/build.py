from laks_build.templates import PeriphInstanceModule

def rules(env):
    periph = env.platform_spec.get('periph', {})

    with PeriphInstanceModule(env, 'laks.periph.dma') as m:
        if 'stm32_dmamux' in periph:
            m.headers.append('dma/stm32_dmamux.h')
            for name, data in periph['stm32_dmamux'].items():
                m.instances.append({
                    'type': 'STM32_DMAMUX_t<STM32_DMAMUX_reg_%s_t>' % data['type'],
                    'name': name,
                    'args': [data['offset']],
                })
        if 'stm32_dma' in periph:
            m.headers.append('dma/stm32_dma.h')
            for name, data in periph['stm32_dma'].items():
                m.instances.append({
                    'type': 'STM32_DMA_t<STM32_DMA_reg_%s_t>' % data['type'],
                    'name': name,
                    'args': [data['offset']],
                })
