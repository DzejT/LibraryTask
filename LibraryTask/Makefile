SUBDIRS := $(wildcard */.)

.PHONY: all $(SUBDIRS) clean

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

install:
	$(MAKE) -C Library/ install

uninstall:
	$(MAKE) -C Library/ uninstall

clean:
	#$(MAKE) -C $(SUBDIRS) clean | bandžiau daryti taip, bet nesigavo 
	$(MAKE) -C Library/ clean
	$(MAKE) -C Program1/ clean 
	$(MAKE) -C Program2/ clean 
	$(MAKE) -C reader/ clean 
