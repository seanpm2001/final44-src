exec-file obj/gated
symbol-file obj/gated
set prettyprint
shell /bin/rm -f /var/tmp/gated_dump
define term
set trace_flags = trace_flags & ~0x04
signal 15
end
set args -c -tkernel,update,protocol
