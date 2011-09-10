#!/bin/bash

. mk/mk.inc

ARCH="i386"

CC="gcc"
CFLAGS="-ffreestanding -pedantic --std=c99"
WARNINGS="-W -Wall -Wcast-qual -Wcast-align -Winline -Wmissing-prototypes -Wwrite-strings -Wredundant-decls -Wpointer-arith -Wchar-subscripts -Wshadow -Wstrict-prototypes -Werror"
CFLAGS="$CFLAGS $WARNINGS"
INCLUDES="-Isrc/include -Isrc/include/gnu"

LD="ld"
LDSCRIPT="conf/kernel.ld"
LDFLAGS="-T ${LDSCRIPT}"

BOCHS="bochs"

case "$target" in
    run)
        run -p ${BOCHS} -q -f conf/bochs.conf
        ;;
    qemu)
        run -p qemu -no-shutdown -no-reboot -fda floppy.img -boot a
        ;;
    dump-loader)
        run -v hexdump bin/mbr.bin
        ;;
    clean)
	rmIfExists obj
        rmIfExists floppy.img
        ;;
    *)
        # Directory for architecture dependant object files
        KERN_ARCH_OBJ_DIR="obj/kern/${ARCH}"

        # Create directories for objects files
        directories="obj obj/kern obj/kern/${ARCH} obj/kern/entry/ obj/boot"
        for d in $directories; do
            if [ ! -d "$d" ]; then
                run mkdir -p $d
            fi
        done

        BOOT_SRC="src/arch/i386/boot/mbr.s"
        BOOT_OBJ="obj/boot/mbr.bin"
        if stale $BOOT_OBJ $BOOT_SRC; then
            run -v nasm -f bin -o $BOOT_OBJ $BOOT_SRC
        fi
        
        LOCORE_SRC="src/arch/i386/src/locore.s"
        LOCORE_OBJ="${KERN_ARCH_OBJ_DIR}/locore.o"
        if stale $LOCORE_OBJ $LOCORE_SRC; then
            run -v nasm -f elf32 -o $LOCORE_OBJ $LOCORE_SRC
        fi

        INTRSTUB_SRC="src/arch/i386/src/intrstub.s"
        INTRSTUB_OBJ="${KERN_ARCH_OBJ_DIR}/intrstrub.o"
        if stale $INTRSTUB_OBJ $INTRSTUB_SRC; then
            run -v nasm -f elf32 -o $INTRSTUB_OBJ $INTRSTUB_SRC
        fi

        ENTRY_SRC="src/arch/i386/src/entry.s"
        ENTRY_OBJ="obj/kern/entry/entry.o"
        if stale $ENTRY_OBJ $ENTRY_SRC; then
            run -v nasm -f elf32 -o $ENTRY_OBJ $ENTRY_SRC
        fi

        # Build architecture dependant objects
        KERN_ARCH_SRC="src/arch/${ARCH}/src/*.c"
        KERN_ARCH_HEADERS="src/arch/${ARCH}/include/*.h"
        for s in $KERN_ARCH_SRC; do
            object_name="`basename ${s} .c`.o"
            object_file="${KERN_ARCH_OBJ_DIR}/${object_name}"
            if stale $object_file $s; then
                run -v ${CC} ${CFLAGS} ${INCLUDES} -c -o $object_file $s
            fi
        done

        # Build architecture independant objects
        KERN_INDEP_SRC="src/kern/*.c"
        KERN_INDEP_HEADERS="src/include/*.h src/include/gnu/*.h"
        KERN_INDEP_OBJ_DIR="obj/kern"
        for s in $KERN_INDEP_SRC; do
            object_name="`basename ${s} .c`.o"
            object_file="${KERN_INDEP_OBJ_DIR}/${object_name}"
            if stale $object_file $s; then
                run -v ${CC} ${CFLAGS} ${INCLUDES} -c -o $object_file $s
            fi
        done

        KERN_IMG="obj/kernel.img"
        KERN_OBJS="${KERN_ARCH_OBJ_DIR}/*.o ${KERN_INDEP_OBJ_DIR}/*.o"
        if stale $KERN_IMG $KERN_OBJS $ENTRY_OBJ; then
            run -v ${LD} ${LDFLAGS} -o $KERN_IMG $KERN_OBJS
        fi

	# Construct an 'empty' floppy and populate it
        if stale floppy.img obj/kernel.img obj/boot/mbr.bin; then
	    run dd if=/dev/zero of=obj/floppy.img bs=512 count=$((2 * 80 * 18))
            run dd conv=notrunc if=obj/boot/mbr.bin of=obj/floppy.img bs=512 count=1
            run dd conv=notrunc if=obj/kernel.img of=obj/floppy.img bs=512 seek=1
            run mv obj/floppy.img .
        fi
        ;;
esac
