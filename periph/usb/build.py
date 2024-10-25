from laks_build.templates import PeriphInstanceModule

def rules(env):
    periph = env.platform_spec.get('periph', {})

    with PeriphInstanceModule(env, 'laks.periph.usb') as m:
        if 'dwc_otg' in periph:
            env.laks_sources.append(env.laks_path / 'periph/usb/dwc_otg.cpp')
            m.imports.append('laks.periph.usb.dwc_otg')
            for name, data in periph['dwc_otg'].items():
                m.instances.append({
                    'type': 'DWC_OTG_t',
                    'name': name,
                    'args': [data['offset']],
                })

        if 'stm32_usb' in periph:
            env.laks_sources.append(env.laks_path / 'periph/usb/stm32.cpp')
            m.imports.append('laks.periph.usb.stm32')
            for name, data in periph['stm32_usb'].items():
                m.instances.append({
                    'type': 'STM32_USB_t<STM32_USB_reg_%s_t>' % data['type'],
                    'name': name,
                    'args': [data['offset'], data['buf_offset']],
                })
