umount /mnt
rmmod f2fs
make
insmod f2fs.ko
/root/f2fs-tools/mkfs/mkfs.f2fs /dev/sdc
mount /dev/sdc /mnt
#dd if=/dev/zero of=/mnt/aaa bs=1M count=16
#sleep 60
#dd if=/dev/zero of=/mnt/bbb bs=1M count=16
#sleep 60
#rm -rf /mnt/bbb
#echo a>/mnt/a
#~/a
#~/b
#~/c
#sleep 30
#rm -rf /mnt/a
#umount /mnt
#mount /dev/sdb /mnt
cd /opt/ltp/; for((i=1;i<=1;i++));do ./runltp -f fs -d /mnt;done
