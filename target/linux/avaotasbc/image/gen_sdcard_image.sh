#!/bin/sh
# SPDX-License-Identifier: GPL-2.0-only
#
# Copyright (C) 2013 OpenWrt.org

set -ex
if [ $# -ne 7 ] && [ $# -ne 8 ]; then
    echo "SYNTAX: $0 <file> <bootfs image> <rootfs image> <bootfs size> <rootfs size> <boot0 image> <boot_package image> [<align>]"
    exit 1
fi

OUTPUT="$1"
BOOTFS="$2"
ROOTFS="$3"
BOOTFSSIZE="$4"
ROOTFSSIZE="$5"
BOOT0="$6"
BOOT_PKG="$7"
ALIGN="$8"

head=4
sect=63

set $(ptgen -o $OUTPUT -h $head -s $sect -l 1024 -t c -p ${BOOTFSSIZE}M -t 83 -p ${ROOTFSSIZE}M ${ALIGN:+-l $ALIGN})

BOOTOFFSET="$(($1 / 512))"
BOOTSIZE="$(($2 / 512))"
ROOTFSOFFSET="$(($3 / 512))"
ROOTFSSIZE="$(($4 / 512))"

dd bs=8k if="$BOOT0" of="$OUTPUT" seek=1 conv=notrunc
dd bs=8k if="$BOOT_PKG" of="$OUTPUT" seek=2050 conv=notrunc
dd bs=512 if="$BOOTFS" of="$OUTPUT" seek="$BOOTOFFSET" conv=notrunc
dd bs=512 if="$ROOTFS" of="$OUTPUT" seek="$ROOTFSOFFSET" conv=notrunc
