_ram_start = ORIGIN(ram);
_ram_size = LENGTH(ram);
_ram_end = _ram_start + _ram_size;

SECTIONS {
	. = 0;
	
	.vectors : ALIGN(16) SUBALIGN(16) {
		KEEP(*(.vectors))
	} > flash
	
	.init_array : ALIGN(4) SUBALIGN(4) {
		PROVIDE(_init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE(_init_array_end = .);
	} > flash
	
	.fini_array : ALIGN(4) SUBALIGN(4) {
		PROVIDE(_fini_array_start = .);
		KEEP(*(.fini_array))
		KEEP(*(SORT(.fini_array.*)))
		PROVIDE(_fini_array_end = .);
	} > flash
	
	.text : ALIGN(4) SUBALIGN(4) {
		*(.text.startup.*)
		*(.text)
		*(.text.*)
		*(.rodata)
		*(.rodata.*)
		*(.glue_7t)
		*(.glue_7)
		*(.gcc*)
	} > flash
	
	.ARM.extab : {
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > flash
	
	.ARM.exidx : {
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > flash
	
	.eh_frame_hdr : {
		*(.eh_frame_hdr)
	} > flash
	
	.eh_frame : ONLY_IF_RO {
		*(.eh_frame)
	} > flash
	
	. = ALIGN(4);
	
	PROVIDE(_data_rom = .);
	
	.data : {
		PROVIDE(_data_start = .);
		*(.data)
		. = ALIGN(4);
		*(.data.*)
		. = ALIGN(4);
		*(.ramtext)
		. = ALIGN(4);
		PROVIDE(_data_end = .);
	} > ram AT > flash
	
	.bss : {
		PROVIDE(_bss_start = .);
		*(.bss)
		. = ALIGN(4);
		*(.bss.*)
		. = ALIGN(4);
		*(COMMON)
		. = ALIGN(4);
		PROVIDE(_bss_end = .);
	} > ram    
}
