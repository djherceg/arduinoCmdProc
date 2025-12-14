#include <Arduino.h>
#include <cmdProc.h>

uint8_t buf[100];

CmdProc::Proc cmdProc; // komandni procesor
const char infoText[] PROGMEM{"info"};
const char queryText[] PROGMEM{"query"};
const char setText[] PROGMEM{"set"};
const char verText[] PROGMEM{"ver"};
const char helpText[] PROGMEM{"help"};
const char resetText[] PROGMEM{"reset"};

// forward deklaracije komandi
int cmdVer(CmdProc::Proc &c);
int cmdInfo(CmdProc::Proc &c);
int cmdQuery(CmdProc::Proc &c);
int cmdSet(CmdProc::Proc &c);
int cmdHelp(CmdProc::Proc &c);
int cmdReset(CmdProc::Proc &c);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  // initialize the command processor
  cmdProc.Init(6);
  cmdProc.Add(verText, cmdVer, 1, 1);
  cmdProc.Add(infoText, cmdInfo, 1, 2);
  cmdProc.Add(queryText, cmdQuery, 2, 2);
  cmdProc.Add(setText, cmdSet, 3, 3);
  cmdProc.Add(helpText, cmdHelp, 1, 1);
  cmdProc.Add(resetText, cmdReset, 1, 1);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    // Serial.println(sbuf.buffer);
    String sbuf = Serial.readStringUntil('\n');
    int rez = cmdProc.Parse(sbuf);
    Serial.print(F("Result: "));
    Serial.println(rez);
  }
}

int cmdVer(CmdProc::Proc &c)
{
  Serial.println(F("CmdProc version 0.2.1\nBasicExample for AVR"));
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
  return CMD_ERR_GENERALERROR;
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
  return CMD_ERR_GENERALERROR;
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
  return CMD_ERR_INVALIDVALUE;
}

int cmdHelp(CmdProc::Proc &c)
{
  Serial.println(F("Available commands:"));
  Serial.println(F("  ver"));
  Serial.println(F("  info <pin> [detail]"));
  Serial.println(F("  query <pin>"));
  Serial.println(F("  set <pin> <value>"));
  Serial.println(F("  help"));
  return 0;
}

int cmdReset(CmdProc::Proc &c)
{
  return 0;
}
