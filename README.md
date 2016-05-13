# SampleCode
想要能讓樹莓派的console 能用，還得先把一些開機訊息給關了，才能順利連接 GIoT 模組

###Backup the /boot/cmdline.txt file before you edit it just in case of screw-ups:

sudo cp /boot/cmdline.txt /boot/cmdline_backup.txt

#####Edit the file:
sudo nano /boot/cmdline.txt
#####Running "2014-09-09-wheezy-raspbian.img" the file contained the following:
dwc_otg.lpm_enable=0 console=ttyAMA0,115200 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait
#####Delete any parameters involving the serial port "ttyAMA0", which in this example is:
console=ttyAMA0,115200
#####Which gives:
dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait
#####Press CTRL+X to exit and save .
#####You also need to edit this file <<This is no longer present with Raspbian Jessie:
sudo nano /etc/inittab
#####Search for the serial port usage by using CTRL+W and typing:
ttyAMA0
This should find the line of the file specifying the serial port (if there is one).  Use "#" at the start of the line to comment it out.  Then press CTRL+X to save and exit.
Enter "sudo reboot" to restart the pi and now the UART will be available for another process to use it, or the pins can simply be used as general IO.

######另外，樹莓派3 console 被拿去連接藍牙了，也是可以關掉，然後拿來接GIoT模組，如果還想留着藍牙，RPi 3 就不是合適的設備。
### [購買模組點擊這裏](http://tinyurl.com/buygiot).
