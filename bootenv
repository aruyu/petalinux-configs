
setenv ipaddr 192.168.0.102
setenv netmask 255.255.255.0
setenv gatewayip 192.168.0.254
setenv serverip 192.168.0.80
setenv tftpdir evm


setenv bootbin BOOT.BIN
setenv fitimagefile image.ub
setenv jffsfile rootfs.jffs2

setenv bootfile uboot.bin
setenv bootfile_offset 0x00000000
setenv bootfile_size 0x00200000

setenv bootenvfile uboot.env
setenv bootenvfile_offset 0x00260000
setenv bootenvfile_size 0x00020000

setenv bootscrfile boot.scr
setenv bootscrfile_offset 0x00200000
setenv bootscrfile_size 0x00020000
setenv script_offset_f 0x00200000
setenv script_size_f 0x00020000

setenv dtbfile system.dtb
setenv dtbfile_offset 0x00220000
setenv dtbfile_size 0x00040000

setenv plfile system.bit
setenv plfile_offset 0x00280000
setenv plfile_size 0x02000000
setenv plfile_realsize 0x02000000

setenv kernelfile Image.gz
setenv kernelfile_offset 0x02280000
setenv kernelfile_size 0x00A00000

setenv ramfsfile rootfs.cpio.gz.u-boot
setenv ramfsfile_offset 0x02C80000
setenv ramfsfile_size 0x00F00000

setenv sparefile spare.jffs2
setenv sparefile_offset 0x03B80000
setenv sparefile_size 0x04480000


setenv boot_targets      'qspi0_mmc qspi0 mmc0'
setenv distro_bootcmd    'for target in ${boot_targets}; do
                            echo;
                            echo **** Load boot images from ${target} ****;
                            run bootcmd_${target};
                          done;'


setenv bootcmd           'run distro_bootcmd'
setenv bootcmd_qspi0_mmc 'setenv devnum 0; setenv rootfs mmc; run qspi_boot;'
setenv bootcmd_qspi0     'setenv devnum 0; setenv rootfs ram; run qspi_boot;'
setenv bootcmd_mmc0      'setenv devnum 0; setenv rootfs ram; run mmc_boot;'
setenv bootcmd_tftp      'setenv rootfs ram; run loadfrom_tftp; run start_booti;'

setenv qspi_boot         'if sf probe 0 0 0; then
                            if test ${rootfs} = ram; then
                              run loadfrom_sf;
                              run start_booti;

                            elif test ${rootfs} = mmc; then
                              if mmc dev ${devnum}; then
                                setenv devtype mmc;
                                setenv status false;
                                run scandev_rootfs;
                                echo;

                                if test ${status} = true; then
                                  echo ** Bootable rootfs has been found in ${devtype} ${devnum}:${devpart} **;
                                  setenv rootfs mmcblk${devnum}p${devpart};
                                  run loadfrom_sf;
                                  run start_booti;

                                else
                                  echo No such bootable rootfs has been found in ${devtype}${devnum}!;
                                fi;

                              else
                                echo No MMC device has been found!;
                              fi;
                            fi;
                          fi;'

setenv tftp_boot         'if test ${rootfs} = ram; then
                            run loadfrom_tftp;
                            run start_booti;

                          elif test ${rootfs} = mmc; then
                            if mmc dev ${devnum}; then
                              setenv devtype mmc;
                              setenv status false;
                              run scandev_rootfs;
                              echo;

                              if test ${status} = true; then
                                echo ** Bootable rootfs has been found in ${devtype} ${devnum}:${devpart} **;
                                setenv rootfs mmcblk${devnum}p${devpart};
                                run loadfrom_tftp;
                                run start_booti;

                              else
                                echo No such bootable rootfs has been found in ${devtype}${devnum}!;
                              fi;

                            else
                              echo No MMC device has been found!;
                            fi;
                          fi;'

setenv mmc_boot          'if mmc dev ${devnum}; then
                            setenv devtype mmc;
                            setenv status false;
                            run scandev_boot;
                            echo;

                            if test ${status} = true; then
                              echo ** All images have been found in ${devtype} ${devnum}:${devpart} **;
                              run loadfrom_dev;
                              run start_booti;

                            else
                              echo No such images have been found in ${devtype}${devnum}!;
                            fi;
                          fi;'


setenv start_booti       'setenv bootargs earlycon console=ttyPS0,115200 clk_ignore_unused root=/dev/${rootfs} rw rootwait;

                          if test ${rootfs} = ram; then
                            booti 0x200000 0x4000000 0x100000;
                          else
                            booti 0x200000 - 0x100000;
                          fi;

                          echo ${target} BOOT FAILED!!;'

setenv loadfrom_sf       'sf read ${loadaddr} ${plfile_offset} ${plfile_size}; fpga load 0 ${loadaddr} ${plfile_realsize};
                          sf read 0x100000 ${dtbfile_offset} ${dtbfile_size};
                          sf read 0x200000 ${kernelfile_offset} ${kernelfile_size};
                          sf read 0x4000000 ${ramfsfile_offset} ${ramfsfile_size};'

setenv loadfrom_tftp     'tftpboot ${loadaddr} ${tftpdir}/${plfile}; fpga load 0 ${loadaddr} ${filesize};
                          tftpboot 0x100000 ${tftpdir}/${dtbfile};
                          tftpboot 0x200000 ${tftpdir}/${kernelfile};
                          tftpboot 0x4000000 ${tftpdir}/${ramfsfile};'

