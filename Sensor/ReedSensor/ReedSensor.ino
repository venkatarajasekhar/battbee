
void int_handler()
{
  Serial.println("Int handler");
}

void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, int_handler, CHANGE);
}

void loop()
{
  delay(1000);
}
