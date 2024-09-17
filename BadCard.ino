#include <SD.h>

#include "src/Unicode/unicode.h"

#include "icons.h"

#include "USB.h"

// https://gitlab.com/DJPX/advanced-keyboard-support-arduino
#include "src/USBHID-Keyboard/USBHIDKeyboard.h"
USBHIDKeyboard Keyboard;
#include "src/USBHID-Keyboard/KeyboardLayout_ES.h"
#include "src/USBHID-Keyboard/KeyboardLayout_DE.h"
#include "src/USBHID-Keyboard/KeyboardLayout_US.h"
#include "src/USBHID-Keyboard/KeyboardLayout_PT.h"
#include "src/USBHID-Keyboard/KeyboardLayout_FR.h"
#include "src/USBHID-Keyboard/KeyboardLayout_SE.h"
#include "src/USBHID-Keyboard/KeyboardLayout_IT.h"
#include "src/USBHID-Keyboard/KeyboardLayout_HU.h"
#include "src/USBHID-Keyboard/KeyboardLayout_DK.h"
#include "src/USBHID-Keyboard/KeyboardLayout_BR.h"
#include "src/USBHID-Keyboard/KeyboardLayout_GB.h"
#include "src/USBHID-Keyboard/KeyboardLayout_NO.h"
#include "src/USBHID-Keyboard/KeyboardLayout_JP.h"

KeyboardLayout *layout;

int currentKBLayout = 0;

// https://github.com/T-vK/ESP32-BLE-Keyboard
#include "src/BLE-Keyboard/BleKeyboard.h"
BleKeyboard BLEKeyboard("GoodCard :)", "VoidNoi", 100);

#include <SPI.h>
#include "M5Cardputer.h"

#define display M5Cardputer.Display
#define kb M5Cardputer.Keyboard


const int maxFiles = 100;

#include "keys.h"

File myFile;
String root = "/BadCard";
String path = root;
String pathArray[maxFiles] = {root};
int pathLen = 0;

int fileAmount;

int mainCursor = 0;
int scriptCursor = 0;
int kbLayoutsCursor = 0;

String sdFiles[maxFiles] = {"NEW SCRIPT", "NEW FOLDER", "ACTIVATE BLE", "KB LAYOUT"};
int fileType[maxFiles] = {1, 2, 3, 4};

const int scriptOptionsAmount = 3;
String scriptMenuOptions[scriptOptionsAmount] = {"Execute script", "Edit script", "Delete script"};
// The almighty doesn't want kbLayoutsLen and KbLayouts initialization to be below maxFiles for some reason
// so don't you dare move it or the keyboard layouts menu will be bugged
const int kbLayoutsLen = 14; // Needs 1 more than the amount of layouts to prevent a visual bug in the menu
String kbLayouts[kbLayoutsLen] = {"en_US", "es_ES", "de_DE", "pt_PT", "fr_FR", "sv_SE", "it_IT", "hu_HU", "da_DK", "pt_BR", "en_GB", "nb_NO", "ja_JP"};

const int ELEMENT_COUNT_MAX = 500;
String fileText[ELEMENT_COUNT_MAX];

int cursorPosX, cursorPosY, screenPosX, screenPosY = 0;

int newFileLines = 0;

int letterHeight = 16;
int letterWidth = 12;

int deletingFolder = false;

String cursor = "|";

String fileName;

bool creatingFile = false;
bool editingFile = false;
bool saveFile = false;
bool isBLE = false;
bool showIcons = false;

void getCurrentPath() {
  path = "";
  for (int i = 0; i <= pathLen; i++) {
    path += pathArray[i];
  }
}

