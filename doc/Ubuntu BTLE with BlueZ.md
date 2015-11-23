## Connecting Emotiv device via BTLE on Ubuntu 14.04 with BlueZ library
### Install and configure lastest bluez
#### Build bluez
```shell
mkdir -p ~/tmp/bluez && cd ~/tmp/bluez
wget -c https://www.kernel.org/pub/linux/bluetooth/bluez-5.36.tar.gz
tar -xvzf bluez-5.36.tar.gz
cd bluez-5.36/
./configure --disable-systemd --enable-threads --enable-library
make
```

#### Configure
   * Add `EnableGatt = true` to file `/etc/bluetooth/main.conf`
   * Backup then edit file `/etc/dbus-1/system.d/bluetooth.conf`, remove `1` from all the lines, such as:
   
    `<allow send_interface="org.bluez.Agent1"/>` to `<allow send_interface="org.bluez.Agent"/>`
    
    `<allow send_interface="org.bluez.GattDescriptor1"/>`  to `<allow send_interface="org.bluez.GattDescriptor"/>`

### Run bluez service and test connection
#### Stop old bluez service
`sudo /etc/init.d/bluetooth stop`

#### Start new bluez
`sudo ./src/bluetoothd -n -E --plugin=audio`

#### Test connection
Open other terminal and run `bluetoothctl`. Use following commands to test connection:
* List available Bluetooth adapters

  `[bluetooth]#list`

* Power on Bluetooth adapter

  `[bluetooth]#power on`

* Set Bluetooth adapter discoverable

  `[bluetooth]#discoverable on`

* Set Bluetooth adapter pairable

  `[bluetooth]#pairable on`

* Tell Bluetooth adapter to scan Bluetooth devices
  
  `[bluetooth]#scan on`

#### List and connect to device
* List all available bluetooth devices

  `[bluetooth]#devices`

* Get device info
  
  `[bluetooth]#info [MAC_Address]`

  > Each Emotiv device has a specific service UUID `81072f40-9f3d-11e3-a9dc-0002a5d5c51b`

* Connect to an Emotiv device
  
  `[bluetooth]#connect [MAC Address]`

Now you can run the SDK or app on Ubuntu - enjoy!

#### Using graphical view
To use the graphical view instead of command line, run `d-feet &` then select `/org.bluez/hci0/[MAC_Address]`
