
setenv ipaddr 192.168.208.127
setenv serverip 192.168.208.27
/srv/tftpboot/ 主机tftp文件夹
编译内核用编译器  /opt/arm-2016.08/bin/arm-linux-


export PATH=/opt/arm-2016.08/bin:$PATH
export ARCH=arm


tftp 0x30008000 zImage
tftp 0x40000000 ramdisk_img2.gz

bootm 0x30008000

Ctrl+c 中断程序


# mount 192.168.208.27:/srv/nfs4 /mnt -o nolock,proto=tcp 


/ # cat /mnt/xxx


ifconfig eth0 192.168.208.127


make x210_initrd_defconfig


arm-none-linux-gnueabi-gcc
make -j8
arm-linux-gcc



tar xf /usr/src/x210v3/kernel/linux-3.0.8.tar.gz 

（读linux3.0,8默认配置文件）make x210_initrd_defconfig
make x210ii_initrd_defconfig(linux2.6)



dd if=/dev/zero of=ramdisk_img bs=1k count=8192
mke2fs ramdisk_img
mount ramdisk_img
umount /mnt/ramdisk

find ./ -print |cpio -H newc -o |gzip -9 > ~/ramdisk.cpio.gz
