umount /mnt
rmmod f2fs
make
insmod f2fs.ko
/root/f2fs-tools/mkfs/mkfs.f2fs /dev/sdc
#mkfs.f2fs /dev/sdc
mount -o background_gc=off  /dev/sdc /mnt
#mkdir /mnt/p
~/ioc /mnt/
#cd /opt/ltp/; for((i=1;i<=1;i++));do ./runltp -f fs_1 -d /mnt;done
