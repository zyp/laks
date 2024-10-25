from laks_build.templates import PeriphInstanceModule

def rules(env):
    periph = env.platform_spec.get('periph', {})

    with PeriphInstanceModule(env, 'laks.periph.i2c') as m:
        if 'stm32_i2c' in periph:
            m.headers.append('i2c/stm32_i2c.h')
            for name, data in periph['stm32_i2c'].items():
                m.instances.append({
                    'type': 'STM32_I2C_t<STM32_I2C_reg_%s_t>' % data['type'],
                    'name': name,
                    'args': [data['offset']],
                })