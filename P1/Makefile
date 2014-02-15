#
# Makefile
# 	Makefile global del sistema
#
all: sistema programas

sistema:
	cd minikernel; make

programas:
	cd usuario; make

clean:
	cd minikernel; make clean
	cd usuario; make clean

# Custom target to execute the kernel
exe:
	./boot/boot ./minikernel/kernel
