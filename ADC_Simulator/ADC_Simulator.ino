#define SCK 2
#define DO 3
#define CS1 4
#define CS2 5
#define CS3 6
#define LCDO 8
#define LCOM 9
#define LoadcellPin A0
#define TempPin1 A1
#define TempPin2 A2
#define TempPin3 A3

int streamdata[32][3];
long loadcelldataInit = 1000000;
long loadcelldata;
int counter = 0;

int ipow(int base, int ex) // Power function for integers
{
    int base_o = base; // storing original base value
    while(ex > 1) // multiplies base by itself while exponent is greater than one; if exponent equals one the function will return the base
    {
        base *= base_o;
        ex--;
    }
    if(ex == 0) // returns 1 if exponent is zero (x^0 = 1 for all x)
    {
        return 1;
    }
    return base;
}

void gettempdata()
{
  for(int i = 0; i < 32; i++)
  {
    streamdata[i][0] = 0;
    streamdata[i][1] = 0;
    streamdata[i][2] = 0;
  }
  int tempdata[3] = {analogRead(TempPin1)/8,analogRead(TempPin2)/8,analogRead(TempPin3)};
  for(int j = 0; j < 3; j++)
  {
      for(int i = 9; i >= 0; i--)
      {
        if(tempdata[j]%ipow(2,i+1) > 0 && ipow(2,i) <= tempdata[j])
        {
          streamdata[20+i][j] = 1;
          tempdata[j] = tempdata[j]%ipow(2,i);
        }
        else
        {
          streamdata[20+i][j] = 0;
        }
      }
  }
}

void sendtempdata()
{
  int i = 31;
  while(digitalRead(CS1) == LOW && i >= 0)
  {
    delay(1);
    if(digitalRead(SCK) == LOW)
    {
      digitalWrite(DO, streamdata[i][0]);
      i--;
      delay(1);
    }
  }
  
  int j = 31;
  while(digitalRead(CS2) == LOW && j >= 0)
  {
    delay(1);
    if(digitalRead(SCK) == LOW)
    {
      digitalWrite(DO, streamdata[j][1]);
      j--;
      delay(1);
    }
  }
  
  int k = 31;
  while(digitalRead(CS3) == LOW && k >= 0)
  {
    delay(1);
    if(digitalRead(SCK) == LOW)
    {
      digitalWrite(DO, streamdata[k][2]);
      k--;
      delay(1);
    }
  }
}

void loadcellInit()
{
  digitalWrite(LCDO, HIGH);
  delay(100);
  digitalWrite(LCDO, LOW);
  shiftOut(LCDO, SCK, MSBFIRST, (loadcelldataInit >> 16));
  shiftOut(LCDO, SCK, MSBFIRST, (loadcelldataInit >> 8));
  shiftOut(LCDO, SCK, MSBFIRST, loadcelldataInit);
}

void sendLoadcellData(long data)
{
    if(LCOM)
    {
      delay(1);
      digitalWrite(LCDO, LOW);
      shiftOut(LCDO, SCK, MSBFIRST, (data >> 16));
      shiftOut(LCDO, SCK, MSBFIRST, (data >> 8));
      shiftOut(LCDO, SCK, MSBFIRST, data);
    }
    else
    {
      digitalWrite(LCDO, HIGH);
    }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(DO, OUTPUT);
  pinMode(LCDO, OUTPUT);
  pinMode(CS1, INPUT);
  pinMode(CS2, INPUT);
  pinMode(CS3, INPUT);
  pinMode(SCK, INPUT);
  pinMode(LoadcellPin, INPUT);
  pinMode(TempPin1, INPUT);
  pinMode(TempPin2, INPUT);
  pinMode(TempPin3, INPUT);
  Serial.begin(9600);


}

void loop() {

  gettempdata();
  /*
  for(int i = 31; i >= 0; i--)
  {
    Serial.print(streamdata[i]);
  }
  Serial.println();
  */
  sendtempdata();

  if(counter < 20)
  {
    loadcellInit();
    counter++;
  }
  
  long loadcelldata = analogRead(LoadcellPin) * 2000000/1023 + 1000000;

  sendLoadcellData(loadcelldata);
  
  
  //delay(250);
  //Serial.println();
  //Serial.println("New Data");
}
