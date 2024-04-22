# BadCard
<img src="https://github.com/VoidNoi/BadCard/blob/main/BADCARD.png" height="400">
BadUSB for the Cardputer with ducky script support and extra functionality

Ducky Script processing and executing ported from [arducky](https://github.com/Creased/arducky)

<details>
  <summary>
    <strong>Available commands</strong>
  </summary>
  
     (1) Commands without payload:
     - ENTER
     - MENU <=> APP
     - DOWNARROW <=> DOWN
     - LEFTARROW <=> LEFT
     - RIGHTARROW <=> RIGHT
     - UPARROW <=> UP
     - BREAK <=> PAUSE
     - CAPSLOCK
     - DELETE
     - END
     - ESC <=> ESCAPE
     - HOME
     - INSERT
     - NUMLOCK
     - PAGEUP
     - PAGEDOWN
     - PRINTSCREEN
     - SCROLLLOCK
     - SPACE
     - TAB
     - REPLAY (global commands aren't implemented)
     
     (2) Commands with payload:
     - DEFAULT_DELAY <=> DEFAULTDELAY (global commands aren't implemented.)
     - DELAY (+int)
     - STRING (+string)
     - GUI <=> WINDOWS (+char)
     - SHIFT (+char or key)
     - ALT (+char or key)
     - CTRL <=> CONTROL (+char or key)
     - REM (+string)
  
</details>
<strong>If you're having trouble executing your payloads on Windows 11 try adding a <code>DELAY 500</code> at the end of your payload https://github.com/VoidNoi/BadCard/issues/1#issuecomment-2000571655</strong>

## How to use
Your files should use the `.txt` extension.
<br/>
You can place your payloads inside the `/BadCard` folder on your SD card (It will be created automatically after installing BadCard)
<br/>
OR
<br/>
You can make your own payloads in your cardputer with the <strong>"NEW FILE"</strong> option 

## File creating controls
<ul>
  <li>
    You can navigate your file pressing <code>fn</code> and the <code>up</code>, <code>down</code>, <code>left</code> or <code>right</code> keys (aka <code>;</code>, <code>.</code>, <code>,</code> or <code>/</code>)
  </li>
  <li>
    When you're finished creating your file press <code>fn</code> and <code>esc</code>, add the name of your file and press enter (you don't need to add the <code>.txt</code> extension)
  </li>
</ul>

## TO DO
<ul>
  <li>
    File editing function - Done [GuidoAguiar](https://github.com/guidoaguiar)
  </li>
  <li>
    Bluetooth connection support
  </li>
</ul>