void getDirectory(String directory, int &amount, String* fileArray, int* types) {
  File dir = SD.open(directory);

  String tempFilesArray[maxFiles];
  int tempFilesAmount = 0;
  int tempDirsAmount = amount;

  while (true) {
    File entry =  dir.openNextFile();
    if (!entry) {
      // no more files
      break;
    }

    String fileName = entry.name();
    // Add file to list if not named language.lang
    if (fileName != "language.lang") {
      if (entry.isDirectory()) {
        fileArray[tempDirsAmount] = fileName; // Store directories first
        types[tempDirsAmount] = 6; // Set type directory
        tempDirsAmount++;
      } else {
        tempFilesArray[tempFilesAmount] = fileName; // Store files in a temporary array
        tempFilesAmount++;
      }
      amount++;
    }
    
    if (amount > maxFiles) {
      display.fillScreen(BLACK);
      display.println("Can't store any more scripts");
    }
  }
  int filesCount = 0;
  // Set files after directories and set the type to file
  for(int i = tempDirsAmount; i < amount; i++) {
    if (tempFilesAmount > 0) {
      fileArray[i] = tempFilesArray[filesCount];
      types[i] = 5;
      filesCount++;
    }
  }
}

void printMenu(int cursor, String* strings, int stringsAmount, int screenDirection, bool addIcons) {
  int textPosX = addIcons ? 40 : 20;
  for (int i = 0; i <= stringsAmount; i++) {
    int prevString = i-screenDirection;
    
    if (screenDirection != 0) {
      if (addIcons){
        display.setSwapBytes(true);
        display.fillRect(20, i*20, 16, 16, BLACK);
        display.pushImage(20, i*20, 16, 16, (uint16_t *)icons[fileType[i+screenPosY]]);
      }
      display.setTextColor(BLACK);
      display.drawString(strings[prevString+screenPosY], textPosX, i*20);
          
      display.setTextColor(PURPLE);
      display.drawString(strings[i+screenPosY], textPosX, i*20);
    } else {
      if (addIcons){
        display.setSwapBytes(true);
        display.pushImage(20, i*20, 16, 16, (uint16_t *)icons[fileType[i]]);
      }
      display.setTextColor(PURPLE);
      display.drawString(strings[i], textPosX, i*20);
    }
  }
}

