
include ../sdk/NitroSDK/build/buildtools/commondefs.wine

AS = arm-none-eabi-as
AR = arm-none-eabi-ar
ARFLAGS = rc

all: libsyscall.a

ASFLAGS = -march=armv5te

.PRECIOUS: libsyscall.o

src/%.s: ;

libsyscall.o: libsyscall.s
	$(AS) $(ASFLAGS) -c $< -MD -o $@

libsyscall.a: libsyscall.o
	$(AR) $(ARFLAGS) $< -o $@

clean:
	rm -f libsyscall.o libsyscall.a libsyscall.d
