

all:
	$(MAKE) -C libuya
	$(MAKE) -C patch
	$(MAKE) -C unpatch
	$(MAKE) -C midflag
	$(MAKE) -C domination
	$(MAKE) -C koth
	
clean:
	$(MAKE) -C libuya clean
	$(MAKE) -C patch clean
	$(MAKE) -C unpatch clean
	$(MAKE) -C example-cgm clean
	$(MAKE) -C spleef clean
	$(MAKE) -C infected clean
	$(MAKE) -C midflag clean
	$(MAKE) -C domination clean
	$(MAKE) -C koth clean

install:
	$(MAKE) -C libuya install
