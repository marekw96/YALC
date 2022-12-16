FLASH_SERVER=pi@192.168.0.17
FLASH_DEV=/dev/ttyUSB0

sudo docker run -it --rm -v $(pwd):/app mw:esp32-build /app/YALC.ino
scp generated/YALC.ino.bin generated/YALC.ino.elf generated/YALC.ino.elf generated/YALC.ino.partitions.bin $FLASH_SERVER:/tmp
ssh $FLASH_SERVER "arduino-cli upload --fqbn esp32:esp32:esp32-poe-iso -i /tmp/YALC.ino.bin -p /dev/ttyUSB0"