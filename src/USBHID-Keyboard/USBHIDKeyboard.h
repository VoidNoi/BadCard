/*
  Keyboard.h

  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once
#include "Print.h"
#include "./USBHID.h"
#if CONFIG_TINYUSB_HID_ENABLED

#include "esp_event.h"
// Default keyboardlayout
#include "KeyboardLayout.h"
#include "KeyboardLayout_US.h"

ESP_EVENT_DECLARE_BASE(ARDUINO_USB_HID_KEYBOARD_EVENTS);

typedef enum {
    ARDUINO_USB_HID_KEYBOARD_ANY_EVENT = ESP_EVENT_ANY_ID,
    ARDUINO_USB_HID_KEYBOARD_LED_EVENT = 0,
    ARDUINO_USB_HID_KEYBOARD_MAX_EVENT,
} arduino_usb_hid_keyboard_event_t;

typedef union {
    struct {
            uint8_t numlock:1;
            uint8_t capslock:1;
            uint8_t scrolllock:1;
            uint8_t compose:1;
            uint8_t kana:1;
            uint8_t reserved:3;
    };
    uint8_t leds;
} arduino_usb_hid_keyboard_event_data_t;

#define LED_NUMLOCK     0x01
#define LED_CAPSLOCK    0x02
#define LED_SCROLLLOCK  0x04
#define LED_COMPOSE     0x08
#define LED_KANA        0x10

//  Low level key report: up to 6 keys and shift, ctrl etc at once
typedef struct
{
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport;

class USBHIDKeyboard: public USBHIDDevice, public Print
{
private:
    USBHID hid;
    KeyReport _keyReport;
    // contains the used keymap
    const uint8_t *_asciimap;
    KeyboardLayout *keyboardlayout;
public:
    USBHIDKeyboard(void);
    void begin(KeyboardLayout* layout=nullptr);
    void end(void);
    void setModifier(uint8_t modifier);
    void unsetModifier(uint8_t modifier);
    size_t write(uint8_t k);
    size_t write(const uint8_t *buffer, size_t size);
    size_t write(uint16_t k);
    size_t write(const uint16_t *buffer, size_t size);
    size_t write(std::u16string text);
    size_t press(uint8_t k);
    size_t release(uint8_t k);
    size_t press(uint16_t k);
    size_t release(uint16_t k);
    void releaseAll(void);
    void sendReport(KeyReport* keys);

    //raw functions work with TinyUSB's HID_KEY_* macros
    size_t pressRaw(uint8_t k);
    size_t releaseRaw(uint8_t k);

    void onEvent(esp_event_handler_t callback);
    void onEvent(arduino_usb_hid_keyboard_event_t event, esp_event_handler_t callback);

    // internal use
    uint16_t _onGetDescriptor(uint8_t* buffer);
    void _onOutput(uint8_t report_id, const uint8_t* buffer, uint16_t len);
};



#endif