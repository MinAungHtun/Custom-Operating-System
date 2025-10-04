





OPTIMIZATION = 0
GCCPARAMS = -m32 -O$(OPTIMIZATION) -std=c++11 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wall -Wextra -pedantic-errors -Iinclude
ASPARAMS = --32
LDPARAMS = -melf_i386


objects = obj/loader.o \
          obj/constructors.o \
          obj/globaldescriptortable.o \
          obj/memorymanagement.o \
          obj/common/outputstream.o \
          obj/common/inputstream.o \
          obj/common/color.o \
          obj/common/graphicscontext.o \
          obj/common/font.o \
          obj/hardwarecommunication/devicedescriptor.o \
          obj/hardwarecommunication/port.o \
          obj/hardwarecommunication/bus.o \
          obj/hardwarecommunication/interruptstubs.o \
          obj/hardwarecommunication/interrupts.o \
          obj/hardwarecommunication/peripheralcomponentinterconnect.o \
          obj/drivers/driver.o \
          obj/drivers/rawdatahandler.o \
          obj/drivers/console/console.o \
          obj/drivers/console/textmodeconsole.o \
          obj/drivers/console/consolearea.o \
          obj/drivers/console/consolestream.o \
          obj/drivers/keyboard/keyboard.o \
          obj/drivers/keyboard/keyboardinterpreter.o \
          obj/drivers/mouse/mouse.o \
          obj/drivers/clock/clock.o \
          obj/drivers/video/video.o \
          obj/drivers/video/videographicsarray.o \
          obj/drivers/video/videoelectronicsstandardsassociation.o \
          obj/drivers/ethernet/ethernet.o \
          obj/drivers/ethernet/amd_am79c973.o \
          obj/protocols/ethernet/ethernetframe.o \
          obj/protocols/ethernet/internetprotocol.o \
          obj/protocols/ethernet/addressresolutionprotocol.o \
          obj/protocols/ethernet/internetcontrolmessageprotocol.o \
          obj/protocols/ethernet/transmissioncontrolprotocol.o \
          obj/protocols/ethernet/userdatagramprotocol.o \
          obj/graphicalobjects/graphicalobject.o \
          obj/graphicalobjects/desktop.o \
          obj/graphicalobjects/window.o \
          obj/graphicalobjects/textbox.o \
          obj/graphicalobjects/button.o \
          obj/graphicalobjects/label.o \
          obj/applications/application.o \
          obj/applications/myynet.o \
          obj/kernel.o




MYYOS.bin: linker.ld $(objects)
	mkdir -p $(@D)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

obj/protocols/ethernet/addressresolutionprotocol.o: src/protocols/ethernet/addressresolutionprotocol.cpp
	# optimization explicitly turned off, because it replaces cache-waiting with an infinite loop :-(
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -O0 -c -o $@ $<

obj/globaldescriptortable.o: src/globaldescriptortable.cpp
	# optimization explicitly turned off (because it messes up the asm("lgdt" ...)
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -O0 -c -o $@ $<




MYYOS.iso: MYYOS.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/MYYOS.bin
	cp gpl3.txt iso/
	echo 'set timeout=0'                 > iso/boot/grub/grub.cfg
	echo 'set default=0'                >> iso/boot/grub/grub.cfg
	echo ''                             >> iso/boot/grub/grub.cfg
	echo 'menuentry "MYYOS" {'          >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/MYYOS.bin'  >> iso/boot/grub/grub.cfg
	echo '  boot'                       >> iso/boot/grub/grub.cfg
	echo '}'                            >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

.PHONY: zip
zip: MYYOS.zip
MYYOS.zip: MYYOS.iso
	zip -r MYYOS.zip include src makefile linker.ld gpl3.txt

.PHONY: run
run: MYYOS.iso
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm MYYOS &

.PHONY: install
install: MYYOS.bin
	sudo cp MYYOS.bin /boot/MYYOS.bin

.PHONY: clean
clean:
	rm $(objects) MYYOS.bin MYYOS.iso 2>/dev/null || true
