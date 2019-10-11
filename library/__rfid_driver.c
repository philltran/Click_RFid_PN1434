/*
 * Project name:
     RFid Click (using CR95HF contactless transceiver)
 * Copyright:
     (c) Mikroelektronika, 2013.
 * Revision History:
     20130624:
       - initial release (FJ);
 * Description:
     This is a sample program which demonstrates the use of the ST's
     CR95HF contactless transceiver. The CR95HF is used in the applications such 
     as near field communication (NFC) using 13.56 MHz wireless interface.
     In this example, when the RFid tag is put over the RFid Click antenna, 
     the tag ID will be displayed on the TFT display.
 * Test configuration:
     Device:          STM32F107VC
                      http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/CD00220364.pdf
     Dev.Board:       EasyMx PRO v7 for STM32(R) ARM(R)
                      http://www.mikroe.com/easymx-pro/stm32/
     Oscillator:      HSE, 72.000MHz
     Ext. Modules:    RFid Click Board - ac:RFid_Click
                      http://www.mikroe.com/click/rfid/
                      EasyTFT Board - ac:EasyTFT
                      http://www.mikroe.com/add-on-boards/display/easytft/
     SW:              mikroC PRO for ARM
                      http://www.mikroe.com/mikroc/arm/
 * NOTES:
     - Place RFid click board in the mikroBUS socket 1 on the EasyMx PRO v7 for STM32 board.
     - Place EasyTFT display in the TFT display socket and turn on the backlight using the switch SW11.3.
 */

#include "resources.h"

unsigned short sdata[18];
unsigned short rdata[18];
unsigned short res = 0, dataNum = 0;
unsigned short j = 0, tmp = 0;
int x_pos = 0;
int x_pos_old = 0;
char CR95HF_ID[13];
char ID[38];
char ID_old[38];
char txt_hex[3];
char flag = 0;
unsigned char NFC_flag = 1;
unsigned char TAG_flag = 0;

// Write command to the CR95HF
void writeCmd(unsigned short cmd, unsigned short dataLen){
  unsigned short i = 0;
  
  CS = 0;
  SPI3_Write(0x00);  // Send cmd to CR95HF
  SPI3_Write(cmd);
  SPI3_Write(dataLen);
  while (dataLen == 0){
    CS = 1;
    break;
  }
  for(i=0; i<dataLen; i++){
    SPI3_Write(sdata[i]);
  }
  CS = 1;
}

// Poll the CR95HF
void readCmd(){
  unsigned short i = 0;

  while(1){
    CS = 0;
    SPI3_Write(0x03);
    res = SPI3_Read(0);
    CS = 1;

    if((res & 0x08) >> 3){
      CS = 0;
      SPI3_Write(0x02);
      res = SPI3_Read(0);
      dataNum = SPI3_Read(0);
      for(i=0; i<dataNum; i++)
        rdata[i] = SPI3_Read(0);
      CS = 1;
      break;
    }
    CS = 1;
    Delay_ms(10);
  }
}

// Initialize MCU and peripherals
void Init(){
  // Configure GPIO pins
  GPIO_Digital_Output(&GPIOC_ODR, _GPIO_PINMASK_2);
  GPIO_Digital_Output(&GPIOA_ODR, _GPIO_PINMASK_4);
  GPIO_Digital_Output(&GPIOA_ODR, _GPIO_PINMASK_0);
  GPIO_Digital_Output(&GPIOD_ODR, _GPIO_PINMASK_13);

  // Set in SPI mode
  SSI_0 = 1;
  SSI_1 = 0;
  IRQ_IN = 1;
  Delay_ms(1);

  IRQ_IN = 0;
  Delay_ms(1);
  IRQ_IN = 1;
  Delay_ms(1);

  // Initialize TFT display
  TFT_Init(320, 240);
  Delay_ms(100);
  
  // Clear TFT display
  TFT_Fill_Screen(CL_WHITE);
  
  // Set font
  TFT_Set_Font(TFT_defaultFont, CL_BLACK, FO_HORIZONTAL);
  
  // Initialize SPI module
  SPI3_Init_Advanced(_SPI_FPCLK_DIV64, _SPI_MASTER | _SPI_8_BIT | _SPI_CLK_IDLE_LOW | _SPI_FIRST_CLK_EDGE_TRANSITION | _SPI_MSB_FIRST | _SPI_SS_DISABLE | _SPI_SSM_ENABLE | _SPI_SSI_1, &_GPIO_MODULE_SPI3_PC10_11_12);
  Delay_ms(100);
}

