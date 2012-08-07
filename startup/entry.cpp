#include <stdint.h>
#include <rcc/rcc.h>

int main();

typedef void (*funcp_t)();

// Symbols from linker script.
extern uint32_t _data_rom;
extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _bss_start;
extern uint32_t _bss_end;
extern funcp_t _init_array_start;
extern funcp_t _init_array_end;
extern funcp_t _fini_array_start;
extern funcp_t _fini_array_end;

void __attribute__((naked)) entry() {
	// Initialize HAL.
	rcc_init();
	
	// Load .data from rom image.
	uint32_t* rp = &_data_rom;
	uint32_t* wp = &_data_start;
	
	while(wp < &_data_end) {
		*wp++ = *rp++;
	}
	
	// Clear .bss.
	wp = &_bss_start;
	
	while(wp < &_bss_end) {
		*wp++ = 0;
	}
	
	// Call constructors.
	funcp_t* fp = &_init_array_start;
	
	while(fp < &_init_array_end) {
		(*fp++)();
	}
	
	// Call main().
	main();
	
	// Call destructors.
	fp = &_fini_array_start;
	
	while(fp < &_fini_array_end) {
		(*fp++)();
	}
	
	// Halt.
	while(1);
}

extern "C" void __cxa_pure_virtual() {
	while (1);
}

void* __dso_handle = 0;
