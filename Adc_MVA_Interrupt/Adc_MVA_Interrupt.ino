#define NbADCvalues 200

uint16_t adcValueIndex = 0;
volatile uint32_t AnalogData = 0;

uint16_t ADCvalue[NbADCvalues];
volatile uint8_t AdcActive = 1;

uint32_t lastPrint = 0;
uint16_t intervalPrint = 200;

void setup() {
  for (int i = 0; i < NbADCvalues; i++) {
    ADCvalue[i] = 0;
  }
  Serial.begin(115200);
  ADMUX = 0b01000000;
  ADCSRA = 0b10001111;
  ADCSRB = 0b00000000;
  sei();
  ADCSRA |= 0b01000000;
}

void loop() {
  uint32_t Now = millis();

  if ((Now - lastPrint) >= intervalPrint) {
    
    cli();
    AdcActive = 0;
    uint32_t adcCopy = AnalogData;
    sei();
    
    float voltage = adcCopy / NbADCvalues;
    voltage = voltage * 5.0 / 1024;
    delay(1);
    Serial.println(voltage);
    lastPrint = Now;
    AdcActive = 1;
    ADCSRA |= 0b01000000;
  }
}

ISR(ADC_vect) {
  AnalogData -= ADCvalue[adcValueIndex];
  ADCvalue[adcValueIndex] = ADCL | (ADCH << 8);
  AnalogData += ADCvalue[adcValueIndex];
  adcValueIndex++;
  if (adcValueIndex == NbADCvalues) {
    adcValueIndex = 0;
  }
  if (AdcActive == 1) {
    ADCSRA |= 0b01000000;
  }
}

