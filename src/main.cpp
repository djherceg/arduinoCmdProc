#include <Arduino.h>
#include "serialbuf.h"

#ifndef ENV_ESP32DEV
#include "avr/pgmspace.h"
#endif
#include "cmdProc.h"

SerialBuf sbuf;

uint8_t buf[50];


#ifdef ENV_ESP32DEV
const char sw1name[]{"Switch1"};
const char count1name[]{"Counter1"};
const char rname[]{"Temp1"};
const char lngname[]{"LongCounter"};
const char text1name[]{"Text1"};
#else
const char sw1name[] PROGMEM{"Switch1"};
const char count1name[] PROGMEM{"Counter1"};
const char rname[] PROGMEM{"Temp1"};
const char lngname[] PROGMEM{"LongCounter"};
const char text1name[] PROGMEM{"Text1"};
#endif

CmdProc::Proc cmdProc; // komandni procesor
const char infoCmd[] PROGMEM{"info"};
const char queryCmd[] PROGMEM{"query"};
const char setCmd[] PROGMEM{"set"};
const char verCmd[] PROGMEM{"ver"};
const char startbinCmd[] PROGMEM{"startbin"};
const char resetCmd[] PROGMEM{"reset"};

// forward deklaracije komandi
int cmdVer(CmdProc::Proc *c);
int cmdInfo(CmdProc::Proc *c);
int cmdQuery(CmdProc::Proc *c);
int cmdSet(CmdProc::Proc *c);
int cmdStartbin(CmdProc::Proc *c);
int cmdReset(CmdProc::Proc *c);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  // initialize the SerialBuffer
  sbuf.init();
  sbuf.textMode();

  // initialize the command processor
  cmdProc.Init(6);
  cmdProc.Add(verCmd, cmdVer, 1, 1);
  cmdProc.Add(infoCmd, cmdInfo, 1, 2);
  cmdProc.Add(queryCmd, cmdQuery, 2, 2);
  cmdProc.Add(setCmd, cmdSet, 3, 3);
  cmdProc.Add(startbinCmd, cmdStartbin, 1, 1);
  cmdProc.Add(resetCmd, cmdReset, 1, 1);
}

void loop()
{
  // put your main code here, to run repeatedly:
  sbuf.loop();
  if (sbuf.isavailable())
  {
    // Serial.println(sbuf.buffer);

    if (sbuf.getMode() == SERIALBUF_TEXTMODE)
    {
      int rez = cmdProc.Parse(sbuf.buffer);
      Serial.println(rez);
      Serial.println(F("ready"));
    }
  }
  sbuf.clear();
}

void printStr_P(const char *p)
{
  char *ptr = (char *)malloc(strlen_P(p) + 1);
  if (ptr != NULL)
  { // if memory allocation is successful
    strcpy_P(ptr, p);
    Serial.print(ptr); // function overload
    free(ptr);
  }
}

int cmdVer(CmdProc::Proc *c)
{
  return 0;
}

int cmdInfo(CmdProc::Proc *c)
{
  char *s;
  if ((s = c->GetNextToken()) == nullptr)
  {
    Serial.print(F("cmdInfo"));
    return 0;
  }
  else
  {
    int pin;
    if (CmdProc::tryParseInt(s, pin))
    {
      Serial.println(pin);
      return 0;
    }
  }
  return CMD_ERR_GENERALERROR;
}

int cmdQuery(CmdProc::Proc *c)
{
  char *s;
  s = c->GetNextToken();
  int pin;
  if (CmdProc::tryParseInt(s, pin))
  {
    return 0;
  }
  return CMD_ERR_GENERALERROR;
}

int cmdSet(CmdProc::Proc *c)
{
  int i;
  char *s;
  s = c->GetNextToken();
  int pin;
  if (CmdProc::tryParseInt(s, pin))
  {
    s = c->GetNextToken();
    if (CmdProc::tryParseInt(s, i))
    {
      return 0;
    }
    return CMD_ERR_INVALIDVALUE;
  }
  return CMD_ERR_GENERALERROR;
}

int cmdStartbin(CmdProc::Proc *c)
{
  //sbuf.binaryMode();
  return 0;
}

int cmdReset(CmdProc::Proc *c)
{
  return 0;
}
