#
# Makefile
# 	Makefile global del sistema
#
DELIVERABLE_N = 1
PKG           = minikernel_LopezSanchezOriol-pract-$(DELIVERABLE_N)
PKG_EXAM      = $(PKG)-Modif
DIST          = ./dist
DIST_PKG      = $(DIST)/$(PKG)
DIST_PKG_EXAM = $(DIST_PKG)-Modif
TAR_OPTS      = zcvf

all: sistema programas

sistema:
	cd minikernel; make

programas:
	cd usuario; make

clean:
	rm -rf dist
	cd minikernel; make clean
	cd usuario; make clean

# Target to execute the kernel
exe:
	./boot/boot ./minikernel/kernel

# Target to pack for delivery
dist: clean
	mkdir -p $(DIST_PKG)
	cp -r ./minikernel $(DIST_PKG)/
	cp -r ./usuario $(DIST_PKG)/
	cp -r ./boot $(DIST_PKG)/
	cp ./Makefile $(DIST_PKG)/
	cp ./README $(DIST_PKG)/

tar:
	cd $(DIST); tar $(TAR_OPTS) ./$(PKG).tar.gz ./$(PKG)

build: dist tar

dist_exam: clean
	mkdir -p $(DIST_PKG_EXAM)
	cp -r ./minikernel $(DIST_PKG_EXAM)/
	cp -r ./usuario $(DIST_PKG_EXAM)/
	cp -r ./boot $(DIST_PKG_EXAM)/
	cp ./Makefile $(DIST_PKG_EXAM)/
	cp ./README $(DIST_PKG_EXAM)/

tar_exam:
	cd $(DIST); tar $(TAR_OPTS) ./$(PKG_EXAM).tar.gz ./$(PKG_EXAM)

build_exam: dist_exam tar_exam
