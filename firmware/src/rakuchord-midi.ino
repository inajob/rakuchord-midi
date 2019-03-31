#include <MIDI.h>
#include <font.h>
#include <Wire.h>

#define ADDRESS_OLED 0x3C

byte trigger[] = {
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0
};
volatile char magic[] = {
0 , 2, 4, 5, 7, 9,11,
12,14,16,17,19,21,23,
24,26,28,29,31,33,35
};
MIDI_CREATE_DEFAULT_INSTANCE();

void lcdSetup(){
  Wire.begin();
  Wire.setClock(400000L);
  delay(100);
    
  Wire.beginTransmission(ADDRESS_OLED);
  Wire.write(0b10000000); //control byte, Co bit = 1, D/C# = 0 (command)
  Wire.write(0xAE); //display off
  Wire.write(0b00000000); //control byte, Co bit = 0, D/C# = 0 (command)
  Wire.write(0xA8); //Set Multiplex Ratio  0xA8, 0x3F
  Wire.write(0b00111111); //64MUX
  Wire.write(0b00000000); //control byte, Co bit = 0, D/C# = 0 (command)
  Wire.write(0xD3); //Set Display Offset 0xD3, 0x00
  Wire.write(0x00);
  Wire.write(0b10000000); //control byte, Co bit = 1, D/C# = 0 (command)
  Wire.write(0x40); //Set Display Start Line 0x40
  Wire.write(0b10000000); //control byte, Co bit = 1, D/C# = 0 (command)
  Wire.write(0xA1); //Set Segment re-map 0xA0/0xA1
  Wire.write(0b10000000); //control byte, Co bit = 1, D/C# = 0 (command)
  Wire.write(0xC8); //Set COM Output Scan Direction 0xC0,/0xC8
  Wire.write(0b00000000); //control byte, Co bit = 0, D/C# = 0 (command)
  Wire.write(0xDA); //Set COM Pins hardware configuration 0xDA, 0x02
  Wire.write(0b00010010);
  Wire.write(0b00000000); //control byte, Co bit = 0, D/C# = 0 (command)
  Wire.write(0x81); //Set Contrast Control 0x81, default=0x7F
  Wire.write(255); //0-255
  Wire.write(0b10000000); //control byte, Co bit = 1, D/C# = 0 (command)
  Wire.write(0xA4); //Disable Entire Display On
  Wire.write(0b00000000); //control byte, Co bit = 0, D/C# = 0 (command)
  Wire.write(0xA6); //Set Normal Display 0xA6, Inverse display 0xA7
  Wire.write(0b00000000); //control byte, Co bit = 0, D/C# = 0 (command)
  Wire.write(0xD5); //Set Display Clock Divide Ratio/Oscillator Frequency 0xD5, 0x80
  Wire.write(0b10000000);
  Wire.write(0b00000000); //control byte, Co bit = 0, D/C# = 0 (command)
  Wire.write(0x20); //Set Memory Addressing Mode
  Wire.write(0x10); //Page addressing mode
  Wire.endTransmission();
  Wire.beginTransmission(ADDRESS_OLED);
  Wire.write(0b00000000); //control byte, Co bit = 0, D/C# = 0 (command)
  Wire.write(0x22); //Set Page Address
  Wire.write(0); //Start page set
  Wire.write(7); //End page set
  Wire.write(0b00000000); //control byte, Co bit = 0, D/C# = 0 (command)
  Wire.write(0x21); //set Column Address
  Wire.write(0); //Column Start Address
  Wire.write(127); //Column Stop Address
  Wire.write(0b00000000); //control byte, Co bit = 0, D/C# = 0 (command)
  Wire.write(0x8D); //Set Enable charge pump regulator 0x8D, 0x14
  Wire.write(0x14);
  Wire.write(0b10000000); //control byte, Co bit = 1, D/C# = 0 (command)
  Wire.write(0xAF); //Display On 0xAF
  Wire.endTransmission();

  delay(10);
}

