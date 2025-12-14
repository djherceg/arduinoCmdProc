/*
    Serial buffer
    Djordje Herceg, 11.11.2022.

    GitHub: https://github.com/djherceg/serialBuf
*/


#ifndef __SERIALBUF_H
#define __SERIALBUF_H

#include <stdint.h>

#define SERIALBUF_TEXTMODE 0
#define SERIALBUF_BINARYMODE 1

class SerialBuf
{
private:
    static const int maxbuf = 80;
    bool overflow;
    uint32_t mils;
    uint32_t stopInterval = 50;         // interval after the last received characer in BINARYMODE when the reception is considered complete
    bool finished;
    int length;                         // length of contents in buffer 
    int position;                       // current position in buffer
    int mode = SERIALBUF_TEXTMODE;      // BINARYMODE or TEXTMODE (recognize CR+LF or LF as End-of-line)

public:
    /** Initializes the buffer with the 50ms stop interval */
    void init();

    /** Initializes the buffer with the specified stop interval */
    void init(uint32_t stopinterval);

    /** Clears the buffer */
    void clear();

    /** Buffer in TEXTMODE. Expects LF to finish reception, ignores CR. */
    void textMode();

    /** Buffer in BINARYMODE. Waits stopInterval milliseconds before reception is considered complete */
    void binaryMode();

    /** Buffer mode: SERIALBUF_TEXTMODE or SERIALBUF_BINARYMODE */
    int getMode();

    /** Must be frequently invoked in loop() */
    void loop();

    /** Vraća karakter na poziciji+offset ili -1 ako ne postoji */
    int peek(uint8_t offset);                 // karakter na position+offset ili -1 ako ne postoji

    /** Vraća karakter na poziciji+1 ili -1 ako ne postoji */
    //int peeknext();             // karakter na position+1 ili -1 ako ne postoji

    /** Da li postoji karakter na poziciji */
    bool iscurrent();           // da li postoji karakter na position

    /** Da li postoji karakter na poziciji+1 */
    bool isnext();              // da li postoji karakter na position+1

    /** Čita jedan karakter iz bafera ili vraća -1 ako nema više */
    int read();                 // čita jedan karakter iz bafera ili vraća -1 ako nema više

    /** Da li bafer sadrži još najmanje n karaktera */
    bool isnextn(int n);        

    /** Da li je bafer prepunjen */
    bool isoverflow();          // da li je bafer prepunjen preko kapaciteta

    /** Da li je bafer završio prijem i sadrži poruku */
    bool isavailable();         // da li je bafer završio prijem i sadrži poruku
    

    char buffer[maxbuf + 1];    // bafer
    
    /** vraća dužinu sadržaja u baferu */
    int getLength();            
    
    /** vraća trenutnu poziciju u baferu */
    int getPosition();
};

#endif