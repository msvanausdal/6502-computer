#define SHIFT_DATA 3
#define SHIFT_LATCH 2
#define SHIFT_CLK 1
#define WRITE_EN 15
#define OUT_EN 14
#define EEPROM_D0 2
#define EEPROM_D7 9

void set_address(int address, bool out_en) {
  digitalWrite(SHIFT_LATCH, LOW);

  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8))
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);
  digitalWrite(OUT_EN, out_en?LOW:HIGH);

  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

byte read_eeprom(int address) {
  for (int pin = EEPROM_D0; pin
}

void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_CLK, LOW);
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, 0x55);
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, 0x55);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

}
