/* Memory Spaces Definitions */
MEMORY
{
  FLASH    (rx)  : ORIGIN = 0x00010000, LENGTH = 0x000D8000
  RAM      (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00040000
  bkupram  (rwx) : ORIGIN = 0x47000000, LENGTH = 0x00002000
  qspi     (rwx) : ORIGIN = 0x04000000, LENGTH = 0x01000000
}

_stack_start = __ebss;

/* The stack size used by the application. NOTE: Stack and heap sizes should be set in
 * CMakeLists.txt */
STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : DEFINED(__stack_size__) ? __stack_size__ : 0x10000;
HEAP_SIZE = DEFINED(HEAP_SIZE) ? HEAP_SIZE : DEFINED(__heap_size__) ? __heap_size__ : 0x10000;

SECTIONS {
  .bkupram (NOLOAD):
  {
      . = ALIGN(8);
      _sbkupram = .;
      *(.bkupram .bkupram.*);
      . = ALIGN(8);
      _ebkupram = .;
  } > bkupram
  
  .qspi (NOLOAD):
  {
      . = ALIGN(8);
      _sqspi = .;
      *(.qspi .qspi.*);
      . = ALIGN(8);
      _eqspi = .;
  } > qspi
  
  /* stack section */
  .stack (NOLOAD):
  {
      . = ALIGN(8);
      _sstack = .;
      . = . + STACK_SIZE;
      . = ALIGN(8);
      _estack = .;
  } > RAM
  
  . = ALIGN(4);
  _end = . ;
  
  /* heap section */
  .heap (NOLOAD):
  {
      . = ALIGN(8);
      _sheap = .;
      . = . + HEAP_SIZE;
      . = ALIGN(8);
      _sheap = .;
  } > RAM
} INSERT AFTER .bss
