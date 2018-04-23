.PHONY: clean all

.PRECIOUS: *.o

all: ld41.nes

clean:
	@rm -fv ld41.s
	@rm -fv ls41.o
	@rm -fv ld41.nes
	@rm -fv crt0.o

%.o: %.s
	ca65 $<

%.o: %.c
	cc65 -Oi $< --add-source
	ca65 $*.s
	rm $*.s

%.nes: %.o crt0.o
	ld65 -o $@ -C nes.cfg  crt0.o $< runtime.lib