void lcdClear(){
  Wire.beginTransmission(ADDRESS_OLED);
  Wire.write(0b10000000); //control byte, Co bit = 1, D/C# = 0 (command)
  Wire.write(0xB0); //set page start address
  Wire.write(0b00000000); //control byte, Co bit = 0, D/C# = 0 (command)
  Wire.write(0x21); //set Column Address
  Wire.write(0); //Column Start Address
  Wire.write(127); //Column Stop Address
  Wire.endTransmission();

  for(int k = 0; k < 8; k ++){
    Wire.beginTransmission(ADDRESS_OLED);
    Wire.write(0b10000000); //control byte, Co bit = 1, D/C# = 0 (command)
    Wire.write(0xB0 + k); //set page start address(B0～B7)
    Wire.endTransmission();
    for(int j = 0; j < 8*2; j ++){
      Wire.beginTransmission(ADDRESS_OLED);
      Wire.write(0b01000000);
      for(int i=0; i<8; i++){
        Wire.write(0x00);
      }
      Wire.endTransmission();
    }
  }
}
void drawText(char* buf, byte y){
  Wire.beginTransmission(ADDRESS_OLED);
  Wire.write(0b10000000); //control byte, Co bit = 1, D/C# = 0 (command)
  Wire.write(0xB0 + y); //set page start address(B0～B7)
  Wire.write(0b00000000); //control byte, Co bit = 0, D/C# = 0 (command)
  Wire.write(0x21); //set Column Address
  Wire.write(0); //Column Start Address(0～127)
  Wire.write(127); //Column Stop Address
  Wire.endTransmission();

  while(*buf != 0){
    Wire.beginTransmission(ADDRESS_OLED);
    Wire.write(0b01000000);
    for(int i = 0; i < 4; i ++){
      Wire.write(pgm_read_byte_near(&(font[*buf-' '][i])));
    }
    Wire.write(0x00);
    Wire.endTransmission();
    buf++;
  }
}

