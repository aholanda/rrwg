# set to 1 to enable 64bits target by default (32bit is the default)
set $64BITS = 1

set confirm off
set verbose off
set prompt \033[31mgdb$ \033[0m

set output-radix 0x10
set input-radix 0x10

# These make gdb never pause in its output
set height 0
set width 0

# Display instructions in Intel format
set disassembly-flavor intel

set $SHOW_CONTEXT = 1
set $SHOW_NEST_INSN = 0

set $CONTEXTSIZE_STACK = 6
set $CONTEXTSIZE_DATA  = 8
set $CONTEXTSIZE_CODE  = 8