void handleMenus(int options, void (*executeFunction)(), int& cursor, String* strings, bool addIcons) {
  display.fillScreen(BLACK);
  cursor = 0;
  int screenDirection; // -1 = down | 0 = none | 1 = up;
  while (true) {
    M5Cardputer.update();
    if (kb.isChange()) {
      if (screenPosY == 0) {
        screenDirection = 0;
      }
      display.setTextColor(BLACK);
      int drawCursor = cursor*20;
        
      display.drawString(">", 5, drawCursor);

      if (kb.isKeyPressed(';') && cursor > 0){
        cursor--;

        if (screenPosY > 0 && cursor > 0) {
          screenPosY--;
          screenDirection = -1;
        }
      } else if (kb.isKeyPressed('.') && cursor < options) {
        cursor++;

        if (cursor * 20 >= display.height() - 20) {
          screenPosY++;
          screenDirection = 1;
        }
      }
      
      drawCursor = cursor*20;
      if (cursor * 20 > display.height()-20) {
        drawCursor = (display.height() - 20) - 15;
      }

      display.setTextColor(PURPLE);

      display.drawString(">", 5, drawCursor);

      printMenu(cursor, strings, options, screenDirection, addIcons);

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
  String fileName = path + "/" + sdFiles[mainCursor];

  if (SD.exists(fileName)) {
    
    if (!isBLE) {
      USB.begin();
      Keyboard.begin(layout);
    }

    delay(1000);
    
    display.println(fileName);

    // Open the file for reading (fill myFile with char buffer)
    myFile = SD.open(fileName, FILE_READ);
    
    // Check if the file has successfully been opened and continue
    if (myFile) {
      // Initialize control over keyboard
        
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
      if (isBLE) {
        BLEKeyboard.end();
      } else {
        Keyboard.end();
      }
      handleFolders();
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
  
    display.setCursor(display.width()/2-line.length()/2*letterWidth, display.height()-letterWidth*2);
    display.setTextColor(PURPLE);
    display.println(line);

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
  } else if (command == "DELAY") {                            // Delay before the next command
    delay((int) payload.toInt());                           // Convert payload to integer and make pause for 'payload' time
  } else if (command == "STRING") {
    int payloadLen = payload.length();
    unsigned char array[payloadLen];
    for (int i = 0; i < payloadLen; i++) {
      array[i] = payload[i];
    }
    array[payloadLen] = '\0';

    char16_t uString[payloadLen];

    utf8_to_utf16(&array[0], payloadLen, &uString[0], payloadLen);
    uString[payloadLen] = '\0';
    if (isBLE) {
      BLEKeyboard.print(payload);
    } else {
      Keyboard.write(uString);                                // Type-in the payload
    }                          
    // String processing

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

  
  display.setCursor(display.width()/2-line.length()/2*letterWidth, display.height()-letterWidth*2);
  display.setTextColor(BLACK);
  display.println(line);
  
  if (isBLE) {
    BLEKeyboard.releaseAll();
  } else {
    Keyboard.releaseAll();
  }
}

void keyboardPress(uint8_t key) {
  if (isBLE) {
    BLEKeyboard.press(key);
  } else {
    Keyboard.press(key);
  }
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
    keyboardPress(c);           // Press the key onkeyboard
  } else if (command == "ENTER") {
    keyboardPress(KEY_RETURN);
  } else if (command == "MENU" || command == "APP") {
    keyboardPress(KEY_MENU);
  } else if (command == "DOWNARROW" || command == "DOWN") {
    keyboardPress(KEY_DOWN_ARROW);
  } else if (command == "LEFTARROW" || command == "LEFT") {
    keyboardPress(KEY_LEFT_ARROW);
  } else if (command == "RIGHTARROW" || command == "RIGHT") {
    keyboardPress(KEY_RIGHT_ARROW);
  } else if (command == "UPARROW" || command == "UP") {
    keyboardPress(KEY_UP_ARROW);
  } else if (command == "BREAK" || command == "PAUSE") {
    keyboardPress(KEY_PAUSE);
  } else if (command == "CAPSLOCK") {
    keyboardPress(KEY_CAPS_LOCK);
  } else if (command == "DELETE" || command == "DEL") {
    keyboardPress(KEY_DELETE);
  } else if (command == "END") {
    keyboardPress(KEY_END);
  } else if (command == "ESC" || command == "ESCAPE") {
    keyboardPress(KEY_ESC);
  } else if (command == "HOME") {
    keyboardPress(KEY_HOME);
  } else if (command == "INSERT") {
    keyboardPress(KEY_INSERT);
  } else if (command == "NUMLOCK") {
    keyboardPress(KEY_NUMLOCK);
  } else if (command == "PAGEUP") {
    keyboardPress(KEY_PAGE_UP);
  } else if (command == "PAGEDOWN") {
    keyboardPress(KEY_PAGE_DOWN);
  } else if (command == "PRINTSCREEN") {
    keyboardPress(KEY_PRINTSCREEN);
  } else if (command == "SCROLLLOCK") {
    keyboardPress(KEY_SCROLLLOCK);
  } else if (command == "SPACE") {
    keyboardPress(KEY_SPACE);
  } else if (command == "BACKSPACE") {
    keyboardPress(KEY_BACKSPACE);
  } else if (command == "TAB") {
    keyboardPress(KEY_TAB);
  } else if (command == "GUI" || command == "WINDOWS") {
    keyboardPress(KEY_LEFT_GUI);
  } else if (command == "SHIFT") {
    keyboardPress(KEY_RIGHT_SHIFT);
  } else if (command == "ALT") {
    keyboardPress(KEY_LEFT_ALT);
  } else if (command == "CTRL" || command == "CONTROL") {
    keyboardPress(KEY_LEFT_CTRL);
  } else if (command == "F1" || command == "FUNCTION1") {
    keyboardPress(KEY_F1);
  } else if (command == "F2" || command == "FUNCTION2") {
    keyboardPress(KEY_F2);
  } else if (command == "F3" || command == "FUNCTION3") {
    keyboardPress(KEY_F3);
  } else if (command == "F4" || command == "FUNCTION4") {
    keyboardPress(KEY_F4);
  } else if (command == "F5" || command == "FUNCTION5") {
    keyboardPress(KEY_F5);
  } else if (command == "F6" || command == "FUNCTION6") {
    keyboardPress(KEY_F6);
  } else if (command == "F7" || command == "FUNCTION7") {
    keyboardPress(KEY_F7);
  } else if (command == "F8" || command == "FUNCTION8") {
    keyboardPress(KEY_F8);
  } else if (command == "F9" || command == "FUNCTION9") {
    keyboardPress(KEY_F9);
  } else if (command == "F10" || command == "FUNCTION10") {
    keyboardPress(KEY_F10);
  } else if (command == "F11" || command == "FUNCTION11") {
    keyboardPress(KEY_F11);
  } else if (command == "F12" || command == "FUNCTION12") {
    keyboardPress(KEY_F12);
  }
}

void insertLine(String file[], int y){
  for (int i = newFileLines; i > y; i--){
    file[i] = file[i-1];
  }
  file[y] = '\0';
}

void insertChar(String file[], int y, int x, char letter){
  if (cursorPosX >= file[y].length()) {
    file[y] += letter;
  } else {
    String modLine = file[y].substring(0,x-1) + letter + file[y].substring(x-1,file[y].length());
    file[y] = modLine;
  }
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
  cleanArray<String*>(fileText, newFileLines);
  cursorPosX = 0;
  cursorPosY = 0;
  screenPosX = 0;
  screenPosY = 0;
}

void saveFileChanges() {
  saveFile = false;

  myFile = SD.open(path + "/" + fileName + ".txt", FILE_WRITE);
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
    handleFolders();
  } else {
    display.println("File didn't open");
    myFile.close();
    delay(2000);
    handleFolders();
  }
}

void editFile() {
  // Open the file for reading
  myFile = SD.open(path + "/" + fileName, FILE_READ);
  if (myFile) {
    // Load the file into the fileText array
    newFileLines = 0;
    String line = "";
    while (myFile.available()) {
      char c = myFile.read();
      if ((int)c == 0x0a) {
        fileText[newFileLines] = line;
        newFileLines++;
        line = "";
      }
      else if ((int)c != 0x0d) {
        line += c;
      }
    }
    myFile.close();

    // Display the file content:
    display.fillScreen(BLACK);
    display.setTextSize(2);
    for (int i = 0; i <= newFileLines; i++) {
      display.drawString(fileText[i], 0, i * letterHeight);
    }
  }
  else {
    display.println("Error occurred while opening file");
    return;
  }
  display.setCursor(1, 1);
  editingFile = true;
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
  String fileName = path + "/" + sdFiles[mainCursor];
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
  handleFolders();
}

void scriptOptions() {
  if (scriptCursor == 0) {
    executeScript();
  }
  else if (scriptCursor == 1) {
    fileName = sdFiles[mainCursor];
    editFile();
  }
  else if (scriptCursor == 2) {
    deleteScript();
  }
}

void setKBLayout(int layoutNum) {
  switch (layoutNum) {
    case 0:
      layout = new KeyboardLayout_US();
      break;
    case 1:
      layout = new KeyboardLayout_ES();
      break;
    case 2:
      layout = new KeyboardLayout_DE();
      break;
    case 3:
      layout = new KeyboardLayout_PT();
      break;
    case 4:
      layout = new KeyboardLayout_FR();
      break;
    case 5:
      layout = new KeyboardLayout_SE();
      break;
    case 6:
      layout = new KeyboardLayout_IT();
      break;
    case 7:
      layout = new KeyboardLayout_HU();
      break;
    case 8:
      layout = new KeyboardLayout_DK();
      break;
    case 9:
      layout = new KeyboardLayout_BR();
      break;
    case 10:
      layout = new KeyboardLayout_GB();
      break;
    case 11:
      layout = new KeyboardLayout_NO();
      break;
    case 12:
      layout = new KeyboardLayout_JP();
      break;
  }
}

void kbLayoutsOptions() {
  currentKBLayout = kbLayoutsCursor;
  setKBLayout(currentKBLayout);
  
  setLang(currentKBLayout);

  handleFolders();
}

template <typename T> void cleanArray(T array, int length) {
  for (int i = 0; i <= length; i++) {
    array[i] = '\0';
  }
	length = 0;
}

void handleFolders() {
  if (pathArray[pathLen] != root) {
    fileAmount = 3;
    if (mainCursor > 2 && fileType[scriptCursor] < 8 && !deletingFolder) {
      pathArray[pathLen] = "/" + sdFiles[mainCursor];
    }
    getCurrentPath();
    sdFiles[0] = "..";
    sdFiles[1] = "NEW SCRIPT";
    sdFiles[2] = "NEW FOLDER";
    fileType[0] = 7;
    fileType[1] = 1;
    fileType[2] = 2;
  } else {
    fileAmount = 4;
    path = root;
    pathLen = 0;

    sdFiles[0] = "NEW SCRIPT";
    sdFiles[1] = "NEW FOLDER";
    sdFiles[2] = "ACTIVATE BLE";
    sdFiles[3] = "KB LAYOUT";

    fileType[0] = 1;
    fileType[1] = 2;
    fileType[2] = 3;
    fileType[3] = 4;
  }

  getDirectory(path, fileAmount, sdFiles, fileType);
  // Empties an extra value at the end to prevent previous files from appearing in the menu
  sdFiles[fileAmount] = '\0';
  fileType[fileAmount] = '\0';

  mainCursor = 0;
  deletingFolder = false;
  handleMenus(fileAmount-1, mainOptions, mainCursor, sdFiles, true);
}

void newFolder() {
  fileName = '\0';

  display.setCursor(1,1);
  display.fillScreen(BLACK);

  display.setCursor(display.width()/2-(12/2)*letterWidth, 0);
  display.println("Folder Name:");
  display.drawString(fileName, display.width()/2-(fileName.length()/2)*letterWidth, letterHeight);
  saveFile = true;
}

void makeFolder() {
  saveFile = false;

  getCurrentPath();
  SD.mkdir(path + "/" + fileName);
  handleFolders();
}

void deleteFolderMenu() {
  int deleteCursor = 1;
  int deleteCursorPosX = display.width()/2+2*letterWidth/2+letterWidth;
  //Displays the delete folder menu
  display.fillScreen(BLACK); 
  display.setCursor(display.width()/2-15*letterWidth/2, display.height()/2-letterHeight*3);
  display.println("Deleting folder");
  display.setCursor(display.width()/2-12*letterWidth/2, display.height()/2-letterHeight/2*3);
  display.println("Are you sure?");
  display.setCursor(display.width()/2-10*letterWidth/2, display.height()/2);
  display.println("Yes     No");
  // Handles menu controls
  while (true) {
    M5Cardputer.update();
    if (kb.isChange()) {
      display.setTextColor(BLACK);
      display.drawString(">", deleteCursorPosX, display.height()/2);
      // "," is pressed, select yes
      if (kb.isKeyPressed(',')) {
        deleteCursor = 0;
        
        deleteCursorPosX = display.width()/2-10*letterWidth/2-letterWidth;
      }
      // "/" is pressed, select no
      if (kb.isKeyPressed('/')) {
        deleteCursor = 1;

        deleteCursorPosX = display.width()/2+2*letterWidth/2+letterWidth;
      }
      
      display.setTextColor(PURPLE);
      display.drawString(">", deleteCursorPosX, display.height()/2);

      // Cursor is on yes ("0"), attempt to delete folder
      if (kb.isKeyPressed(KEY_ENTER)) {
        if (deleteCursor == 0) {
          String folderPath = path + "/" + sdFiles[mainCursor];
          File dir = SD.open(folderPath);
          // The folders have files inside, try to delete them
          if (!SD.rmdir(folderPath)){
            while (true) {
              File folderFile =  dir.openNextFile();
              if (!folderFile) { // No more files
                if (SD.rmdir(folderPath)) { // All files have been removed, delete folder
                  display.fillScreen(BLACK);
                  display.setCursor(1,1);
                  display.println("Folder successfully deleted");
                } else { // The folder couldn't be removed for some unknown reason
                  display.fillScreen(BLACK);
                  display.setCursor(1,1);
                  display.println("Folder couldn't be deleted");
                }

                mainCursor = 0;
                break;
              }
              
              String folderFileName = folderFile.name();
              // If the file is a directory stop the function and let the user remove it manually
              // This is done because I haven't implemented recursive file/folder deletion
              if (folderFile.isDirectory()) {
                display.fillScreen(BLACK);
                display.setCursor(1,1);
                display.println("Remove folders first");
                mainCursor = 0;
                break;
              } else { // The file couldn't be removed for some unknown reason
                if (!SD.remove(folderPath + "/" + folderFileName)) {
                  display.fillScreen(BLACK);
                  display.setCursor(1,1);
                  display.println("Couldn't remove a file");
                }
              }
            }
          } else { // If the folder doesn't have any files inside, delete it
            display.fillScreen(BLACK);
            display.setCursor(1,1);
            display.println("Folder successfully deleted");
          }
          dir.close();
          delay(1500);
        }
        // Return to current folder
        handleFolders();
        break;
      }
    }
  }
}

void mainOptions() {
  switch (fileType[mainCursor]) {
  case 1: // New File
    newFile();
    break;
  case 2: // New Folder
    newFolder();
    break;
  case 3: // BLE toggle
    display.fillScreen(BLACK);
    if (!isBLE) {
      isBLE = true;
      BLEKeyboard.begin();
      sdFiles[1] = "BLE ACTIVATED";
    } else {
      isBLE = false;
      sdFiles[1] = "ACTIVATE BLE";
    }
    handleFolders();
    break;
  case 4: // Keyboard Layouts
    handleMenus(kbLayoutsLen-2, kbLayoutsOptions, kbLayoutsCursor, kbLayouts, false); // We remove 1 more than the others from kbLayoutsLen to compensate for the extra 1 added at declaration
    break;
  case 5: // File
    fileType[0] = 8;
    fileType[1] = 9;
    fileType[2] = 10;
    handleMenus(scriptOptionsAmount-1, scriptOptions, scriptCursor, scriptMenuOptions, true);
    break;
  case 6: // Folder
    // If G0 button is pressed show delete folder menu
    if (digitalRead(0)==0) {
      deletingFolder = true;
      deleteFolderMenu();
    } else {
      pathLen++;
      handleFolders();
    }
    break;
  case 7: // Previous folder
    sdFiles[pathLen] = '\0';
    pathLen--;
    handleFolders();
    break;
  }
}

void bootLogo(){

  display.fillScreen(BLACK);
  
  display.setTextSize(2);
  String BCVersion = "BadCard v1.6.0";

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
      handleFolders();
      break;
    }
  }
}

