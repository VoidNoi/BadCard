#include <SD.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
USBHIDKeyboard Keyboard;
#include <SPI.h>

#include "M5Cardputer.h"

#define display M5Cardputer.Display
#define kb M5Cardputer.Keyboard

#define KEY_MENU          0xED
#define KEY_PAUSE         0xD0
#define KEY_NUMLOCK       0xDB
#define KEY_PRINTSCREEN   0xCE
#define KEY_SCROLLLOCK    0xCF
#define KEY_SPACE         0xB4
#define KEY_BACKSPACE     0xB2
#define KEY_DOWN_ARROW    0xD9

File myFile;
String root = "/BadCard";

int fileAmount;

int mainCursor = 0;
int scriptCursor = 0;

const int maxFiles = 50;

String sdFiles[maxFiles] = {"NEW SCRIPT"};

const int ELEMENT_COUNT_MAX = 500;
String fileText[ELEMENT_COUNT_MAX];

int cursorPosX, cursorPosY, screenPosX, screenPosY = 0;

int newFileLines = 0;

int letterHeight = 16.6;
int letterWidth = 12;

String cursor = "|";

String fileName;

bool creatingFile = false;
bool saveFile = false;

void getDirectory() {
  fileAmount = 1;
  File dir = SD.open(root);

  while (true) {
    File entry =  dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }

    String fileName = entry.name();
    sdFiles[fileAmount] = fileName;
    fileAmount++;

    if (fileAmount > maxFiles) {
      display.println("Can't store any more scripts");
    }

  }
  fileAmount--;
}

void printDirectory() {
  for (int i = 0; i <= fileAmount; i++) {
    display.drawString(sdFiles[i+screenPosY], 20, i*20);
  }
}

void handleMenus(int options, void (*executeFunction)(), int& cursor, void (*printMenu)()) {
  cursor = 0;
  while (true) {
    M5Cardputer.update();
    if (kb.isChange()) {
      display.fillScreen(BLACK);
      
      if (kb.isKeyPressed(';') && cursor > 0){
        cursor--;

        if (screenPosY > 0 && cursor > 0) {
          screenPosY--;
        }
      } else if (kb.isKeyPressed('.') && cursor < options) {
        cursor++;

        if (cursor * 20 >= display.height() - 20) {
          screenPosY++;
        }
      }
      int drawCursor = cursor*20;

      if (cursor * 20 > display.height()-20) {
        drawCursor = (display.height() - 20) - 15;
      }

      display.setTextColor(PURPLE);
        
      display.drawString(">", 5, drawCursor);

      printMenu();
      if (kb.isKeyPressed(KEY_ENTER)) {
        screenPosY = 0;
        delay(100);
        executeFunction();
        break;
      }
    }
  }
}

void executeScript() {
  display.fillScreen(BLACK);
  display.setCursor(1,1);
  display.println("Executing function");
  String fileName = root + "/" + sdFiles[mainCursor];

  if (SD.exists(fileName)) {
    Keyboard.begin();
    USB.begin();
    display.println(fileName);

    // Open the file for reading (fill myFile with char buffer)
    myFile = SD.open(fileName, FILE_READ);
    
    // Check if the file has successfully been opened and continue
    if (myFile) {
      // Initialize control over keyboard
      Keyboard.begin();
        
      // Process lines from file with LF EOL (0x0a), not CR+LF (0x0a+0x0d)
      String line = "";
      while (myFile.available()) {  // For each char in buffer
        // Read char from buffer
        char c = myFile.read();
    
        // Process char
        if ((int) c == 0x0a){         // Line ending (LF) reached
          processLine(line);        // Process script line by reading command and payload
          line = "";                // Clean the line to process next
        } else if((int) c != 0x0d) {  // If char isn't a carriage return (CR)
          line += c;                // Put char into line
        }
      }

      // Close the file
      myFile.close();
    } 
      // End control over keyboard
      Keyboard.end();
      mainMenu();
    } 
}

