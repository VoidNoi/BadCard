/*
 * Hungarian keyboard layout.
 */

#include "KeyboardLayout.h"

#pragma once

//================================================================================
//================================================================================
//  Keyboard

// hu_HU keys
#define KEY_O_ACUTE             (136+0x2e)
#define KEY_O_UMLAUT            (136+0x27)
#define KEY_O_DOUBLE_ACUTE      (136+0x2f)

#define KEY_U_ACUTE             (136+0x30)
#define KEY_U_UMLAUT            (136+0x2d)
#define KEY_U_DOUBLE_ACUTE      (136+0x31)

#define KEY_A_ACUTE             (136+0x34)

#define KEY_E_ACUTE             (136+0x33)

#define KEY_I_ACUTE             (136+0x32)
#define KEY_TWO                 (136+0x1f)
#define KEY_THREE               (136+0x20)
#define KEY_FOUR                (136+0x21)
#define KEY_FIVE                (136+0x22)
#define KEY_SIX                 (136+0x23)
#define KEY_EIGHT               (136+0x25)
#define KEY_NINE                (136+0x26)

#define AMOUNT_OF_SPECIAL_CHARS 30

extern const char16_t KeyboardLayout_hu_HU_special_characters[AMOUNT_OF_SPECIAL_CHARS] PROGMEM = {
  u'ó',
  u'ö',
  u'ő',
  u'ú',
  u'ü',
  u'ű',
  u'á',
  u'é',
  u'í',
  u'Ó',
  u'Ö',
  u'Ő',
  u'Ú',
  u'Ü',
  u'Ű',
  u'Á',
  u'É',
  u'Í',
  u'ˇ',
  u'^',
  u'˘',
  u'°',
  u'˛',
  u'˙',
  u'´',
  u'˝',
  u'¨',
  u'¸',
  u'ß',
  u'¤'
};

extern const uint16_t KeyboardLayout_hu_HU_special_keycodes[AMOUNT_OF_SPECIAL_CHARS] PROGMEM = {
  KEY_O_ACUTE,
  KEY_O_UMLAUT,
  KEY_O_DOUBLE_ACUTE,
  KEY_U_ACUTE,
  KEY_U_UMLAUT,
  KEY_U_DOUBLE_ACUTE,
  KEY_A_ACUTE,
  KEY_E_ACUTE,
  KEY_I_ACUTE,
  KEY_O_ACUTE | U16SHIFT,
  KEY_O_UMLAUT | U16SHIFT,
  KEY_O_DOUBLE_ACUTE | U16SHIFT,
  KEY_U_ACUTE | U16SHIFT,
  KEY_U_UMLAUT | U16SHIFT,
  KEY_U_DOUBLE_ACUTE | U16SHIFT,
  KEY_A_ACUTE | U16SHIFT,
  KEY_E_ACUTE | U16SHIFT,
  KEY_I_ACUTE | U16SHIFT,
  KEY_TWO | ALT_GR,
  KEY_THREE | ALT_GR,
  KEY_FOUR | ALT_GR,
  KEY_FIVE | ALT_GR,
  KEY_SIX | ALT_GR,
  KEY_EIGHT | ALT_GR,
  KEY_NINE | ALT_GR,
  KEY_O_UMLAUT | U16SHIFT,
  KEY_U_UMLAUT | U16SHIFT,
  KEY_O_ACUTE | U16SHIFT,
  KEY_A_ACUTE | ALT_GR,
  KEY_U_DOUBLE_ACUTE | ALT_GR
};

extern const uint8_t KeyboardLayout_hu_HU[128] PROGMEM =
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
  0x21|SHIFT,    // !
  0x1f|SHIFT,    // "
  0x1b|ALT_GR,   // #
  0x33|ALT_GR,   // $
  0x22|SHIFT,    // %
  0x06|ALT_GR,   // &
  0x1e|SHIFT,    // '
  0x25|SHIFT,    // (
  0x26|SHIFT,    // )
  0x38|ALT_GR,   // *
  0x20|SHIFT,    // +
  0x36,          // ,
  0x38,          // -
  0x37,          // .
  0x23|SHIFT,    // /
  
  0x35,          // 0
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
  0x36|ALT_GR,   // ;
  0x32|ALT_GR,   // <
  0x24|SHIFT,    // =
  0x1d|ALT_GR,   // >
  0x36|SHIFT,    // ?
  0x19|ALT_GR,   // @
  
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
  0x1d|SHIFT,    // Y
  0x1c|SHIFT,    // Z
  
  0x09|ALT_GR,   // [
  0x14|ALT_GR,   // bslash
  0x0a|ALT_GR,   // ]
  0x20|ALT_GR,   // ^
  0x38|SHIFT,    // _
  0x24|ALT_GR,   // `
  
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
  0x1d,          // y
  0x1c,          // z
  
  0x05|ALT_GR,   // {
  0x1a|ALT_GR,   // |
  0x11|ALT_GR,   // }
  0x1e|ALT_GR,   // ~
  0x00           // DEL
};

class KeyboardLayout_HU : public KeyboardLayout
{
public:
  KeyboardLayout_HU()
  {
  }
  ~KeyboardLayout_HU(){};
  const uint8_t *getKeymap()
  {
    return KeyboardLayout_hu_HU;
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
	    return KeyboardLayout_hu_HU[key];
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
	Serial.println((char16_t)KeyboardLayout_hu_HU_special_characters[i]);
	
	if (key == KeyboardLayout_hu_HU_special_characters[i])
	  {
	    Serial.println("Compare keys");
	    Serial.println(key);
	    Serial.println((char16_t)KeyboardLayout_hu_HU_special_characters[i]);
	    Serial.println((char)key);
	    Serial.println((char)KeyboardLayout_hu_HU_special_characters[i]);
	    
	    
	    keycode = KeyboardLayout_hu_HU_special_keycodes[i];
	    Serial.println(keycode);
	    break;
	  }
      }
    return keycode;
  }

protected:
};
