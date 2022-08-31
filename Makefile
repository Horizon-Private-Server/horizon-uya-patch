

all:
	$(MAKE) -C libuya
	$(MAKE) -C patch
	$(MAKE) -C unpatch
	$(MAKE) -C example-cgm
	
clean:
	$(MAKE) -C libuya clean
	$(MAKE) -C patch clean
	$(MAKE) -C unpatch clean
	$(MAKE) -C example-cgm clean

install:
	$(MAKE) -C libuya install
