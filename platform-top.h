#if defined(CONFIG_MICROBLAZE)
#include <configs/microblaze-generic.h>
#define CONFIG_SYS_BOOTM_LEN 0xF000000
#endif
#if defined(CONFIG_ARCH_ZYNQ)
#include <configs/zynq-common.h>
#endif
#if defined(CONFIG_ARCH_ZYNQMP)
#include <configs/xilinx_zynqmp.h>
#endif
#if defined(CONFIG_ARCH_VERSAL)
#include <configs/xilinx_versal.h>
#endif
#if defined(CONFIG_ARCH_VERSAL_NET)
#include <configs/xilinx_versal_net.h>
#endif


#if 1

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS \
    ENV_MEM_LAYOUT_SETTINGS \
    BOOTENV \
    "ipaddr=192.168.0.102\0" \
    "netmask=255.255.255.0\0" \
    "gatewayip=192.168.0.254\0" \
    "serverip=192.168.0.80\0" \
    "tftpdir=evm\0" \
    \
    \
    "bootbin=BOOT.BIN\0" \
    "fitimagefile=image.ub\0" \
    "jffsfile=rootfs.jffs2\0" \
    \
    "bootfile=uboot.bin\0" \
    "bootfile_offset=0x00000000\0" \
    "bootfile_size=0x00200000\0" \
    \
    "bootenvfile=uboot.env\0" \
    "bootenvfile_offset=0x00260000\0" \
    "bootenvfile_size=0x00020000\0" \
    \
    "bootscrfile=boot.scr\0" \
    "bootscrfile_offset=0x00200000\0" \
    "bootscrfile_size=0x00020000\0" \
    "script_offset_f=0x00200000\0" \
    "script_size_f=0x00020000\0" \
    \
    "dtbfile=system.dtb\0" \
    "dtbfile_offset=0x00220000\0" \
    "dtbfile_size=0x00040000\0" \
    \
    "plfile=system.bit\0" \
    "plfile_offset=0x00280000\0" \
    "plfile_size=0x02000000\0" \
    "plfile_realsize=0x02000000\0" \
    \
    "kernelfile=Image.gz\0" \
    "kernelfile_offset=0x02280000\0" \
    "kernelfile_size=0x00A00000\0" \
    \
    "ramfsfile=rootfs.cpio.gz.u-boot\0" \
    "ramfsfile_offset=0x02C80000\0" \
    "ramfsfile_size=0x00F00000\0" \
    \
    "sparefile=spare.jffs2\0" \
    "sparefile_offset=0x03B80000\0" \
    "sparefile_size=0x04480000\0" \
    \
    \
    "bootcmd=run distro_bootcmd\0" \
    "boot_targets=mmc0 qspi0 recovery\0" \
    "distro_bootcmd=for target in ${boot_targets}; do echo; echo Load boot images from ${target}; run bootcmd_${target}; done\0" \
    \
    "bootargs_ram=setenv bootargs earlycon console=ttyPS0,115200 clk_ignore_unused root=/dev/ram rw rootwait\0" \
    "bootargs_mmc=setenv bootargs earlycon console=ttyPS0,115200 clk_ignore_unused root=/dev/mmcblk0p2 rw rootwait\0" \
    "bootfrom_sf=sf probe 0 0 0; sf read ${loadaddr} ${plfile_offset} ${plfile_size}; fpga load 0 ${loadaddr} ${plfile_realsize}; sf read 0x100000 ${dtbfile_offset} ${dtbfile_size}; sf read 0x200000 ${kernelfile_offset} ${kernelfile_size}; sf read 0x4000000 ${ramfsfile_offset} ${ramfsfile_size}\0" \
    "bootfrom_tftp=tftpboot ${loadaddr} ${tftpdir}/${plfile}; fpga load 0 ${loadaddr} ${filesize}; tftpboot 0x100000 ${tftpdir}/${dtbfile}; tftpboot 0x200000 ${tftpdir}/${kernelfile}; tftpboot 0x4000000 ${tftpdir}/${ramfsfile}\0" \
    "bootfrom_mmc=fatload mmc 0:1 ${loadaddr} /${plfile}; fpga load 0 ${loadaddr} ${plfile_size}; fatload mmc 0:1 0x100000 /${dtbfile}; fatload mmc 0:1 0x200000 /${kernelfile}; fatload mmc 0:1 0x4000000 /${ramfsfile}\0" \
    "checkmmc1=if test -e mmc 0:1 /${plfile} && test -e mmc 0:1 /${dtbfile} && test -e mmc 0:1 /${kernelfile} && test -e mmc 0:1 /${ramfsfile}; then setenv status true; else setenv status false; fi\0" \
    "checkmmc2=if test -e mmc 0:2 /bin/sh; then setenv status true; else setenv status false; fi\0" \
    \
    "bootcmd_qspi0=run bootargs_ram; run bootfrom_sf; booti 0x200000 0x4000000 0x100000\0" \
    "bootcmd_mmc0=run bootargs_mmc; run checkmmc2; if test ${status} = true; then run bootfrom_sf; booti 0x200000 - 0x100000; else echo Rootfs not found in mmc 0:2; fi\0" \
    "bootcmd_recovery=run bootargs_ram; run checkmmc1; if test ${status} = true; then run bootfrom_mmc; booti 0x200000 0x4000000 0x100000; else echo Recovery images not found in mmc 0:1; fi\0" \
    \
    "bootcmd_qspi0_net=run bootargs_ram; run bootfrom_tftp; booti 0x200000 0x4000000 0x100000\0" \
    "bootcmd_mmc0_net=run bootargs_mmc; run checkmmc2; if test ${status} = true; then run bootfrom_tftp; booti 0x200000 - 0x100000; else echo Rootfs not found in mmc 0:2; fi\0" \
    \
    \
    "updatecmd=sf probe 0 0 0; sf erase ${targetfile_offset} ${targetfile_size}; tftpboot 0x100000 ${tftpdir}/${targetfile}; sf write 0x100000 ${targetfile_offset} ${filesize}; run updatesize\0" \
    "uploadcmd=sf probe 0 0 0; sf read 0x100000 ${targetfile_offset} ${targetfile_size}; tftpput 0x100000 ${targetfile_size} ${serverip}:${tftpdir}/new_${targetfile}\0" \
    "updatesize=setenv ${targetname}_realsize ${filesize}; saveenv\0" \
    \
    "update_targets=boot dtb pl kernel ramfs\0" \
    "update_all=for target in ${update_targets}; do echo; echo Update an image for ${target}; run update_${target}; done\0" \
    "update_boot=setenv targetname bootfile; setenv targetfile ${bootfile}; setenv targetfile_offset ${bootfile_offset}; setenv targetfile_size ${bootfile_size}; run updatecmd\0" \
    "update_bootenv=setenv targetname bootenvfile; setenv targetfile ${bootenvfile}; setenv targetfile_offset ${bootenvfile_offset}; setenv targetfile_size ${bootenvfile_size}; run updatecmd\0" \
    "update_bootscr=setenv targetname bootscrfile; setenv targetfile ${bootscrfile}; setenv targetfile_offset ${bootscrfile_offset}; setenv targetfile_size ${bootscrfile_size}; run updatecmd\0" \
    "update_dtb=setenv targetname dtbfile; setenv targetfile ${dtbfile}; setenv targetfile_offset ${dtbfile_offset}; setenv targetfile_size ${dtbfile_size}; run updatecmd\0" \
    "update_pl=setenv targetname plfile; setenv targetfile ${plfile}; setenv targetfile_offset ${plfile_offset}; setenv targetfile_size ${plfile_size}; run updatecmd\0" \
    "update_kernel=setenv targetname kernelfile; setenv targetfile ${kernelfile}; setenv targetfile_offset ${kernelfile_offset}; setenv targetfile_size ${kernelfile_size}; run updatecmd\0" \
    "update_ramfs=setenv targetname ramfsfile; setenv targetfile ${ramfsfile}; setenv targetfile_offset ${ramfsfile_offset}; setenv targetfile_size ${ramfsfile_size}; run updatecmd\0" \
    "update_spare=setenv targetname sparefile; setenv targetfile ${sparefile}; setenv targetfile_offset ${sparefile_offset}; setenv targetfile_size ${sparefile_size}; run updatecmd\0" \
    \
    "upload_all=setenv targetname bootbin; setenv targetfile ${bootbin}; setenv targetfile_offset 0x0; setenv targetfile_size 0x8000000; run uploadcmd\0" \
    "upload_boot=setenv targetname bootfile; setenv targetfile ${bootfile}; setenv targetfile_offset ${bootfile_offset}; setenv targetfile_size ${bootfile_size}; run uploadcmd\0" \
    "upload_bootenv=setenv targetname bootenvfile; setenv targetfile ${bootenvfile}; setenv targetfile_offset ${bootenvfile_offset}; setenv targetfile_size ${bootenvfile_size}; run uploadcmd\0" \
    "update_bootscr=setenv targetname bootscrfile; setenv targetfile ${bootscrfile}; setenv targetfile_offset ${bootscrfile_offset}; setenv targetfile_size ${bootscrfile_size}; run uploadcmd\0" \
    "upload_spare=setenv targetname sparefile; setenv targetfile ${sparefile}; setenv targetfile_offset ${sparefile_offset}; setenv targetfile_size ${sparefile_size}; run uploadcmd\0" \

#endif