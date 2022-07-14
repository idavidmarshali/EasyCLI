<img src="../images/EasyCLI-ICON.png" width="150" align="middle">

# EasyCLI Documentation 
by [DavidMarshal](https://github.com/idavidmarshali)

-----------------

### Getting started :

for using EasyCLI, download the latest release, include the header file in your code.<br> when compiling, add the Static or Shared version of the library as a link target.

Simple Example of the libraries usage :
```c
// test.c
#include "EasyCLI.h"

int main(void){
    // Clear consoles screen
    EC_ClearScreen(EC_CLEARSCREEN_ClearScreen);
    // print colored text
    EC_PrintColored("{*R}Hello World{0} in a bold red color.");
}
```
```
gcc test.c libEasyCLI.a
```
-----------------
## TABLE OF CONTENT
- [**Print colored and styled text (Simple Way)**](#simple-color-printing-and-styling) 
- [**Advanced Coloring**](#advanced-coloring)
- [**Cursor Control**](#cursor-control)
- [**Console Screen Control**](#console-screen-control)
- [**Enums Table**](#enums-table)
- [**Function Table**](#function-table)
- [**Modification Preprocessors**](#modification-preprocessors)
- [**Compiling and building**](#compiling-and-building)

-----------------
### Simple color printing and styling :

one of the ways to print colored or styled text to the screen using EasyCLI is with the usage of EasyCLIs print functions and color formatting rules.

EasyCLI contains 2 print functions :
1. `EC_Print()`
2. `EC_PrintColored()`

these two functions will send any strings given to them to the `EC_Format_s()` which follows a simple formatting rule :<br>

`"NORMAL TEXT {STYLE COLOR} TEXT WITH STYLE AND COLOR"`<br>

for example :<br>
`{*_Y} Yellow bold underlined text`<br>
`{^$B} Slow blinking bright blue text`<br>
`{*/_-^$G} Bold Italic Underlined Striked Slow-blinking bright green text`<br>
`{M} Magenta text`<br>
`{*G}{#R} Bold green text with red background`<br>
`{*}BOLD IMPORTANT TEXT`

**Formatting RULES:**
1. Formatting starts with `{` and end with `}`
2. STYLE Rules **come before** Color rules.
3. You can't have two colors at once : 
   - `{-_R}{#B} is allowed`, `{-_R#B} or {-_#BR} is not`
4. You have to set foreground and background **colors** separately :
   - `{R}{#G} is Allowed`, `{#GR} or {R#G} is not allowed`
5. Faint can't be used with Bright, RapidBLink can't be used with SlowBlink. (although they will be formatted, the behavior is unkown)
6. Color tag `D` stands for default and sets the foreground color to white and background to black but won't change the style.
7. Style tag `0` resets the color and style of the text to consoles default.


#### Styles :
Styles are defined in [EasyCLI.h](../include/EasyCLI.h), they are formatted as `EC_STYLE_STYLENAME`

|    NAME    | TAG |
|:----------:|:---:|
|    BOLD    |  *  |
|   ITALIC   |  /  |
| UNDERLINE  |  _  |
|   STRIKE   |  -  |
| SLOWBLINK  |  ^  |
| RAPIDBLINK |  !  |    
|   FAINT    |  .  |
|   BRIGHT   |  $  |
| BACKGROUND |  #  |
|   RESET    |  0  |

#### Colors :
Colors are defined in [EasyCLI.h](../include/EasyCLI.h), they are formatted as `EC_COLOR_COLORNAME`

|  NAME   | TAG |
|:-------:|:---:|
|   RED   |  R  |
|  GREEN  |  G  |
| YELLOW  |  Y  |
|  BLUE   |  B  |
| MAGENTA |  M  |
|  CYAN   |  C  |    
|  WHITE  |  W  |
| DEFAULT |  D  |

above colors are 3 to 4 bit colors<br>
for more advanced console coloring refer to the [Advanced coloring section](#advanced-coloring)

NOTE:<br>

**the colors above might be represented differently based on the terminal and theme that is being used!**  

`EC_Print()` only prints colored text if `EC_SetColorMode()` has been called with `EC_COLORMODE_Enabled` else it will remove any color formatting from the string and prints it normally.

`EC_PrintColored()` prints colored text regardless of `EC_SetColorMode()`.

-----------------


### Advanced Coloring:

using `EC_Print()` or `EC_PrintColored()` will give you access to a small limited range of 3 to 4 bit colors. now we earn how to use 8 / 16 to 24 bit colors.

EasyCLI has 4 advanced color functions under Screen modification category :
1. `EC_ScreenSetForegroundRGB(R, G, B)`
2. `EC_ScreenSetBackgroundRGB(R, G, B)`
3. `EC_ScreenSetForegroundINDX(index)`
4. `EC_ScreenSetBackgroundINDX(index)`

the **RGB** functions are self explanatory, the will set the foreground/background of the console to the RGB value given to them. **NOTE: each R/G/B values can't exceed 255.**

the **INDX** functions will set the foreground/background color based on the index of a color in the 256bit look up table found [**here**](https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit)

example :
```c
#include "EasyCLI.h"
#include <stdio.h>

int main(void){
    EC_ScreenSetForegroundRGB(255, 150, 0);
    EC_ScreenSetBackgroundINDX(92);
    printf("Orange foreground with a purple background.");
    // or 
    EC_Print("Orange foreground with a purple background.");
}
```
-----------------

### Cursor Control

EasyCLI offers a range of cursor control functions as listed bellow :<br>
NOTE: all cursor related functions are prefixed with `EC_Cursor`
1. Cursor Movement
    - `EC_CursorMoveUp(n)`
    - `EC_CursorMoveDown(n)`
    - `EC_CursorMoveForward(n)`
    - `EC_CursorMoveBackward(n)`
    - `EC_CursorMoveNextLine(n)`
    - `EC_CursorMovePrevLine(n)`
    - `EC_CursorMoveHorizontalAbs(n)`
    - Moves the cursor n times in the said direction.
2. Cursor Positioning
    - `EC_CursorSetPos(row, column)`
    - `EC_CursorSavePos()`
    - `EC_CursorRestorePos()`
    - sets, saves or loads Cursor position.
    - **WINDOWS ONLY** `EC_CursorGetPosW()`
3. Cursor Visibility
    - `EC_CursorHide()`
    - `EC_CursorShow()`
    - `EC_CursorEnableBlink()`
    - `EC_CursorDisableBlink()`
    - Change the visibility of cursor.

for more detailed description of functions see the [Function Table](#function-table)

-----------------

### Console Screen Control

EasyCLI offers simple control over the console screen with the following functions :<br>
NOTE: all screen related functions are prefixed with `EC_Screen` (except ClearScreen and ClearLine)

1. Screen Color
    - `EC_ScreenSetForegroundRGB(R, G, B)`
    - `EC_ScreenSetBackgroundRGB(R, G, B)`
    - `EC_ScreenSetForegroundINDX(index)`
    - `EC_ScreenSetBackgroundINDX(index)`
    - Set the color of console screen to 8/16/24bit colors
    - **WINDOWS ONLY** `EC_ScreenSetConsoleColorW(color)`
2. Screen Buffer
    - `EC_ClearScreen(mode)`
    - `EC_ClearLine(mode)`
    - `EC_ScreenEnableAlterBuff()`
    - `EC_ScreenDisableAlterBuff()`
    - Enable/Disable Consoles alternative screen buffer, clear the screen buffer or a line of it.
4. Screen Character Set
    - `EC_ScreenEnableAsciiCharSet()`
    - `EC_ScreenEnableDecCharSet()`
    - Enable/Disable DEC Line Drawing mode.
5. Screen Scrolling
    - `EC_ScreenScrollUp(n)`
    - `EC_ScreenScrollDown(n)`
    - Scroll n times in the said direction

for more detailed description of functions see the [Function Table](#function-table)

-----------------

## Enums Table:
**NOTE: Every Enum name most be prefixed with `EC_` as defined in [EasyCLI.h](../include/EasyCLI.h) *if* `#define EC_NO_PREFIX` is not present**
<br>E.g. `ResultEnum -> EC_ResultEnum`

| Enum Name           | Values                                                                                                                                                                                                                                                                                                                                                          |
|:--------------------|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `   ResultEnum    ` | `EC_RES_Success` (Function Succeeded without any error)<br>`EC_RES_Failed` (Function threw an error, explained in [Function Table](#function-table))<br>`EC_RES_NoHandle` (Function failed to get handle of an standard stream)<br>`EC_RES_FailedToAllocMem` (Function failed to allocate memory)<br>`EC_RES_InvalidArg` (Function recived an invalid argument) |
| `  ColorModeEnum  ` | `EC_COLORMODE_Disabled` (set EC_IsColorMode to false, EC_Print() removes formatting)<br>`EC_COLORMODE_Enabled` (set EC_IsColorMode to true, EC_Print() prints colored string)                                                                                                                                                                                   |
| ` ClearScreenEnum ` | `EC_CLEARSCREEN_CursorToEnd` (Clear from the cursor to the end of the screen)<br>`EC_CLEARSCREEN_CursorToStart` (Clear from cursor to start of the screen)<br>`EC_CLEARSCREEN_ClearScreen` (Clear from start to end)<br>`EC_CLEARSCREEN_ClearScrollBuffer` (Clear the Scroll buffer)                                                                            |
| `  ClearLineEnum  ` | `EC_CLEARLINE_CursorToEnd` (Clear from cursor to the end of current line)<br>`EC_CLEARLINE_CursorToStart` (Clear from cursor to the start of current line)<br>`EC_CLEARLINE_ClearLine` (Clear the current Line)                                                                                                                                                 |

NOTE: `EC_ClearScreen()` and `EC_ClearLine()` will clear the screen without moving the cursor!

-----------------

## Function Table:
**NOTE: Every Function name most be prefixed with `EC_` as defined in [EasyCLI.h](../include/EasyCLI.h) *if* `#define EC_NO_PREFIX` is not present**
<br>E.g. `CursorHide(void) -> EC_CursorHide()`


| Function                                                                       |  Return Type  | Description                                                                                                                                                                                                                                                                                                                                    |
|:-------------------------------------------------------------------------------|:-------------:|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `                        Print(char* cpFormat, ...)                         `  | EC_ResultEnum | Prints colored text to screen if EC_IsColorMode == true else<br> Removes color format and prints. if returned EC_RES_Failed<br> means vsnprintf() has returned < 0.                                                                                                                                                                            |
| `                     PrintColored(char* cpFormat, ...)                     `  | EC_ResultEnum | Prints colored text to screen regardless of EC_IsColorModes value<br>if returned EC_RES_Failed means vsnprintf() has returned < 0.                                                                                                                                                                                                             |
| `                    SetColorMode(EC_ColorModeEnum mode)                    `  | EC_ResultEnum | Sets the value of EC_IsColorMode used by EC_Print().                                                                                                                                                                                                                                                                                           |
| `                   ClearScreen(EC_ClearScreenEnum mode)                    `  | EC_ResultEnum | Clears the screen based on the given mode. see [Enums Table]() for<br> more info.                                                                                                                                                                                                                                                              |
| `                     ClearLine(EC_ClearLineEnum mode)                      `  | EC_ResultEnum | Clears the line that the cursor is currently on based on the given <br>mode. see [Enums Table]() for more info.                                                                                                                                                                                                                                |  
| `                            CursorMoveUp(int n)                            `  |     void      | Moves the cursor n times up.                                                                                                                                                                                                                                                                                                                   |
| `                           CursorMoveDown(int n)                           `  |     void      | Moves the cursor n times down.                                                                                                                                                                                                                                                                                                                 |
| `                          CursorMovForward(int n)                          `  |     void      | Moves the cursor n times forward.                                                                                                                                                                                                                                                                                                              |
| `                         CursorMoveBackward(int n)                         `  |     void      | Moves the cursor n times backwards.                                                                                                                                                                                                                                                                                                            |
| `                         CursorMoveNextLine(int n)                         `  |     void      | Moves the cursor to the n line after current line.                                                                                                                                                                                                                                                                                             |
| `                         CursorMovePrevLine(int n)                         `  |     void      | Moves the cursor to the n line after current line.                                                                                                                                                                                                                                                                                             |
| `                      CursorMoveHorizontalAbs(int n)                       `  |     void      | Moves the cursor to column n without changing<br> cursors vertical position.                                                                                                                                                                                                                                                                   |
| `                             CursorHide(void)                              `  |     void      | Hides the cursor indicator.                                                                                                                                                                                                                                                                                                                    |
| `                             CursorShows(void)                             `  |     void      | Shows the cursor indicator if hidden.                                                                                                                                                                                                                                                                                                          |
| `                          CursorEnableBlink(void)                          `  |     void      | Enables the blinking action of the cursor.                                                                                                                                                                                                                                                                                                     |
| `                         CursorDisableBlink(void)                          `  |     void      | Disables the blinking action of the cursor.<br>(Makes it static)                                                                                                                                                                                                                                                                               |
| `                     CursorSetPos(int row, int column)                     `  |     void      | Sets the position of cursor to the given <br>row and column.                                                                                                                                                                                                                                                                                   |
| `                            CursorSavePos(void)                            `  |     void      | Saves the position of cursor to memory.                                                                                                                                                                                                                                                                                                        |
| `                          CursorRestorePos(void)                           `  |     void      | Restores the position of cursor from memory.                                                                                                                                                                                                                                                                                                   |
| `               ScreenSetForegroundRGB( nsigned char R, G, B)               `  |     void      | Sets the foreground color of console to <br>a 16/24bit color.                                                                                                                                                                                                                                                                                  |
| `               ScreenSetBackgroundRGB( nsigned char R, G, B)               `  |     void      | Sets the background color of console to <br>a 16/24bit color.                                                                                                                                                                                                                                                                                  |
| `               ScreenSetForegroundINDX( unsigned char index)               `  |     void      | Sets the foreground color to a color from<br>the [8bit / 256bit color table](https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit).                                                                                                                                                                                                            |
| `               ScreenSetBackgroundINDX( unsigned char index)               `  |     void      | Sets the background color to a color from<br>the [8bit / 256bit color table](https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit).                                                                                                                                                                                                            |
| `                        ScreenEnableAlterBuff(oid)                         `  |     void      | Enables consoles alternative screen buffer.                                                                                                                                                                                                                                                                                                    |
| `                       ScreenDisableAlterBuff(void)                        `  |     void      | Disables consoles alternative screen buffer.                                                                                                                                                                                                                                                                                                   |
| `                      ScreenEnableAsciiCharSet(void)                       `  |     void      | Returns Consoles Character set to Standard ASCII.                                                                                                                                                                                                                                                                                              |
| `                       ScreenEnableDecCharSet(void)                        `  |     void      | Sets Console Character set to DEC Line Drawing mode<br>[DEC LINE DRAWING](https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#designate-character-set).                                                                                                                                                        |
| `                            ScreenScrollUp(int n)                           ` |     void      | Scroll up n times                                                                                                                                                                                                                                                                                                                              |
| `                          ScreenScrollDown(int n)                          `  |     void      | Scroll down n times                                                                                                                                                                                                                                                                                                                            |
| `                          ScreenScrollDown(int n)                          `  |     void      | Scroll down n times                                                                                                                                                                                                                                                                                                                            |
| `                       SetWindowTitle(char* cpTitle)                       `  |     void      | Changes the title of console window to cpTitle.                                                                                                                                                                                                                                                                                                |
| `                              SoftReset(void)                              `  |     void      | Resets most attributes of console including : `Color`,`Style`,<br>`Title`,`Margins`,`...`.                                                                                                                                                                                                                                                     |
| `                                Beep(void)                                 `  |     void      | Makes a beep sound.                                                                                                                                                                                                                                                                                                                            |
| `                   PrintHyperLink(char* cpTitle, cpLink)                   `  |     void      | Prints a link to the screen <br>**(MIGHT NOT WORK OUTSIDE WINDOWS Terminal)**.                                                                                                                                                                                                                                                                 |
| **\\_\_WINDOWS ONLY FUNCTIONS__/**                                             |   *********   | ******                                                                                                                                                                                                                                                                                                                                         |
| `                      CursorGetPosW(COORD* outCoord)                       `  | EC_ResultEnum | Gets the current cursor position in screen using windows32 api and<br> sets outCoord.X, outCoord.Y Accordingly. if returned <br>EC_RES_NoHandle means it has failed to get STDOUT handle.<br>EC_RES_Failed means Getting console screen info or memcpy has<br> failed.                                                                         |
| `                         ConsoleEnableVTMode(void)                         `  | EC_ResultEnum | Enables [Virtual Terminal processing](https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences) on the console. it is required<br> for windows to parse ANSI Escape codes.if returned EC_RES_Failed<br>means Setting/Getting console mode has failed. EC_RES_NoHandle<br> means it has failed to get STDOUT handle. |
| `                          ConsoleResetMode(void)                           `  | EC_ResultEnum | Reverts Consoles mode back to how it was before<br> EC_ConsoleEnableVTMode() was called. has Same ERRORs as <br>EC_ConsoleEnableVTMode()                                                                                                                                                                                                       |
| `            ScreenSetConsoleColorW(EC_WindowsColorsEnum nColor)            `  | EC_ResultEnum | Sets the consoles color using the internal `color` function of<br> windows cmd. if returned EC_RES_Failed means system() has<br> failed. **this doesn't require Virtual Terminal**                                                                                                                                                             |
| `    Format_s(char* outFormatted, size_t destLen, const char* cpFormat )    `  |      int      | Formats a string that contains color formatting rules and replaces<br> the color formatting rules with co-responding ASCII sequences. retruns<br> the strlen() of the final formatted string. returns -1 when fails to allocate<br>memory or copy to outFormatted.                                                                             |
| ` FormatRemove_s(char* outFormatted, size_t destLen, const char* cpFormat ) `  |      int      | Formats a string that contains color formatting rules and removes<br> any color formatting rules from the string. returns the strlen() of the<br> final formatted string. returns -1 when fails to allocate<br>memory or copy to outFormatted.                                                                                                 |

-----------------



### Modification Preprocessors
**NOTE: all the preprocessors bellow most be defined before the inclusion of [EasyCLI.h](../include/EasyCLI.h) in order to take effect**

- `EC_OVERWRITE_PRINTF` - If defined, overwrites printf() from stdio.h and replaces it with EC_PrintColored(). **NOTE: stdio.h must be included before [EasyCLI.h](../include/EasyCLI.h)** 
- `EC_NO_PREFIX` - If defined, adds a definition for all the functions in [Function Table](#function-table) without the `EC_` prefix
  ```c
  #include "EasyCLI.h"
  int main(void){
        EC_Print("{R}Hello world{0} in red"); // Success, no problem
        Print("{R}Hello World{0} in red"); // ERROR: Undefined reference
  }
  ```
  ```c
    #define EC_NO_PREFIX
    #include "EasyCLI.h"
    int main(void){
        EC_Print("{R}Hello world{0} in red"); // Success, no problem
        Print("{R}Hello World{0} in red"); // Success, no problem
    }
   ```
  
### Compiling and building

for compiling you have 2 options:
- using [build.cmd](../build.cmd) (windows) or [build.sh](../build.sh) (linux);
- using Cmake with the provided [CmakeList.txt](../CMakeLists.txt)

the cmd/bash files use `GCC` for compiling and linking.<br>
[build.cmd](../build.cmd) and [build.sh](../build.sh) will both create static and dynamic libraries in the `build/` folder.


NOTE: gcc needs to be added to the PATH in windows in order for the script to work.