void setup()
{
  MIDI.begin();
  //Serial.begin(38400);
  Serial.begin(115200);

  lcdSetup();
  lcdClear();
  drawText("RAKUHCORD MIDI",0);

  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  
  pinMode(8, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(14, INPUT);
  pinMode(15, INPUT);
  
  // pull up
  digitalWrite(8, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(15, HIGH);


}
byte program = 0;

#define M_NONE 0
#define M_MAJOR 1
#define M_MINOR 2
#define M_SUS4 3

byte shiftMode = M_NONE;
bool gsetting = false;

int getMiddleMajorTone(byte no){
  switch(shiftMode){
    case M_NONE: return magic[no + 2];
    case M_MAJOR:return magic[no + 2];
    case M_MINOR:return magic[no + 2] - 1;
    case M_SUS4: return magic[no + 2] + 1;
  }
  return 0; //error
}

int getMiddleMinorTone(byte no){
  switch(shiftMode){
    case M_NONE: return magic[no + 2];
    case M_MINOR:return magic[no + 2];
    case M_MAJOR:return magic[no + 2] + 1;
    case M_SUS4: return magic[no + 2] + 2;
  }
  return 0; //error
}

void triggerOn(byte no){
  if(trigger[no] == 0){
    if(gsetting){
      if(no < 14){
        program = no;
      }
      MIDI.sendProgramChange(program, 1);
      MIDI.sendProgramChange(program, 2);
      char buf[32];
      sprintf(buf, " PROGRAM: %d", program);
      drawText(buf,1);
    }else{
      if(no < 14){
        MIDI.sendNoteOn(42 + magic[no], 127, 1);
      }else if(no < 21){
        MIDI.sendNoteOn(42 + 12+ magic[no-14], 127, 2);
        unsigned int middle;
        switch(magic[no -14 + 2] - magic[no-14]){
          case 4: middle = getMiddleMajorTone(no-14);break;
          case 3: middle = getMiddleMinorTone(no-14);break;
        }
        MIDI.sendNoteOn(42 + 12+ middle, 127, 2);
        MIDI.sendNoteOn(42 + 12+ magic[no-14  + 4], 127, 2);
      }else if(no == 21){
        shiftMode = M_SUS4;
      }else if(no == 22){
        shiftMode = M_MAJOR;
      }else if(no == 23){
        shiftMode = M_MINOR;
      }else if(no == 24){
        if(program < 127)program ++;
      }else if(no == 25){
        if(program > 0)program --;
      }else if(no == 26){
      }else if(no == 27){
        gsetting = true;
      }
    }
    trigger[no] = 1;
  }else{
    if(trigger[no] < 255)trigger[no] ++;
  }
}
void triggerOff(byte no){
  if(trigger[no] > 0){
    if(no < 14){
      MIDI.sendNoteOff(42 + magic[no], 0, 1);
    }else if(no < 21){
      MIDI.sendNoteOff(42 +12+ magic[no-14], 0, 2);
        unsigned int middle;
        switch(magic[no -14 + 2] - magic[no -14]){
          case 4: middle = getMiddleMajorTone(no-14);break;
          case 3: middle = getMiddleMinorTone(no-14);break;
        }
        MIDI.sendNoteOff(42 + 12+ middle, 0, 2);

      MIDI.sendNoteOff(42 +12+ magic[no-14 + 4], 0, 2);
    }else{
      shiftMode = M_NONE;
    }
    if(no == 27){
      gsetting = false;
    }

    trigger[no] = 0;
  }
}

byte bcount = 0;
byte bscan = 0;
void loop()
{
  if(bcount == 0){
    // button scan loop
    bscan++;
    if(bscan == 8){
      bscan = 0;
    }
    switch(bscan){
      case 0:
        pinMode(4, INPUT);
        digitalWrite(4, HIGH);
        pinMode(5, INPUT);
        digitalWrite(5, HIGH);
        pinMode(6, INPUT);
        digitalWrite(6, HIGH);
        pinMode(7, OUTPUT);
        digitalWrite(7, LOW);
        break;
      case 1:
        pinMode(4, INPUT);
        digitalWrite(4, HIGH);
        pinMode(5, OUTPUT);
        digitalWrite(5, LOW);
        pinMode(6, INPUT);
        digitalWrite(6, HIGH);
        pinMode(7, INPUT);
        digitalWrite(7, HIGH);
      break;
      case 2:
        pinMode(4, INPUT);
        digitalWrite(4, HIGH);
        pinMode(5, INPUT);
        digitalWrite(5, HIGH);
        pinMode(6, OUTPUT);
        digitalWrite(6, LOW);
        pinMode(7, INPUT);
        digitalWrite(7, HIGH);
      break;
      case 3:
        pinMode(4, OUTPUT);
        digitalWrite(4, LOW);
        pinMode(5, INPUT);
        digitalWrite(5, HIGH);
        pinMode(6, INPUT);
        digitalWrite(6, HIGH);
        pinMode(7, INPUT);
        digitalWrite(7, HIGH);
      break;

    }
  }

  // button read
  boolean b1 = digitalRead(15);
  boolean b2 = digitalRead(14);
  boolean b3 = digitalRead(13);
  boolean b4 = digitalRead(12);
  boolean b5 = digitalRead(11);
  boolean b6 = digitalRead(10);
  boolean b7 = digitalRead(8);

  boolean pushed = false;

  switch(bscan){
    case 0:
      if(!b1){triggerOn(21);}else{triggerOff(21);}
      if(!b2){triggerOn(22);}else{triggerOff(22);}
      if(!b3){triggerOn(23);}else{triggerOff(23);}
      if(!b4){triggerOn(24);}else{triggerOff(24);}
      if(!b5){triggerOn(25);}else{triggerOff(25);}
      if(!b6){triggerOn(26);}else{triggerOff(26);}
      if(!b7){triggerOn(27);}else{triggerOff(27);}
    break;
    case 3:
      if(!b1){triggerOn(0);}else{triggerOff(0);}
      if(!b2){triggerOn(1);}else{triggerOff(1);}
      if(!b3){triggerOn(2);}else{triggerOff(2);}
      if(!b4){triggerOn(3);}else{triggerOff(3);}
      if(!b5){triggerOn(4);}else{triggerOff(4);}
      if(!b6){triggerOn(5);}else{triggerOff(5);}
      if(!b7){triggerOn(6);}else{triggerOff(6);}
    break;
    case 1:
      if(!b1){triggerOn(7);}else{triggerOff(7);}
      if(!b2){triggerOn(8);}else{triggerOff(8);}
      if(!b3){triggerOn(9);}else{triggerOff(9);}
      if(!b4){triggerOn(10);}else{triggerOff(10);}
      if(!b5){triggerOn(11);}else{triggerOff(11);}
      if(!b6){triggerOn(12);}else{triggerOff(12);}
      if(!b7){triggerOn(13);}else{triggerOff(13);}
    break;
    case 2:
      if(!b1){triggerOn(14);pushed = true;}else{triggerOff(14);}
      if(!b2 && pushed == false){triggerOn(15);pushed = true;}else{triggerOff(15);}
      if(!b3 && pushed == false){triggerOn(16);pushed = true;}else{triggerOff(16);}
      if(!b4 && pushed == false){triggerOn(17);pushed = true;}else{triggerOff(17);}
      if(!b5 && pushed == false){triggerOn(18);pushed = true;}else{triggerOff(18);}
      if(!b6 && pushed == false){triggerOn(19);pushed = true;}else{triggerOff(19);}
      if(!b7 && pushed == false){triggerOn(20);pushed = true;}else{triggerOff(20);}
    break;
  }

  // counter management
  bcount = (bcount + 1) & 0xf;

}

