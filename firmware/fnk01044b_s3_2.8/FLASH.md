# FNK01044B 2.8" flash files

- `maramau_fnk01044b_s3_2.8_FULL.bin` — flash at `0x0` (recommended).
- `bootloader.bin` (`0x0`), `partitions.bin` (`0x8000`), `maramau_fnk01044b_s3_2.8.bin` (`0x10000`) — flash together.

```bash
python -m esptool --chip esp32s3 -p /dev/ttyACM0 -b 460800 write-flash 0x0 maramau_fnk01044b_s3_2.8_FULL.bin
```
