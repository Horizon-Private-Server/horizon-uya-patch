

all:
	$(MAKE) -C libuya
	$(MAKE) -C patch
	$(MAKE) -C unpatch
	
clean:
	$(MAKE) -C libuya clean
	$(MAKE) -C patch clean
	$(MAKE) -C unpatch clean

install:
	$(MAKE) -C libuya install
