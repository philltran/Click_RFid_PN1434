![MikroE](http://www.mikroe.com/img/designs/beta/logo_small.png)

![RFid click](https://cdn1-shop.mikroe.com/img/product/rfid-click/rfid-click-large_default-12x.jpg)

# Click_RFid_PN1434
Library for RFid click board 1434 (https://www.mikroe.com/rfid-click).

---
### Useful Links

[Product Page](https://www.mikroe.com/rfid-click)

[Manual Page](https://www.mikroe.com/rfid-click#idTabFSCHT765)

[Learn Page](http://learn.mikroe.com/what-is-rfid/)

[RFid click example on Libstock](https://libstock.mikroe.com/projects/view/739/rfid-click-example)

[RFid click schematic](https://download.mikroe.com/documents/add-on-boards/click/rfid/rfid-click-schematic-v103.pdf)

[CR95HF datasheet](https://download.mikroe.com/documents/datasheets/cr95hf-datasheet.pdf)

---
### General Description

RFid click features CR95HF 13.56 MHz contactless transceiver as well as trace antenna. The click is designed to run on a 3.3V power supply only. It communicates with the target MCU via mikroBUS™ UART (TX, RX), SPI (MISO, MOSI, SCK, CS) INT, RST, PWM and AN lines.

The click functions as an integrated transceiver for contactless applications. The board contains a dedicated internal frame controller and analog front end (AFE) for RF communications. It supports ISO/IEC 14443 type A and B, ISO/IEC 15693 and ISO/IEC 18092 protocols (tags) as well as the detection, reading and writing of NFC forum type 1, 2, 3 and 4 tags.

### CR95HF integrated transceiver
The CR95HF has 2 operating modes: Wait for Event (WFE) and Active.

In Active mode, the CR95HF communicates actively with a tag or an external host. WFE mode includes four low consumption states: Power-up, Hibernate, Sleep and Tag Detector. The transceiver can switch from one mode to another.

The Hibernate state has the lowest power consumption out of the four states (1 µA typ. 5 µA max.).

### SMD jumpers
Two SMD jumpers are provided on the board. In case you want to use UART interface, it is necessary to solder SMD jumpers in B position. Otherwise, if you want to use SPI interface, jumpers should be left in its default position (A). In that case interrupt pins (IN and OUT) are usable.

---
### Features
- CR95HF IC
-- Operating modes supported: Reader/Writer
-- RF communication @13.56 MHz
-- Supports NFC Forum Type 1, 2, 3 and 4 tags
- 3.3V power supply
- Interface: UART, SPI

---