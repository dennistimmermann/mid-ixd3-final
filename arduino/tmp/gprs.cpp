//Serial Relay - Arduino will patch a
//serial link between the computer and the GPRS Shield
//at 19200 bps 8-N-1
//Computer is connected to Hardware UART
//GPRS Shield is connected to the Software UART


unsigned char buffer[64]; // buffer array for data recieve over serial port
int count=0;     // counter for buffer array
int state=0;
void setup()
{
  Serial1.begin(19200);               // the GPRS baud rate
  Serial.begin(19200);             // the Serial port of Arduino baud rate.
  pinMode(13, OUTPUT);
}

void loop()
{
  if (Serial1.available())              // if date is comming from softwareserial port ==> data is comming from gprs shield
  {
    while(Serial1.available())          // reading data into char array
    {
      buffer[count++]=Serial1.read();     // writing data into array
      if(count == 64)break;
    }

    Serial.write(buffer,count);            // if no data transmission ends, write buffer to hardware serial port
    clearBufferArray();              // call clearBufferArray function to clear the storaged data from the array
    count = 0;                       // set counter of while loop to zero


  }
  if (Serial.available()) {           // if data is available on hardwareserial port ==> data is comming from PC or notebook
    while(Serial.available()) {
      Serial.read();
    }
    //Serial1.write(Serial.read());       // write it to the GPRS shield
    //digitalWrite(13, HIGH);
    //send SMS

    //set sim pin
    if(state == 0) {
      Serial.println("- setting pin");
      Serial1.print("AT+CPIN=2541\r\n");
    }
    if(state == 1) {
      Serial.println("- setting asciimode");
      Serial1.print("AT+CMGF=1\r\n");
    }
    if(state == 2) {
      Serial.println("- setting isi as recipient");
      Serial1.print("AT+CMGS=\"+17655189321\"\r\n");
    }
    if(state == 3) {
      Serial.println("- appending message");
      Serial1.print("gesendet von unserem GPRS modul :) Sag mir bei FB mal bescheid obs geklappt hat. Dennis");
    }
    if(state == 4) {
      Serial.println("sending");
      Serial1.write((char)26);
    }
    if(state > 4) {
      Serial.println("- nothing more to do");
    }
    state++;
  }
}
void clearBufferArray()              // function to clear buffer array
{
  for (int i=0; i<count;i++)
    { buffer[i]=NULL;}                  // clear all index of array with command NULL
}
