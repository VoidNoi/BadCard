/*
 * Portuguese keyboard layout.
 */

#include "KeyboardLayout.h"

#pragma once

//================================================================================
//================================================================================
//  Keyboard

// pt_PT keys
#define KEY_LEFT_GUILLEMET       (136+0x2e)
#define KEY_ACUTE                (136+0x30)
#define KEY_C_CEDILLA            (136+0x33)
#define KEY_MASCULINE_ORDINAL    (136+0x34)
#define KEY_TILDE                (136+0x31)

#define AMOUNT_OF_SPECIAL_CHARS 10

extern const char16_t KeyboardLayout_pt_PT_special_characters[AMOUNT_OF_SPECIAL_CHARS] PROGMEM = {
  u'«',
  u'º',
  u'´',
  u'ç',
  u'~',
  u'»',
  u'ª',
  u'^',
  u'`',
  u'Ç',
};

extern const uint16_t KeyboardLayout_pt_PT_special_keycodes[AMOUNT_OF_SPECIAL_CHARS] PROGMEM = {
  KEY_LEFT_GUILLEMET,
  KEY_MASCULINE_ORDINAL,
  KEY_ACUTE,
  KEY_C_CEDILLA,
  KEY_TILDE,
  KEY_LEFT_GUILLEMET | U16SHIFT,
  KEY_MASCULINE_ORDINAL | U16SHIFT,
  KEY_TILDE | U16SHIFT,
  KEY_ACUTE | U16SHIFT,
  KEY_C_CEDILLA | U16SHIFT,
};

extern const uint8_t KeyboardLayout_pt_PT[128] PROGMEM =
{
  0x00,          // NUL
  0x00,          // SOH
  0x00,          // STX
  0x00,          // ETX
  0x00,          // EOT
  0x00,          // ENQ
  0x00,          // ACK
  0x00,          // BEL
  0x2a,          // BS  Backspace
  0x2b,          // TAB Tab
  0x28,          // LF  Enter
  0x00,          // VT
  0x00,          // FF
  0x00,          // CR
  0x00,          // SO
  0x00,          // SI
  0x00,          // DEL
  0x00,          // DC1
  0x00,          // DC2
  0x00,          // DC3
  0x00,          // DC4
  0x00,          // NAK
  0x00,          // SYN
  0x00,          // ETB
  0x00,          // CAN
  0x00,          // EM
  0x00,          // SUB
  0x00,          // ESC
  0x00,          // FS
  0x00,          // GS
  0x00,          // RS
  0x00,          // US
  
  0x2c,          // ' '
  0x1e|SHIFT,    // !
  0x1f|SHIFT,    // "
  0x20|SHIFT,    // #
  0x21|SHIFT,    // $
  0x22|SHIFT,    // %
  0x23|SHIFT,    // &
  0x2d,          // '
  0x25|SHIFT,    // (
  0x26|SHIFT,    // )
  0x2f|SHIFT,    // *
  0x2f,          // +
  0x36,          // ,
  0x38,          // -
  0x37,          // .
  0x24|SHIFT,    // /
  0x27,          // 0
  0x1e,          // 1
  0x1f,          // 2
  0x20,          // 3
  0x21,          // 4
  0x22,          // 5
  0x23,          // 6
  0x24,          // 7
  0x25,          // 8
  0x26,          // 9
  0x37|SHIFT,    // :
  0x36|SHIFT,    // ;
  0x32,          // <
  0x27|SHIFT,    // =
  0x32|SHIFT,    // >
  0x2d|SHIFT,    // ?
  0x1f|ALT_GR,   // @
  0x04|SHIFT,    // A
  0x05|SHIFT,    // B
  0x06|SHIFT,    // C
  0x07|SHIFT,    // D
  0x08|SHIFT,    // E
  0x09|SHIFT,    // F
  0x0a|SHIFT,    // G
  0x0b|SHIFT,    // H
  0x0c|SHIFT,    // I
  0x0d|SHIFT,    // J
  0x0e|SHIFT,    // K
  0x0f|SHIFT,    // L
  0x10|SHIFT,    // M
  0x11|SHIFT,    // N
  0x12|SHIFT,    // O
  0x13|SHIFT,    // P
  0x14|SHIFT,    // Q
  0x15|SHIFT,    // R
  0x16|SHIFT,    // S
  0x17|SHIFT,    // T
  0x18|SHIFT,    // U
  0x19|SHIFT,    // V
  0x1a|SHIFT,    // W
  0x1b|SHIFT,    // X
  0x1c|SHIFT,    // Y
  0x1d|SHIFT,    // Z
  0x25|ALT_GR,   // [
  0x35,          // bslash
  0x26|ALT_GR,   // ]
  0x00,          // ^  not supported (requires dead key + space)
  0x38|SHIFT,    // _
  0x00,          // `  not supported (requires dead key + space)
  0x04,          // a
  0x05,          // b
  0x06,          // c
  0x07,          // d
  0x08,          // e
  0x09,          // f
  0x0a,          // g
  0x0b,          // h
  0x0c,          // i
  0x0d,          // j
  0x0e,          // k
  0x0f,          // l
  0x10,          // m
  0x11,          // n
  0x12,          // o
  0x13,          // p
  0x14,          // q
  0x15,          // r
  0x16,          // s
  0x17,          // t
  0x18,          // u
  0x19,          // v
  0x1a,          // w
  0x1b,          // x
  0x1c,          // y
  0x1d,          // z
  0x24|ALT_GR,   // {
  0x35|SHIFT,    // |
  0x27|ALT_GR,   // }
  0x00,          // ~  not supported (requires dead key + space)
  0x00           // DEL
};

class KeyboardLayout_PT : public KeyboardLayout
{
public:
  KeyboardLayout_PT()
  {
  }
  ~KeyboardLayout_PT(){};
  const uint8_t *getKeymap()
  {
    return KeyboardLayout_pt_PT;
  }
  /**
   * @brief Returns the keycode for the given key
   *
   * @param key Key value as string
   * @return Key code if possible
   */
  uint16_t getKeycode(char16_t key)
  {
    Serial.print("Keycode");
    uint16_t specialKeyCode = isSpecialKey(key);
    
    Serial.println(specialKeyCode);
    // Check if special key is in the map
    if (specialKeyCode != 0)
      {
	return specialKeyCode;
      }
    // No special key use the normal layout
    else
      {
	if (key < 128)
	  {
	    return KeyboardLayout_pt_PT[key];
	  }
	// Could not match code
	return 0;
      }
  }
  
  uint16_t isSpecialKey(char16_t key)
  {
    uint16_t keycode = 0;
    for (int i = 0; i < AMOUNT_OF_SPECIAL_CHARS; i++)
      {
	Serial.print("Searching key: ");
	Serial.println(key);
	Serial.print("Comparing with list: ");
	Serial.println((char16_t)KeyboardLayout_pt_PT_special_characters[i]);
	
	if (key == KeyboardLayout_pt_PT_special_characters[i])
	  {
	    Serial.println("Compare keys");
	    Serial.println(key);
	    Serial.println((char16_t)KeyboardLayout_pt_PT_special_characters[i]);
	    Serial.println((char)key);
	    Serial.println((char)KeyboardLayout_pt_PT_special_characters[i]);
	    
	    
	    keycode = KeyboardLayout_pt_PT_special_keycodes[i];
	    Serial.println(keycode);
	    break;
	  }
      }
    return keycode;
  }

protected:
};

//#endif
//#endif
