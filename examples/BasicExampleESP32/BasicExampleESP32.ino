#include <Arduino.h>
#include <cmdProc.h>

CmdProc::Proc cmdProc; // command processor instance

// forward declarations of command functions
int cmdVer(CmdProc::Proc &c);
int cmdInfo(CmdProc::Proc &c);
int cmdQuery(CmdProc::Proc &c);
int cmdSet(CmdProc::Proc &c);
int cmdHelp(CmdProc::Proc &c);
int cmdReset(CmdProc::Proc &c);
int cmdRepeatText(CmdProc::Proc &c);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  // initialize the command processor
  cmdProc.Init(7);
  cmdProc.Add("ver", cmdVer, 1, 1);
  cmdProc.Add("info", cmdInfo, 1, 2);
  cmdProc.Add("query", cmdQuery, 2, 2);
  cmdProc.Add("set", cmdSet, 3, 3);
  cmdProc.Add("help", cmdHelp, 1, 1);
  cmdProc.Add("reset", cmdReset, 1, 1);
  cmdProc.Add("repeatText", cmdRepeatText, 2, 3);
  Serial.println(F("BasicExampleESP32 Command Processor ready. Type 'help' for commands."));
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    // Serial.println(sbuf.buffer);
    String sbuf = Serial.readStringUntil('\n');
    Serial.print(F("Input: "));
    sbuf.trim();
    Serial.println(sbuf);
    int rez = cmdProc.Parse(sbuf);
    Serial.print(F("Result: "));
    Serial.println(rez);
  }
}

int cmdVer(CmdProc::Proc &c)
{
  Serial.println("CmdProc version 0.2.1\nBasicExample for ESP32");
  return 0;
}

int cmdInfo(CmdProc::Proc &c)
{
  int pin;
  if (c.TryParseInt(pin))
  {
    Serial.println(pin);
    return 0;
  }
  return CMDPROC_ERR_GENERALERROR;
}

int cmdQuery(CmdProc::Proc &c)
{
  int pin;
  if (c.TryParseInt(pin))
  {
    Serial.print("Querying pin ");
    Serial.println(pin);
    return 0;
  }
  return CMDPROC_ERR_GENERALERROR;
}

int cmdSet(CmdProc::Proc &c)
{
  int pin, i;
  if (c.TryParseInt(pin) && c.TryParseInt(i))
  {
    Serial.print("Setting pin ");
    Serial.print(pin);
    Serial.print(" to ");
    Serial.println(i);
    return 0;
  }
  return CMDPROC_ERR_INVALIDVALUE;
}

int cmdHelp(CmdProc::Proc &c)
{
  Serial.println("Available commands:");
  Serial.println("  ver");
  Serial.println("  info <pin> [detail]");
  Serial.println("  query <pin>");
  Serial.println("  set <pin> <value>");
  Serial.println("  repeatText <text> [count]");
  Serial.println("  help");
  return 0;
}

int cmdReset(CmdProc::Proc &c)
{
  return 0;
}

int cmdRepeatText(CmdProc::Proc &c)
{
  String text;
  int count = 1;
  if (c.TryParseString(text))
  {
    c.TryParseInt(count); // optional parameter
    for (int i = 0; i < count; i++)
    {
      Serial.println(text);
    }
    return 0;
  }
  return CMDPROC_ERR_INVALIDVALUE;
}