// Get Echo reponse from CR95HF
char EchoResponse() {
  CS = 0;
  SPI3_Write(0x00);  // Send cmd to CR95HF
  SPI3_Write(ECHO);
  CS = 1;

  while(1){
    CS = 0;
    SPI3_Write(0x03);
    tmp = SPI3_Read(0);
    CS = 1;

    if((tmp & 0x08) >> 3){
      CS = 0;
      SPI3_Write(0x02);
      tmp = SPI3_Read(0);
      CS = 1;
      if(tmp == ECHO){
        return 1;
      }
      return 0;
    }
  }
}

// Calibrate CR95HF device
void Calibration() {
  TFT_Write_Text("Calibrating CR95HF...", 55, 100);
  sdata[0] = 0x03;
  sdata[1] = 0xA1;
  sdata[2] = 0x00;
  sdata[3] = 0xF8;
  sdata[4] = 0x01;
  sdata[5] = 0x18;
  sdata[6] = 0x00;
  sdata[7] = 0x20;
  sdata[8] = 0x60;
  sdata[9] = 0x60;
  sdata[10] = 0x00;
  sdata[11] = 0x00;
  sdata[12] = 0x3F;
  sdata[13] = 0x01;
  writeCmd(Idle, 0x0E);
  readCmd();

  sdata[0] = 0x03;
  sdata[1] = 0xA1;
  sdata[2] = 0x00;
  sdata[3] = 0xF8;
  sdata[4] = 0x01;
  sdata[5] = 0x18;
  sdata[6] = 0x00;
  sdata[7] = 0x20;
  sdata[8] = 0x60;
  sdata[9] = 0x60;
  sdata[10] = 0x00;
  sdata[11] = 0xFC;
  sdata[12] = 0x3F;
  sdata[13] = 0x01;
  writeCmd(Idle, 0x0E);
  readCmd();

  sdata[0] = 0x03;
  sdata[1] = 0xA1;
  sdata[2] = 0x00;
  sdata[3] = 0xF8;
  sdata[4] = 0x01;
  sdata[5] = 0x18;
  sdata[6] = 0x00;
  sdata[7] = 0x20;
  sdata[8] = 0x60;
  sdata[9] = 0x60;
  sdata[10] = 0x00;
  sdata[11] = 0x7C;
  sdata[12] = 0x3F;
  sdata[13] = 0x01;
  writeCmd(Idle, 0x0E);
  readCmd();

  sdata[0] = 0x03;
  sdata[1] = 0xA1;
  sdata[2] = 0x00;
  sdata[3] = 0xF8;
  sdata[4] = 0x01;
  sdata[5] = 0x18;
  sdata[6] = 0x00;
  sdata[7] = 0x20;
  sdata[8] = 0x60;
  sdata[9] = 0x60;
  sdata[10] = 0x00;
  sdata[11] = 0x3C;
  sdata[12] = 0x3F;
  sdata[13] = 0x01;
  writeCmd(Idle, 0x0E);
  readCmd();

  sdata[0] = 0x03;
  sdata[1] = 0xA1;
  sdata[2] = 0x00;
  sdata[3] = 0xF8;
  sdata[4] = 0x01;
  sdata[5] = 0x18;
  sdata[6] = 0x00;
  sdata[7] = 0x20;
  sdata[8] = 0x60;
  sdata[9] = 0x60;
  sdata[10] = 0x00;
  sdata[11] = 0x5C;
  sdata[12] = 0x3F;
  sdata[13] = 0x01;
  writeCmd(Idle, 0x0E);
  readCmd();

  sdata[0] = 0x03;
  sdata[1] = 0xA1;
  sdata[2] = 0x00;
  sdata[3] = 0xF8;
  sdata[4] = 0x01;
  sdata[5] = 0x18;
  sdata[6] = 0x00;
  sdata[7] = 0x20;
  sdata[8] = 0x60;
  sdata[9] = 0x60;
  sdata[10] = 0x00;
  sdata[11] = 0x6C;
  sdata[12] = 0x3F;
  sdata[13] = 0x01;
  writeCmd(Idle, 0x0E);
  readCmd();

  sdata[0] = 0x03;
  sdata[1] = 0xA1;
  sdata[2] = 0x00;
  sdata[3] = 0xF8;
  sdata[4] = 0x01;
  sdata[5] = 0x18;
  sdata[6] = 0x00;
  sdata[7] = 0x20;
  sdata[8] = 0x60;
  sdata[9] = 0x60;
  sdata[10] = 0x00;
  sdata[11] = 0x74;
  sdata[12] = 0x3F;
  sdata[13] = 0x01;
  writeCmd(Idle, 0x0E);
  readCmd();

  sdata[0] = 0x03;
  sdata[1] = 0xA1;
  sdata[2] = 0x00;
  sdata[3] = 0xF8;
  sdata[4] = 0x01;
  sdata[5] = 0x18;
  sdata[6] = 0x00;
  sdata[7] = 0x20;
  sdata[8] = 0x60;
  sdata[9] = 0x60;
  sdata[10] = 0x00;
  sdata[11] = 0x70;
  sdata[12] = 0x3F;
  sdata[13] = 0x01;
  writeCmd(Idle, 0x0E);
  readCmd();
  TFT_Write_Text("Completed.", 190, 100);
}