void processLine(String line) {
  /*
   * Process Ducky Script according to the official documentation (see https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Duckyscript).
   *
   * (1) Commands without payload:
   *  - ENTER
   *  - MENU <=> APP
   *  - DOWNARROW <=> DOWN
   *  - LEFTARROW <=> LEFT
   *  - RIGHTARROW <=> RIGHT
   *  - UPARROW <=> UP
   *  - BREAK <=> PAUSE
   *  - CAPSLOCK
   *  - DELETE
   *  - END
   *  - ESC <=> ESCAPE
   *  - HOME
   *  - INSERT
   *  - NUMLOCK
   *  - PAGEUP
   *  - PAGEDOWN
   *  - PRINTSCREEN
   *  - SCROLLLOCK
   *  - SPACE
   *  - TAB
   *  - REPLAY (global commands aren't implemented)
   *
   * (2) Commands with payload:
   *  - DEFAULT_DELAY <=> DEFAULTDELAY (global commands aren't implemented.)
   *  - DELAY (+int)
   *  - STRING (+string)
   *  - GUI <=> WINDOWS (+char)
   *  - SHIFT (+char or key)
   *  - ALT (+char or key)
   *  - CTRL <=> CONTROL (+char or key)
   *  - REM (+string)
   *
   */
  
  int space = line.indexOf(' ');  // Find the first 'space' that'll be used to separate the payload from the command
  String command = "";
  String payload = "";
  
  if (space == -1) {  // There is no space -> (1)
    if (
      line == "ENTER" ||
      line == "MENU" || line == "APP" |
      line == "DOWNARROW" || line == "DOWN" ||
      line == "LEFTARROW" || line == "LEFT" ||
      line == "RIGHTARROW" || line == "RIGHT" ||
      line == "UPARROW" || line == "UP" ||
      line == "BREAK" || line == "PAUSE" ||
      line == "CAPSLOCK" ||
      line == "DELETE" ||
      line == "END" ||
      line == "ESC" || line == "ESCAPE" ||
      line == "HOME" ||
      line == "INSERT" ||
      line == "NUMLOCK" ||
      line == "PAGEUP" ||
      line == "PAGEDOWN" ||
      line == "PRINTSCREEN" ||
      line == "SCROLLLOCK" ||
      line == "SPACE" ||
      line == "TAB"
    ) {
      command = line;
    }
  } else {  // Has a space -> (2)
    command = line.substring(0, space);   // Get chars in line from start to space position
    payload = line.substring(space + 1);  // Get chars in line from after space position to EOL

    if (
      command == "DELAY" ||
      command == "STRING" ||
      command == "GUI" || command == "WINDOWS" ||
      command == "SHIFT" ||
      command == "ALT" ||
      command == "CTRL" || command == "CONTROL" ||
      command == "REM"
    ) { } else {
      // Invalid command
      command = "";
      payload = "";
    }
  }

  if (payload == "" && command != "") {                       // Command from (1)
    processCommand(command);                                // Process command
  } else if (command == "DELAY") {                            // Delay before the next commande
    delay((int) payload.toInt());                           // Convert payload to integer and make pause for 'payload' time
  } else if (command == "STRING") {                           // String processing
    Keyboard.print(payload);                                // Type-in the payload
  } else if (command == "REM") {                              // Comment
  } else if (command != "") {                                 // Command from (2)
    String remaining = line;                                // Prepare commands to run
    while (remaining.length() > 0) {                        // For command in remaining commands
      int space = remaining.indexOf(' ');                 // Find the first 'space' that'll be used to separate commands
      if (space != -1) {                                  // If this isn't the last command
        processCommand(remaining.substring(0, space));  // Process command
        remaining = remaining.substring(space + 1);     // Pop command from remaining commands
      } else {                                            // If this is the last command
        processCommand(remaining);                      // Pop command from remaining commands
        remaining = "";                                 // Clear commands (end of loop)
      }
    } 
  }  else {
    // Invalid command
  }

  Keyboard.releaseAll();
}

