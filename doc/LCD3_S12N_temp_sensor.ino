//M.Sharonov aka obelix662000
//E-bike S12SN controller mod to display motor's temperature
//by inserting corresponding value into communication package

#define ADCREF 4100    //ADC reference, mV, default ADC ref is power supply voltage. Controller's voltage at red wires is ~4.1V
#define SENSOR_COEFF 10 //Sensor coefficient mV/C for LM35 sensor


char SerialInputBuffer[12];
uint16_t pos=0;
uint32_t motorTemp=0;
uint16_t tempADC;
uint8_t crc=0;
void setup()
{
	Serial.begin(9600);
	
}

void loop()
{
	while (Serial.available() > 0) {
		char inChar = Serial.read();
		if (inChar==65) {  //start of sequence
			pos=0;
		};
		SerialInputBuffer[pos]=inChar;
		pos++;
		if (pos>11)
		{
			pos=0;
			if (SerialInputBuffer[0]==65)  //if synchronized
			{
				crc=0;
				SerialInputBuffer[9]=(int8_t)(motorTemp-15);   //negative and positive allowed
				for(int k=1;k<12;k++)
				{
					if (k!=6) crc^=SerialInputBuffer[k];
				};
				SerialInputBuffer[6]=crc;
				for (int j=0;j<12;j++){
					Serial.print(SerialInputBuffer[j]);
				};
				tempADC=analogRead(A0);  //read temp sensor and calculate temperature
				motorTemp=ADCREF/100*tempADC;
				motorTemp=motorTemp*100/SENSOR_COEFF;
				motorTemp=motorTemp/1024;
			}
			
		};

	}
}

