

void checkTemperature()
{
  char MsgTemp[10];
  sensors.requestTemperatures();
  float h = (sensors.getTempCByIndex(0));
  dtostrf(h, 4, 2, MsgTemp);
  broadcast("temp", MsgTemp);
}