// Get CR95HF chip ID
void ReadCR95HF_ID() {
  writeCmd(IDN, 0);
  readCmd();
  for(j=0; j<dataNum; j++){
    CR95HF_ID[j] = rdata[j];
  }
  TFT_Write_Text("CR95HF Device ID:", 55, 80);
  TFT_Write_Text(CR95HF_ID, 175, 80);
}

// Select the RF communication protocol (ISO/IEC 14443-A)
void Select_ISO_IEC_14443_A_Protocol() {
  sdata[0] = 0x02;
  sdata[1] = 0x00;
  writeCmd(ProtocolSelect, 2);
  readCmd();

  // Clear read and write buffers
  for(j=0; j<18; j++ ){
    rdata[j] = 0;
    sdata[j] = 0;
  }
}

// Select the RF communication protocol (ISO/IEC 18092)
void Select_ISO_IEC_18092_Protocol() {
  sdata[0] = 0x04;
  sdata[1] = 0x51;
  writeCmd(ProtocolSelect, 2);
  readCmd();

  // Clear read and write buffers
  for(j=0; j<18; j++ ){
    rdata[j] = 0;
    sdata[j] = 0;
  }
}

// Configure IndexMod & Gain
void IndexMod_Gain() {
  sdata[0] = 0x09;
  sdata[1] = 0x04;
  sdata[2] = 0x68;
  sdata[3] = 0x01;
  sdata[4] = 0x01;
  sdata[5] = 0x50;
  writeCmd(WrReg, 6);
  readCmd();
}

// Configure Auto FDet
void AutoFDet() {
  sdata[0] = 0x09;
  sdata[1] = 0x04;
  sdata[2] = 0x0A;
  sdata[3] = 0x01;
  sdata[4] = 0x02;
  sdata[5] = 0xA1;
  writeCmd(WrReg, 6);
  readCmd();
}

// Read the tag ID
void GetTagID() {
  sdata[0] = 0x26;
  sdata[1] = 0x07;
  writeCmd(SendRecv , 2);
  readCmd();

  sdata[0] = 0x93;
  sdata[1] = 0x20;
  sdata[2] = 0x08;
  writeCmd(SendRecv , 3);
  readCmd();

  if(res == 0x80) {
    for(j=1; j<dataNum-3; j++) {
      ByteToHex(rdata[j], txt_hex);
      strcat(ID, txt_hex);
    }
    TAG_flag = 1;
    x_pos = 130;
  }
  else {
    TAG_flag = 0;
    x_pos = 18;
    Select_ISO_IEC_18092_Protocol();
  }
}

