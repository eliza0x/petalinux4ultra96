mount /dev/sdc1 /mnt/BOOT
mount /dev/sdc2 /mnt/rootfs

rm -rf /mnt/BOOT/* /mnt/rootfs/*

cp BOOT.BIN image.ub system.dtb /mnt/BOOT
tar xvf rootfs.tar.gz -C /mnt/rootfs
sync

umount /mnt/*

