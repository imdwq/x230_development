# 部分常用操作

## 配置bootloader环境
setenv ipaddr 192.168.208.127  
setenv serverip 192.168.208.27  

## 配置编译环境
### 编译程序用编译器  
/opt/arm-2016.08/bin/arm-linux-  
arm-none-linux-gnueabi-gcc  
arm-linux-gcc  
### 编译内核用编译器  
/opt/arm-2011.08/bin/arm-linux-  
### 环境变量设置
export PATH=/opt/arm-2016.08/bin:$PATH  
export ARCH=arm  

## 主机与x210v3的通信
### minicom下tftp常用命令
tftp 0x30008000 zImage2.6  
tftp 0x40000000 ramdisk_img2.gz  
bootm 0x30008000  
/srv/tftpboot/ 主机tftp文件夹  
### 使用NFS访问主机文件(默认主机已开启nfs-sever)
mount 192.168.208.27:/srv/nfs4 /mnt -o nolock,proto=tcp 

## 内核编译
### make命令的使用
（读linux3.0,8默认配置文件）make x210_initrd_defconfig    
make x210ii_initrd_defconfig(linux2.6)  
make menuconfig  
make -j8 8线程编译  

### menuconfig中重要的设置

## 创建文件系统

### 使用NFS文件系统在bootloader下的传递参数
setenv rootfs root=/dev/nfs rw nfsroot=192.168.208.27:/srv/nfs4/nfslinux  
setenv nfsaddrs nfsaddrs=192.168.208.127:192.168.208.27:192.168.208.254:255.255.0.0  
setenv bootargs console=ttySAC2,115200 $rootfs $nfsaddrs  
saveenv

### 创建ramdisk的方法
dd if=/dev/zero of=ramdisk_img bs=1k count=8192  
mke2fs ramdisk_img(使用ext2文件格式格式化)  
mount ramdisk_img  
umount /mnt/ramdisk  
### 制作包含于内核中的ramdisk
find ./ -print |cpio -H newc -o |gzip -9 > ~/ramdisk.cpio.gz  
** 必须在根目录下添加init！！ /sbin/init**  

## linux操作相关  
### linux常用快捷键  
Ctrl+c 中断程序  
Ctrl+z 暂停(stop)程序  
Alt+1234 切换终端标签页  
Ctrl+Shift+T 打开新终端标签页
Ctrl+Shift+W 
### linux命令 
ifconfig eth0 192.168.208.127  
cp -r  拷贝整个文件夹  
ln -s
### 系统权限限制
只能mount ramdisk, umount要在ramdisk文件目录  
dev型文件不能拷贝， 只能用mknod新建
