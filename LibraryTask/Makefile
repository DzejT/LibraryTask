SUBDIRS := $(wildcard */.)

.PHONY: all $(SUBDIRS) clean

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

install:
	$(MAKE) -C lib/ install

uninstall:
	$(MAKE) -C lib/ uninstall

clean:
	#$(MAKE) -C $(SUBDIRS) clean | bandžiau daryti taip, bet nesigavo 
	$(MAKE) -C lib/ clean
	$(MAKE) -C Program1/ clean 
	$(MAKE) -C Program2/ clean 
	$(MAKE) -C reader/ clean 
