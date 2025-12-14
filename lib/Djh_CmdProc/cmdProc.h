#ifndef __CMDPROCV1_H__
#define __CMDPROCV1_H__

#include <Arduino.h>

#if defined(__AVR__)
#include <avr/pgmspace.h>
#endif

#include <string.h>

#define CMD_ERR_GENERALERROR -1
#define CMD_ERR_MISSINGARGUMENT -2
#define CMD_ERR_EXTRAARGUMENT -3
#define CMD_ERR_INVALIDVALUE -4
#define CMD_ERR_VALUEOUTOFRANGE -5
#define CMD_ERR_UNKNOWNCOMMAND -128

namespace CmdProc
{

  class Proc;

  typedef int (*cmdCallback)(Proc &); // tip za callback funkciju

  class Cmd
  {
  public:
    const char *cmdName;
    cmdCallback callback;
    Cmd() {};

    void Set(const char *cmdName, cmdCallback cbk, int minTokens, int maxTokens)
    {
      this->cmdName = cmdName;
      callback = cbk;
      this->minTokens = minTokens;
      this->maxTokens = maxTokens;
    }

    int GetMinTokens()
    {
      return minTokens;
    }

    int GetMaxTokens()
    {
      return maxTokens;
    }

  private:
    int minTokens, maxTokens;
  };

  class Proc
  {
  public:
    void Init(int count)
    {
      this->count = count;
      commands = new Cmd[count];
      current = 0;
    }

    void Add(const char *command, cmdCallback cbk)
    {
      Add(command, cbk, 0, 0);
    }

    void Add(const char *command, cmdCallback cbk, int minTok, int maxTok)
    {
      if (current < count)
      {
        commands[current++].Set(command, cbk, minTok, maxTok);
      }
    }

    int Exec()
    {
      char *pch = GetNextToken();
      if (pch != nullptr)
      {
        // Serial.print(F("pch: >"));
        // Serial.print(pch);
        // Serial.println("<");
        for (int j = 0; j < count; j++)
        {
          if (strCompare(pch, commands[j].cmdName) == 0) // commands[j].cmdName je u PROGMEM za AVR
          {
            int rez = 0;
            int mi = commands[j].GetMinTokens();
            int ma = commands[j].GetMaxTokens();
            if ((mi > 0) && (ma > 0))
            {
              rez = CheckTokenCount(mi, ma);
            }
            if (rez == 0)
              return commands[j].callback(*this); // dereference this pointer to Proc object
            else
              return rez;
          }
        }
      }
      return CMD_ERR_UNKNOWNCOMMAND;
    }

    /// @brief Parses the input string and executes the corresponding command. Maximum input length is 127 characters.
    /// @param s 
    /// @return 
    int Parse(String s)
    {
      char buf[128];   // TODO: make the capacity configurable
      s.toCharArray(buf, sizeof(buf));   // Convert String to char array. This is necessary because Parse modifies the input string.
      return Parse(buf);
    }

    // Parses the string by replacing spaces with null characters (0),
    // ignores multiple spaces,
    // if it encounters a quote (") it ignores all spaces until the next quote (")
    int Parse(char *s)
    {
      tokenCount = 0;
      input = s;
      int i = 0;
      len = strlen(input);
      bool quote = false; // previous char was quote (")
      bool text = false;  // previous char was text
      while (i <= len)
      {
        if ((input[i] == '"'))
        {
          quote = !quote;
          input[i] = 0;
        }
        if ((input[i] == 0) || ((!quote) && (input[i] == ' ')))
        {
          if (text)
          { // count token
            tokenCount++;
          }
          input[i] = 0;
          text = false;
        }
        else
        {
          text = true;
        }
        i++;
      }
      curr = nullptr;
      pos = 0;
      // Serial.print(F("Tokens found: "));
      // Serial.println(tokenCount);
      return Exec();
    }

    char *GetNextToken()
    {
      if (curr != nullptr)
      {
        while ((pos < len) && (input[pos] != 0))
          pos++;
      }
      while ((pos < len) && (input[pos] == 0))
      {
        pos++;
      }
      // Serial.print(F("curr:"));
      if (input[pos] != 0)
      {
        curr = &input[pos];
        // Serial.println(curr);
        return curr;
      }
      else
      {
        // Serial.println(F("<nullptr>"));
        return nullptr;
      }
    }

    int GetTokenCount()
    {
      return tokenCount;
    }

    int CheckTokenCount(int min, int max)
    {
      if (tokenCount < min)
        return CMD_ERR_MISSINGARGUMENT;
      if (tokenCount > max)
        return CMD_ERR_EXTRAARGUMENT;
      return 0;
    }

    // ==== TryParse... methods that get the next token internally ====

    bool TryParseFloat(float &outResult);

    bool TryParseInt32(int32_t &outResult, bool allowMinus = true);

    bool TryParseInt(int &outResult, bool allowMinus = true);

    bool TryParseDec32(int32_t &outResult, bool allowMinus = true);

    bool TryParseDec(int &outResult, bool allowMinus = true);

    bool TryParseHex32(int32_t &outResult);

    bool TryParseTime(int &outHour, int &outMinute, int &outSecond);

    bool TryParseDate(int &outDay, int &outMonth, int &outYear);

  private:
    int count;
    int current;
    Cmd *commands;
    char *input;
    int tokenCount;
    int currentToken;
    char *curr;
    int pos;
    int len; // length of input buffer

    /// @brief Comapares strings, the first is in RAM, the second is in PROGMEM, on AVR. On ESP, both are in RAM.
    /// @param token RAM string
    /// @param cmdName PROGMEM string on AVR, RAM string on ESP32
    /// @return strcmp result
    inline int strCompare(const char *token, const char *cmdName)
    {
#if defined(__AVR__)
      return strcmp_P(token, cmdName);
#else
      return strcmp(token, cmdName);
#endif
    }
  };

  bool tryParseFloat(char *s, float &outResult);

  bool tryParseInt32(char *s, int32_t &outResult, bool allowMinus = false);
  bool tryParseInt(char *s, int &outResult, bool allowMinus = false);

  bool tryParseDec32(char *s, int32_t &outResult, bool allowMinus = false);
  bool tryParseDec(char *s, int &outResult, bool allowMinus = false);

  bool tryParseHex32(char *s, int32_t &outResult);

  bool isHexDigit(char c);

  bool tryParseTime(char *s, int &outHour, int &outMinute, int &outSecond);
  bool tryParseDate(char *s, int &outDay, int &outMonth, int &outYear);
  bool isValidDate(int dd, int mm, int yy);

  // Single global command processor instance
  extern Proc CommandProcessor; // declaration

} // namespace CmdProc

#endif