// Read the NFC Forum tags
void GetNFCTag() {
  sdata[0] = 0x00;
  sdata[1] = 0xFF;
  sdata[2] = 0xFF;
  sdata[3] = 0x00;
  sdata[4] = 0x00;
  writeCmd(SendRecv, 5);
  readCmd();
  
  if(res == 0x80){
    for(j=0; j<dataNum; j++){
      ByteToHex(rdata[j], txt_hex);
      strcat(ID, txt_hex);
    }
    NFC_flag = 1;
    x_pos = 18;
  }
  else {
    NFC_flag = 0;
    x_pos = 130;
    Select_ISO_IEC_14443_A_Protocol();
  }
}

void DrawFrame(){
  TFT_Set_Pen(CL_BLACK, 1);
  TFT_Line(20, 220, 300, 220);
  TFT_Line(20,  46, 300,  46);
  TFT_Set_Font(&HandelGothic_BT21x22_Regular, CL_RED, FO_HORIZONTAL);
  TFT_Write_Text("RFid  Click  Board  Demo", 55, 14);
  TFT_Set_Font(&Verdana12x13_Regular, CL_BLACK, FO_HORIZONTAL);
  TFT_Write_Text("EasyMx PRO v7 for STM32", 19, 223);
  TFT_Set_Font(&Verdana12x13_Regular, CL_RED, FO_HORIZONTAL);
  TFT_Write_Text("www.mikroe.com", 200, 223);
  TFT_Set_Font(&TFT_defaultFont, CL_BLACK, FO_HORIZONTAL);
}

void main() {
  Init();                     // Initialize MCU and peripherals
  DrawFrame();
  while (!EchoResponse()) {   // Until CR95HF is detected
    IRQ_IN = 1;               //   put IRQ_IN pin at low level
    delay_ms(1);
    IRQ_IN = 0;               //   put IRQ_IN pin at low level
    delay_ms(1);
  }
  TFT_Write_Text("CR95HF Device Connected.", 55, 60);
  
  ReadCR95HF_ID();
  Calibration();
  IndexMod_Gain();
  AutoFDet();
  Select_ISO_IEC_18092_Protocol();

  while(1){
    if (!TAG_Flag)
      GetNFCTag();                                               // Get NFC ID
    
    if (!NFC_Flag)
      GetTagID();                                                // Get Tag ID
    
    if (ID[0] == 0){                                             // If there is no tag present
      flag++;                                                    // Increment counter flag
    }
    else {                                                       // If tag is present
      flag = 0;                                                  // Reset counter flag
      if (strcmp(ID_old, ID)){                                   // Compare previous and current tag ID
        TFT_Set_Font(TFT_defaultFont, CL_WHITE, FO_HORIZONTAL);  // Set font color to white
        TFT_Write_Text(ID_old, x_pos_old, 180);                  // Delete previous tag ID
        TFT_Set_Font(TFT_defaultFont, CL_RED, FO_HORIZONTAL);    // Set font color to red
        TFT_Write_Text("Tag ID :", 130, 160);
        TFT_Write_Text(ID, x_pos, 180);                          // Display current tag ID
        TFT_Set_Font(TFT_defaultFont, CL_WHITE, FO_HORIZONTAL);  // Set font color to white
        TFT_Write_Text("Put the RFid Tag over the RFid Click Antenna", 25, 140);
        strcpy(ID_old, ID);                                      // Set current ID as previous ID
        Delay_ms(2000);
        x_pos_old = x_pos;
      }
    }
    if(flag > 5){                                                // If counter flag has reached value > 5
      TFT_Set_Font(TFT_defaultFont, CL_BLACK, FO_HORIZONTAL);    // Set font color to black
      TFT_Write_Text("Put the RFid Tag over the RFid Click Antenna", 25, 140);
      TFT_Set_Font(TFT_defaultFont, CL_WHITE, FO_HORIZONTAL);    // Set font color to white
      TFT_Write_Text("Tag ID :", 130, 160);
      TFT_Write_Text(ID_old, x_pos_old, 180);                    // Delete previous tag ID
      ID_old[0]= 0;                                              // Terminate the old ID string
      flag = 0;                                                  // Reset counter flag
    }
    ID[0]= 0;                                                    // Terminate the ID string
    
    // Clear read and write buffers
    for(j=0; j<18; j++){
      rdata[j] = 0;
      sdata[j] = 0;
    }
  }
}