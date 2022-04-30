# Setting up serial pins on raspberry pi.

Add device tree to /boot/config.txt to disable the bluetooth module.

```sh
sudo nano /boot/config.txt
```

Add at the end of the file

```sh
dtoverlay=disable-bt
```

You must also disable the hciuart service which initialises the bluetooth modem:

```sh
sudo systemctl disable hciuart
```

Reboot your Raspberry Pi to enable the changes:

```sh
sudo reboot
```