void setLang(int lang) {
  File langFile = SD.open(root + "/" + "language" + ".lang", FILE_WRITE);
  langFile.print(lang);
  langFile.close();
}

void getLang() {
  String langPath = root + "/" + "language" + ".lang";

  if (SD.exists(langPath)) {
    File langFile = SD.open(langPath, FILE_READ);
    if (langFile) {
      String line;
      while (langFile.available()) {
        char c = langFile.read();

        if ((int)c != 0x0d) {
          line += c;
        }
      }
      langFile.close();
      int langNum = line.toInt();
      currentKBLayout = langNum;
      setKBLayout(currentKBLayout);
    }
  } else {
    File langFile = SD.open(langPath, FILE_WRITE);
    langFile.print("0");
    langFile.close();
    layout = new KeyboardLayout_US();
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
  
  getLang();

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

        String newFileName = fileType[mainCursor] == 2 ? "Folder Name:" : "File Name:";

        display.setCursor(display.width()/2-(newFileName.length()/2)*letterWidth, 0);
        display.println(newFileName);
        display.drawString(fileName, display.width()/2-(fileName.length()/2)*letterWidth, letterHeight);

        if (status.enter) {
          if (fileType[mainCursor] == 2) {
            makeFolder();
          } else {
            saveFileChanges();
          }
        }
      }
    }
  }

  if (creatingFile || editingFile) {
    if (kb.isChange()) {
      if (kb.isPressed()) {
        Keyboard_Class::KeysState status = kb.keysState();
        int prevCursorY;

        if (status.fn && kb.isKeyPressed('`')) {
          if (creatingFile) {
            fileName = "\0";
            creatingFile = false;
            saveFile = true;
          }
          if (editingFile) {
            String fullName = fileName;
            fullName.remove(fullName.length()-4);
            fileName = fullName;
            editingFile = false;
            saveFile = true;
          }
        } else if (status.fn && kb.isKeyPressed(';') && cursorPosY > 0) {
          prevCursorY = cursorPosY;
          cursorPosY--;
          if (cursorPosX >= fileText[prevCursorY].length()) {
            cursorPosX = fileText[cursorPosY].length();
          }
          if (screenPosY > 0 && cursorPosY > 0) {
            screenPosY--;
          }
          if (cursorPosX * letterWidth > display.width() && fileText[cursorPosY].length() * letterWidth > display.width()) {
            screenPosX = (fileText[cursorPosY].length() - 19) * -letterWidth;
          } else {
            screenPosX = 0;
          }
        } else if (status.fn && kb.isKeyPressed('.') && cursorPosY < newFileLines) {
          prevCursorY = cursorPosY;
          cursorPosY++;
          if (cursorPosX >= fileText[prevCursorY].length()) {
            cursorPosX = fileText[cursorPosY].length();
          }
          if (cursorPosY * letterHeight >= display.height() - letterHeight) {
            screenPosY++;
          }
          if (cursorPosX * letterWidth > display.width() && fileText[cursorPosY].length() * letterWidth > display.width()) {
            screenPosX = (fileText[cursorPosY].length() - 19) * -letterWidth;
          } else {
            screenPosX = 0;
          }
        } else if (status.fn && kb.isKeyPressed(',') && cursorPosX > 0) {
          cursorPosX--;
          if (screenPosX < 0) {
            screenPosX += letterWidth;
          }
        } else if (status.fn && kb.isKeyPressed('/') && cursorPosX < fileText[cursorPosY].length()) {
          cursorPosX++;
          if (cursorPosX * letterWidth >= display.width()) {
            screenPosX -= letterWidth;
          }
        } else if (!status.fn) {
          for (auto i : status.word) {

            if (cursorPosX * letterWidth >= display.width() - letterWidth) {
              screenPosX -= letterWidth;
            }
            cursorPosX++;

            insertChar(fileText, cursorPosY, cursorPosX, i);
          }
        }

        if (status.del) {
          if (screenPosX < 0) {
            screenPosX += letterWidth;
          }
          if (cursorPosX > 0) {
            cursorPosX--;
            fileText[cursorPosY].remove(cursorPosX, 1);
          } else if (cursorPosX <= 0 && cursorPosY > 0) {
            
            cursorPosX = fileText[cursorPosY-1].length(); // Set cursor at the end of the previous line
            // Set screen position to where the previous line ends
            if (fileText[cursorPosY-1].length() * letterWidth > display.width()) { 
              screenPosX = (fileText[cursorPosY-1].length() - 19) * -letterWidth;
            } else {
              screenPosX = 0;
            }
            fileText[cursorPosY - 1] += fileText[cursorPosY]; // Move the entire line up to the end of the line above
            removeLine(fileText, cursorPosY);
          }
        }

        if (status.enter) {
          String currentLine = fileText[cursorPosY];
          String remainingLine = currentLine.substring(cursorPosX);
          currentLine = currentLine.substring(0, cursorPosX);

          fileText[cursorPosY] = currentLine;
          newFileLines++;
          cursorPosY++;
          insertLine(fileText, cursorPosY);
          fileText[cursorPosY] = remainingLine;

          if (cursorPosY * letterHeight >= display.height() - letterHeight) {
            screenPosY++;
          }
          screenPosX = 0;
          cursorPosX = 0;
        }

        if (cursorPosX > fileText[cursorPosY].length()) {
          cursorPosX = fileText[cursorPosY].length();
        }

        display.fillScreen(BLACK);

        display.setTextSize(1.5);

        int drawCursorX = cursorPosX * letterWidth;
        int drawCursorY = cursorPosY * letterHeight - 5;

        if (cursorPosX * letterWidth > display.width() - letterWidth) {
          drawCursorX = display.width() - letterWidth;
        }

        if (cursorPosY * letterHeight > display.height() - letterHeight) {
          drawCursorY = (display.height() - letterHeight) - 10;
        }

        display.drawString(cursor, drawCursorX - 3, drawCursorY, &fonts::Font2);
        
        display.setTextSize(2);

        for (int i = 0; i<= newFileLines; i++) {
          display.drawString(fileText[i+screenPosY], screenPosX, i*letterHeight);
        }
      }
    }
  }
} 
