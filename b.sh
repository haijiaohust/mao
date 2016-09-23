cp ~/x86_64_ranchu_defconfig ~/goldfish/arch/x86/configs
cp ~/f2fs/*.c ~/goldfish/fs/f2fs
cp ~/f2fs/*.h ~/goldfish/fs/f2fs
cp ~/f2fs/f2fs_trace.h ~/goldfish/include/trace/events/
cp ~/f2fs/f2fs_fs.h ~/goldfish/include/linux/
cp ~/f2fs/Kconfig ~/goldfish/fs/f2fs
cp ~/f2fs/Makefile_full ~/goldfish/fs/f2fs/Makefile
~/android-6.0.1_r46/prebuilts/qemu-kernel/build-kernel.sh --arch=x86_64
cp /tmp/kernel-qemu/x86_64-3.10.0/kernel-qemu ~/kernel-ranchu
