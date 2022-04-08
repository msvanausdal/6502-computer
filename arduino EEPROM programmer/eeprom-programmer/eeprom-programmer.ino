#define SHIFT_DATA 3
#define SHIFT_LATCH 2
#define SHIFT_CLK 1
#define CHIP_EN 16
#define WRITE_EN 15
#define OUT_EN 14
#define EEPROM_D0 2
#define EEPROM_D7 9

void eeprom_set_address(int address, bool out_en) {
  digitalWrite(SHIFT_LATCH, LOW);

  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);
  digitalWrite(OUT_EN, out_en?LOW:HIGH);

  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

byte eeprom_read(int address) {
  digitalWrite(WRITE_EN, HIGH);
  digitalWrite(OUT_EN, LOW);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {
    pinMode(pin, INPUT);
  }
  eeprom_set_address(address, true);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin--) {
    data = (data << 1) + digitalRead(pin);
  }
  digitalWrite(OUT_EN, HIGH);
  return data;
}

void eeprom_write(int address, byte data) {
  digitalWrite(WRITE_EN, HIGH);
  digitalWrite(OUT_EN, HIGH);
  eeprom_set_address(address, false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {
    pinMode(pin, OUTPUT);
  }

  for(int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10);
}

void eeprom_write_partial(byte[] data, len) {
  for (int addr = 0; addr <= len; addr++) {
    eeprom_write(addr, data[addr]);
  }
}

void eeprom_write_full(byte[] data) {
  for (int addr = 0; addr <= 2047; addr++) {
    eeprom_write(addr, data[addr]);
  }
}

void eeprom_erase() {
  for(int addr = 0; addr <= 2047; addr++) {
    eeprom_write(addr, 0xff);
  }
}

byte[] serial_read_data(int len) {
  byte[len] data;
  if(Serial.readBytes(data, len) == len) {
    return data;
  }
  else {
    Serial.println("Error: Incorrect read size.");
    return null;
  }
}

void serial_print_eeprom(int first = 0, int last = 2047) {
  digitalWrite(WRITE_EN, HIGH);
  digitalWrite(OUT_EN, LOW);
  for (int base = first; base <= last; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset++) {
      data[offset] = read_eeprom(base + offset);
    }

    char buf[80];
    sprintf(buf, "%04x: %02x %02x %02x %02x %02x %02x %02x %02x    %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);
    Serial.println(buf);
  }
  digitalWrite(OUT_EN, HIGH);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  pinMode(WRITE_EN, OUTPUT);
  pinMode(OUT_EN, OUTPUT);
  pinMode(CHIP_EN, OUTPUT);
  digitalWrite(CHIP_EN, LOW);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte input;
  if(readBytes(input, 1) == 1) {
    if(input == 0x01) {
      byte len;
      if(readBytes(len, 1) == 1) {
        if(len <= 2047) {
          serial_print_eeprom(0, len);
        }
        else {
          Serial.println("Error: Invald print length.");
        }
      }
      else {
        Serial.println("Error: Invalid print length.");
      }
    }
    else if(input == 0x02) {
      byte len;
      if(readBytes(len, 1) == 1) {
        if(len <= 2047) {
          byte[len] data;
          if(readBytes(data, len) == len) {
            write_eeprom_partial(data, len);
          }
          else {
            Serial.println("Error: Invalid read data.");
          }
        }
        else {
          Serial.println("Error: Invalid read length.");
        }
      }
      else {
        Serial.println("Error: Invalid read length.");
      }
    }
    else if(input == 0xFF) {
      eeprom_erase();
    }
    else if(input != 0x00) {
      Serial.println("Error: Invalid command %02x.", input);
    }
}