setenv loadfrom_dev      'load ${devtype} ${devnum}:${devpart} ${loadaddr} /${plfile}; fpga load 0 ${loadaddr} ${plfile_size};
                          load ${devtype} ${devnum}:${devpart} 0x100000 /${dtbfile};
                          load ${devtype} ${devnum}:${devpart} 0x200000 /${kernelfile};
                          load ${devtype} ${devnum}:${devpart} 0x4000000 /${ramfsfile};'


setenv scandev_boot      'part list ${devtype} ${devnum} devplist;
                          for part in ${devplist}; do
                            echo;
                            echo Trying to check bootimgs from ${devtype} ${devnum}:${part}.;

                            if test -e ${devtype} ${devnum}:${part} /${plfile} && test -e ${devtype} ${devnum}:${part} /${dtbfile}; then
                              echo ${plfile} has been found.;
                              echo ${dtbfile} has been found.;

                              if test -e ${devtype} ${devnum}:${part} /${kernelfile} && test -e ${devtype} ${devnum}:${part} /${ramfsfile}; then
                                echo ${kernelfile} has been found.;
                                echo ${ramfsfile} has been found.;
                                setenv status true;
                                setenv devpart ${part};
                              fi;
                            fi;
                          done;'

setenv scandev_rootfs    'part list ${devtype} ${devnum} devplist;
                          for part in ${devplist}; do
                            echo;
                            echo Trying to check bootable rootfs from ${devtype} ${devnum}:${part}.;

                            if test -e ${devtype} ${devnum}:${part} /sbin/init && test -e ${devtype} ${devnum}:${part} /bin/sh; then
                              echo /sbin/init has been found.;
                              echo /bin/sh has been found.;
                              setenv status true;
                              setenv devpart ${part};
                            fi;
                          done;'



setenv updatecmd         'sf probe 0 0 0; sf erase ${targetfile_offset} ${targetfile_size}; tftpboot 0x100000 ${tftpdir}/${targetfile}; sf write 0x100000 ${targetfile_offset} ${filesize}; run updatesize;'
setenv uploadcmd         'sf probe 0 0 0; sf read 0x100000 ${targetfile_offset} ${targetfile_size}; tftpput 0x100000 ${targetfile_size} ${serverip}:${tftpdir}/new_${targetfile};'
setenv updatesize        'setenv ${targetname}_realsize ${filesize}; saveenv;'

setenv update_targets    'boot dtb pl kernel ramfs'
setenv update_all        'for target in ${update_targets}; do
                            echo;
                            echo **** Update an image for ${target} ****;
                            run update_${target};
                          done;'

setenv update_boot       'setenv targetname bootfile; setenv targetfile ${bootfile}; setenv targetfile_offset ${bootfile_offset}; setenv targetfile_size ${bootfile_size}; run updatecmd;'
setenv update_bootenv    'setenv targetname bootenvfile; setenv targetfile ${bootenvfile}; setenv targetfile_offset ${bootenvfile_offset}; setenv targetfile_size ${bootenvfile_size}; run updatecmd;'
setenv update_bootscr    'setenv targetname bootscrfile; setenv targetfile ${bootscrfile}; setenv targetfile_offset ${bootscrfile_offset}; setenv targetfile_size ${bootscrfile_size}; run updatecmd;'
setenv update_dtb        'setenv targetname dtbfile; setenv targetfile ${dtbfile}; setenv targetfile_offset ${dtbfile_offset}; setenv targetfile_size ${dtbfile_size}; run updatecmd;'
setenv update_pl         'setenv targetname plfile; setenv targetfile ${plfile}; setenv targetfile_offset ${plfile_offset}; setenv targetfile_size ${plfile_size}; run updatecmd;'
setenv update_kernel     'setenv targetname kernelfile; setenv targetfile ${kernelfile}; setenv targetfile_offset ${kernelfile_offset}; setenv targetfile_size ${kernelfile_size}; run updatecmd;'
setenv update_ramfs      'setenv targetname ramfsfile; setenv targetfile ${ramfsfile}; setenv targetfile_offset ${ramfsfile_offset}; setenv targetfile_size ${ramfsfile_size}; run updatecmd;'
setenv update_spare      'setenv targetname sparefile; setenv targetfile ${sparefile}; setenv targetfile_offset ${sparefile_offset}; setenv targetfile_size ${sparefile_size}; run updatecmd;'

setenv upload_all        'setenv targetname bootbin; setenv targetfile ${bootbin}; setenv targetfile_offset 0x0; setenv targetfile_size 0x8000000; run uploadcmd;'
setenv upload_boot       'setenv targetname bootfile; setenv targetfile ${bootfile}; setenv targetfile_offset ${bootfile_offset}; setenv targetfile_size ${bootfile_size}; run uploadcmd;'
setenv upload_bootenv    'setenv targetname bootenvfile; setenv targetfile ${bootenvfile}; setenv targetfile_offset ${bootenvfile_offset}; setenv targetfile_size ${bootenvfile_size}; run uploadcmd;'
setenv upload_bootscr    'setenv targetname bootscrfile; setenv targetfile ${bootscrfile}; setenv targetfile_offset ${bootscrfile_offset}; setenv targetfile_size ${bootscrfile_size}; run uploadcmd;'
setenv upload_spare      'setenv targetname sparefile; setenv targetfile ${sparefile}; setenv targetfile_offset ${sparefile_offset}; setenv targetfile_size ${sparefile_size}; run uploadcmd;'
