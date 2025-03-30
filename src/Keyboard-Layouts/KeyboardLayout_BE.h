/*
 * French-Belgian keyboard layout.
 */

#include "KeyboardLayout.h"

#pragma once

//================================================================================
//================================================================================
//  Keyboard

// fr_BE keys ??
#define KEY_SUPERSCRIPT_TWO (136+0x35)
#define KEY_E_ACUTE         (136+0x1f)
#define KEY_E_GRAVE         (136+0x24)
#define KEY_C_CEDILLA       (136+0x26)
#define KEY_A_GRAVE         (136+0x27)
#define KEY_U_GRAVE         (136+0x34)
#define KEY_SECTION         (136+0x23)
#define KEY_MICRO           (136+0x31)
#define KEY_EURO            (136+0x08)

#define AMOUNT_OF_SPECIAL_CHARS 11

extern const char16_t KeyboardLayout_fr_BE_special_characters[AMOUNT_OF_SPECIAL_CHARS] PROGMEM = {
  u'²',
  u'³',
  u'é',
  u'è',
  u'ç',
  u'à',
  u'ù',
  u'§',
  u'µ',
  u'£',
  u'€',
};

extern const uint16_t KeyboardLayout_fr_BE_special_keycodes[AMOUNT_OF_SPECIAL_CHARS] PROGMEM = {
  KEY_SUPERSCRIPT_TWO,
  KEY_SUPERSCRIPT_TWO | U16SHIFT,
  KEY_E_ACUTE,
  KEY_E_GRAVE,
  KEY_C_CEDILLA,
  KEY_A_GRAVE,
  KEY_U_GRAVE,
  KEY_SECTION,
  KEY_MICRO,
  KEY_MICRO | U16SHIFT,
  KEY_EURO | U16SHIFT,
};

extern const uint8_t KeyboardLayout_fr_BE[128] PROGMEM =
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
  0x25,          // !
  0x20,          // "
  0x20|ALT_GR,   // #
  0x30,          // $
  0x34|SHIFT,    // %
  0x1E,          // &
  0x21,          // '
  0x22,          // (
  0x2d,          // )
  0x30|SHIFT,    // *
  0x38|SHIFT,    // +
  0x10,          // ,
  0x2e,          // -
  0x36|SHIFT,    // .
  0x37|SHIFT,    // /
  0x27|SHIFT,    // 0
  0x1e|SHIFT,    // 1
  0x1f|SHIFT,    // 2
  0x20|SHIFT,    // 3
  0x21|SHIFT,    // 4
  0x22|SHIFT,    // 5
  0x23|SHIFT,    // 6
  0x24|SHIFT,    // 7
  0x25|SHIFT,    // 8
  0x26|SHIFT,    // 9
  0x37,          // :
  0x36,          // ;
  0x32,          // <
  0x38,          // =
  0x32|SHIFT,    // >
  0x10|SHIFT,    // ?
  0x1f|ALT_GR,   // @
  0x14|SHIFT,    // A
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
  0x33|SHIFT,    // M
  0x11|SHIFT,    // N
  0x12|SHIFT,    // O
  0x13|SHIFT,    // P
  0x04|SHIFT,    // Q
  0x15|SHIFT,    // R
  0x16|SHIFT,    // S
  0x17|SHIFT,    // T
  0x18|SHIFT,    // U
  0x19|SHIFT,    // V
  0x1d|SHIFT,    // W
  0x1b|SHIFT,    // X
  0x1c|SHIFT,    // Y
  0x1a|SHIFT,    // Z
  0x2f|ALT_GR,   // [
  0x32|ALT_GR,   // bslash
  0x30|ALT_GR,   // ]
  0x23|ALT_GR,   // ^
  0x2e|SHIFT,    // _
  0x31|ALT_GR,   // `
  0x14,          // a
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
  0x33,          // m
  0x11,          // n
  0x12,          // o
  0x13,          // p
  0x04,          // q
  0x15,          // r
  0x16,          // s
  0x17,          // t
  0x18,          // u
  0x19,          // v
  0x1d,          // w
  0x1b,          // x
  0x1c,          // y
  0x1a,          // z
  0x26|ALT_GR,   // {
  0x1e|ALT_GR,   // |
  0x27|ALT_GR,   // }
  0x38|ALT_GR,   // ~
  0x00           // DEL
};

class KeyboardLayout_BE : public KeyboardLayout
{
public:
  KeyboardLayout_BE()
    {
    }
  ~KeyboardLayout_BE(){};
  const uint8_t *getKeymap()
  {
    return KeyboardLayout_fr_BE;
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
      return KeyboardLayout_fr_BE[key];
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
  Serial.println((char16_t)KeyboardLayout_fr_BE_special_characters[i]);
  
  if (key == KeyboardLayout_fr_BE_special_characters[i])
    {
      Serial.println("Compare keys");
      Serial.println(key);
      Serial.println((char16_t)KeyboardLayout_fr_BE_special_characters[i]);
      Serial.println((char)key);
      Serial.println((char)KeyboardLayout_fr_BE_special_characters[i]);
      
      
      keycode = KeyboardLayout_fr_BE_special_keycodes[i];
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
