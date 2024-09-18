

all:
	$(MAKE) -C libuya
	$(MAKE) -C patch
	$(MAKE) -C unpatch
	$(MAKE) -C juggernaut
	# $(MAKE) -C spleef
	# $(MAKE) -C infected
	
clean:
	$(MAKE) -C libuya clean
	$(MAKE) -C patch clean
	$(MAKE) -C unpatch clean
	$(MAKE) -C juggernaut clean
	$(MAKE) -C spleef clean
	$(MAKE) -C infected clean

install:
	$(MAKE) -C libuya install