void processCommand(String command) {
  /*
   * Process commands by pressing corresponding key
   * (see https://www.arduino.cc/en/Reference/KeyboardModifiers or
   *      http://www.usb.org/developers/hidpage/Hut1_12v2.pdf#page=53)
   */
  if (command.length() == 1) { 
        // Process key (used for example for WIN L command)
    char c = (char) command[0];  // Convert string (1-char length) to char      
    Keyboard.press(c);           // Press the key onkeyboard
  } else if (command == "ENTER") {
    Keyboard.press(KEY_RETURN);
  } else if (command == "MENU" || command == "APP") {
    Keyboard.press(KEY_MENU);
  } else if (command == "DOWNARROW" || command == "DOWN") {
    Keyboard.press(KEY_DOWN_ARROW);
  } else if (command == "LEFTARROW" || command == "LEFT") {
    Keyboard.press(KEY_LEFT_ARROW);
  } else if (command == "RIGHTARROW" || command == "RIGHT") {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else if (command == "UPARROW" || command == "UP") {
    Keyboard.press(KEY_UP_ARROW);
  } else if (command == "BREAK" || command == "PAUSE") {
    Keyboard.press(KEY_PAUSE);
  } else if (command == "CAPSLOCK") {
    Keyboard.press(KEY_CAPS_LOCK);
  } else if (command == "DELETE" || command == "DEL") {
    Keyboard.press(KEY_DELETE);
  } else if (command == "END") {
    Keyboard.press(KEY_END);
  } else if (command == "ESC" || command == "ESCAPE") {
    Keyboard.press(KEY_ESC);
  } else if (command == "HOME") {
    Keyboard.press(KEY_HOME);
  } else if (command == "INSERT") {
    Keyboard.press(KEY_INSERT);
  } else if (command == "NUMLOCK") {
    Keyboard.press(KEY_NUMLOCK);
  } else if (command == "PAGEUP") {
    Keyboard.press(KEY_PAGE_UP);
  } else if (command == "PAGEDOWN") {
    Keyboard.press(KEY_PAGE_DOWN);
  } else if (command == "PRINTSCREEN") {
    Keyboard.press(KEY_PRINTSCREEN);
  } else if (command == "SCROLLLOCK") {
    Keyboard.press(KEY_SCROLLLOCK);
  } else if (command == "SPACE") {
    Keyboard.press(KEY_SPACE);
  } else if (command == "BACKSPACE") {
    Keyboard.press(KEY_BACKSPACE);
  } else if (command == "TAB") {
    Keyboard.press(KEY_TAB);
  } else if (command == "GUI" || command == "WINDOWS") {
    Keyboard.press(KEY_LEFT_GUI);
  } else if (command == "SHIFT") {
    Keyboard.press(KEY_RIGHT_SHIFT);
  } else if (command == "ALT") {
    Keyboard.press(KEY_LEFT_ALT);
  } else if (command == "CTRL" || command == "CONTROL") {
    Keyboard.press(KEY_LEFT_CTRL);
  } else if (command == "F1" || command == "FUNCTION1") {
    Keyboard.press(KEY_F1);
  } else if (command == "F2" || command == "FUNCTION2") {
    Keyboard.press(KEY_F2);
  } else if (command == "F3" || command == "FUNCTION3") {
    Keyboard.press(KEY_F3);
  } else if (command == "F4" || command == "FUNCTION4") {
    Keyboard.press(KEY_F4);
  } else if (command == "F5" || command == "FUNCTION5") {
    Keyboard.press(KEY_F5);
  } else if (command == "F6" || command == "FUNCTION6") {
    Keyboard.press(KEY_F6);
  } else if (command == "F7" || command == "FUNCTION7") {
    Keyboard.press(KEY_F7);
  } else if (command == "F8" || command == "FUNCTION8") {
    Keyboard.press(KEY_F8);
  } else if (command == "F9" || command == "FUNCTION9") {
    Keyboard.press(KEY_F9);
  } else if (command == "F10" || command == "FUNCTION10") {
    Keyboard.press(KEY_F10);
  } else if (command == "F11" || command == "FUNCTION11") {
    Keyboard.press(KEY_F11);
  } else if (command == "F12" || command == "FUNCTION12") {
    Keyboard.press(KEY_F12);
  }
}

void insertLine(String file[], int y){
  for (int i = sizeof(file)-1;i>y;i--){
    file[i] = file[i-1];
  }
  file[y] = '\0';
}

void removeLine(String file[], int y){
  for (int i = y; i < newFileLines; i++) {
    file[i] = file[i+1];
  }
  file[newFileLines] = '\0';
  cursorPosY--;
  newFileLines--;
}

void cleanNewFile() {
  for (int i = 0; i <= newFileLines; i++) {
    fileText[i] = '\0';
  }
  cursorPosX = 0;
  cursorPosY = 0;
  screenPosX = 0;
  screenPosY = 0;
  cursorPosX = 0;
  cursorPosY = 0;
  newFileLines = 0;
}

void saveNewFile() {
  saveFile = false;
  myFile = SD.open(root + "/" + fileName + ".txt", FILE_WRITE);
  if (myFile) {
    for (int i = 0; i <= newFileLines; i++) {
      int textLen = fileText[i].length();
      char charText[textLen];
      strcpy(charText, fileText[i].c_str());

      unsigned char unChar[textLen];

      for (int x = 0; x < textLen; x++) {
        unChar[x] = static_cast<unsigned char>(charText[x]);
      }

      myFile.write(unChar, textLen);
      myFile.print('\n');

    }

    myFile.close();
    mainMenu();
  } else {
    display.println("File didn't open");
    myFile.close();
  }
}

void newFile() {
  fileName = '\0';
  cleanNewFile();
  saveFile = false;
  display.fillScreen(BLACK);
  display.setCursor(1,1);
  creatingFile = true;
}

void deleteScript() {
  String fileName = root + "/" + sdFiles[mainCursor];
  display.fillScreen(BLACK);
  display.setCursor(1,1);
  display.println("Deleting script...");
  if (SD.remove(fileName)) {
    display.println("Script deleted");
    sdFiles[fileAmount] = '\0';
    fileAmount--;
    delay(1000);
  } else {
    display.println("Script couldn't be deleted");
    delay(1000);
  }
  mainMenu();

}

void scriptOptions() {
  if (scriptCursor == 0) {
    executeScript();
  } else if (scriptCursor == 1) {
    deleteScript();
  }
}

void scriptMenu() {
  char optionsList[2][20] = {"Execute script", "Delete script"};
    
  //display.fillScreen(BLACK);
  for (int i = 0; i < 2; i++) {
    display.setCursor(20,i*20);
    display.println(optionsList[i]);
  }
}

void mainOptions() {
  if (mainCursor == 0) {
    newFile(); 
  } else {
    handleMenus(1, &scriptOptions, scriptCursor, &scriptMenu);
  }
}

void mainMenu() {
  display.fillScreen(BLACK);
  display.setTextSize(2);
  display.setCursor(20,1);

  getDirectory();
  handleMenus(fileAmount, &mainOptions, mainCursor, &printDirectory);
}

void bootLogo(){

  display.fillScreen(BLACK);
  
  display.setTextSize(2);
  String BCVersion = "BadCard v1.0.2";
  display.setCursor(display.width()/2-(BCVersion.length()/2)*letterWidth, display.height()/2 - 50);
  display.println(BCVersion);

  display.setTextSize(1);
  display.setCursor(display.width()/2 - 90, display.height()/2 - 25);
  display.println(" __   __     ______     __    ");
  display.setCursor(display.width()/2 - 90, display.height()/2 - 15);
  display.println("/\\\ \"-.\\ \\   /\\  __ \\   /\\ \\   ");
  display.setCursor(display.width()/2 - 90, display.height()/2 - 5);
  display.println("\\ \\ \\-.  \\  \\ \\ \\/\\ \\  \\ \\ \\  ");
  display.setCursor(display.width()/2 - 90, display.height()/2 + 5);
  display.println(" \\ \\_\\\\\"\\_\\  \\ \\_____\\  \\ \\_\\ ");
  display.setCursor(display.width()/2 - 90, display.height()/2 + 15);
  display.println("  \\/_/ \\/_/   \\/_____/   \\/_/ ");

  display.setTextSize(2);
  
  display.setCursor(display.width()/2 - 95, display.height()/2 + 40);
  display.println("Press any key...");

  while(true) {
    M5Cardputer.update();
    if (kb.isChange()) {
      delay(100);
      mainMenu();
      break;
    }
  }
}

void setup() {
  auto cfg = M5.config();
  M5Cardputer.begin(cfg, true);

  SPI.begin(
    M5.getPin(m5::pin_name_t::sd_spi_sclk),
    M5.getPin(m5::pin_name_t::sd_spi_miso),
    M5.getPin(m5::pin_name_t::sd_spi_mosi),
    M5.getPin(m5::pin_name_t::sd_spi_ss));
  while (false == SD.begin(M5.getPin(m5::pin_name_t::sd_spi_ss), SPI)) {
    delay(1);
  }

  if (!SD.exists(root)) {
    SD.mkdir(root);
  }

  display.setRotation(1);
  display.setTextColor(PURPLE);

  bootLogo();
}

void loop() {
  M5Cardputer.update();

  if (saveFile) {
    if (kb.isChange()) {
      if (kb.isPressed()) {
        Keyboard_Class::KeysState status = kb.keysState();

        for (auto i : status.word) {
          fileName += i;
        }

        if (status.del) {
          fileName.remove(fileName.length() - 1);  
        }

        display.fillScreen(BLACK);
        display.setCursor(display.width()/2-5*letterWidth, 0);
        display.println("File Name:");
        display.drawString(fileName, display.width()/2-(fileName.length()/2)*letterWidth, letterHeight);

        if (status.enter) {
          saveNewFile();
        }
      }
    }
  }

  if (creatingFile) {
    if (kb.isChange()) {
      if (kb.isPressed()) {
        Keyboard_Class::KeysState status = kb.keysState();
        
        if (status.fn && kb.isKeyPressed('`')) {
          creatingFile = false;
          saveFile = true;
          
        } else if (status.fn && kb.isKeyPressed(';') && cursorPosY > 0){
          cursorPosY--;

          if (fileText[cursorPosY].length() * letterWidth > display.width()) {
            screenPosX = (fileText[cursorPosY].length() - 19) * -letterWidth;
          } else {
            screenPosX = 0;
          }

          if (screenPosY > 0 && cursorPosY > 0) {
            screenPosY--;
          }

        } else if (status.fn && kb.isKeyPressed('.') && cursorPosY < newFileLines) {
          cursorPosY++;

          if (fileText[cursorPosY].length() * letterWidth > display.width()) {
            screenPosX = (fileText[cursorPosY].length() - 19) * -letterWidth;
          } else {
            screenPosX = 0;
          }

          if (cursorPosY * letterHeight >= display.height() - letterHeight) {
            screenPosY++;
          }

        } else {
          for (auto i : status.word) {

            if ((fileText[cursorPosY].length() + 1) * letterWidth >= display.width()) {
              screenPosX -= letterWidth;
            }
            cursorPosX += 12;

            fileText[cursorPosY] += i;
          }
        }

        if (status.del) {

          if (screenPosX < 0) {
            screenPosX += letterWidth;
          }
          cursorPosX -= 12;

          if (fileText[cursorPosY].length() <= 0 && cursorPosY > 0) {
            removeLine(fileText, cursorPosY);
            if (fileText[cursorPosY].length() * letterWidth > display.width()) {
              screenPosX = (fileText[cursorPosY].length() - 19) * -letterWidth;
            } else {
              screenPosX = 0;
            }
          } else {
            fileText[cursorPosY].remove(fileText[cursorPosY].length()-1);
          }
        }

        if (status.enter) {
          newFileLines++;
          cursorPosY++;
          
          insertLine(fileText, cursorPosY);

          if (cursorPosY * letterHeight >= display.height()-letterHeight) {
            screenPosY++;
          }
          screenPosX = 0;
          cursorPosX = 0;
        }

        display.fillScreen(BLACK);

        display.setTextSize(1.5);

        int drawCursorX = fileText[cursorPosY].length() * letterWidth;
        int drawCursorY = cursorPosY*letterHeight - 5;

        if ((fileText[cursorPosY].length() + 1) * letterWidth >= display.width()) {
          drawCursorX = display.width() - letterWidth;
        }

        if (cursorPosY * letterHeight > display.height()-letterHeight) {
          drawCursorY = (display.height() - letterHeight) - 10;
        }
        display.drawString(cursor, drawCursorX, drawCursorY, &fonts::Font2);
        
        display.setTextSize(2);

        for (int i = 0; i<= newFileLines; i++) {
          display.drawString(fileText[i+screenPosY], screenPosX, i*letterHeight);
        }
      }
    }
  }
} 