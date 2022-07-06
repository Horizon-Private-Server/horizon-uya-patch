all: clean
	$(MAKE) -f ${CURDIR}/Makefile.ntsc
	$(MAKE) -f ${CURDIR}/Makefile.ntsc clean
	$(MAKE) -f ${CURDIR}/Makefile.pal
	$(MAKE) -f ${CURDIR}/Makefile.pal clean
	
clean:
	$(MAKE) -f ${CURDIR}/Makefile.ntsc clean
	$(MAKE) -f ${CURDIR}/Makefile.pal clean
	rm -f -r lib/

install: clean
	$(MAKE) -f ${CURDIR}/Makefile.ntsc install
	$(MAKE) -f ${CURDIR}/Makefile.ntsc clean
	$(MAKE) -f ${CURDIR}/Makefile.pal install
	$(MAKE) -f ${CURDIR}/Makefile.pal clean
	