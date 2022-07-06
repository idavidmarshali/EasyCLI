/*
MIT License
Copyright (c) 2022 DavidMarshal (https://github.com/idavidmarshali)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#ifndef EASYCLI_H
#define EASYCLI_H

#include <stddef.h>

#define EC_MAX_SEQUENCE     24
#define EC_SEQUENCE_START   '{'
#define EC_SEQUENCE_END     '}'

// -------------------
// C0 control codes, the root of all sequences.
#define EC_C0_BELL "\x7"  // "\a" can also be used (is also used as String terminator in some sequences)
#define EC_C0_BS   "\x8"  // backspace
#define EC_C0_LF   "\x0A" // Line feed (\n)
#define EC_C0_FF   "\x0C" // Form Feed
#define EC_C0_CR   "\x0D" // Carriage Return
#define EC_C0_ESC  "\x1B" // the ESC character, starter of all escape sequences.
// -------------------
// Fe Escape sequences (C1 Control codes)  (https://en.wikipedia.org/wiki/ANSI_escape_code#Fe_Escape_sequences)
#define EC_FE_CSI     EC_C0_ESC"["       // Control Sequence Introducer
#define EC_FE_DECLD   EC_C0_ESC"(0"      // Enable Dec Line Drawing
#define EC_FE_ASCIILD EC_C0_ESC"(B"      // Enable Ascii Line Drawing
#define EC_FE_OSC     EC_C0_ESC"]"       // Operating system command
#define EC_FE_ST      EC_C0_ESC"\\"      // String Terminator
// -------------------
#define EC_OSC_HL EC_FE_OSC"8;;%s"EC_FE_ST"%s"EC_FE_OSC"8;;"EC_FE_ST // HyperLink - Not Supported by all terminals!
#define EC_OSC_SWT EC_FE_OSC"0;%s"EC_C0_BELL // ESC ] 0; <string> ST (Set Window Title)
// -------------------
// all of the "Control Sequence Introducer" sequences. (https://en.wikipedia.org/wiki/ANSI_escape_code#CSI_(Control_Sequence_Introducer)_sequences)
// they all start with CSI ("ESC [")
#define EC_CSI_ED  EC_FE_CSI"%dJ"     // CSI n J    (ED) Erase Display
#define EC_CSI_EL  EC_FE_CSI"%dK"     // CSI n J    (EL) Erase Line
#define EC_CSI_CUU EC_FE_CSI"%dA"     // CSI n A    (CUU) Cursor UP
#define EC_CSI_CUD EC_FE_CSI"%dB"     // CSI n B    (CUD) Cursor DOWN
#define EC_CSI_CUF EC_FE_CSI"%dC"     // CSI n C    (CUF) Cursor FORWARD
#define EC_CSI_CUB EC_FE_CSI"%dD"     // CSI n D    (CUB) Cursor BACKWARD
#define EC_CSI_CUP EC_FE_CSI"%d;%dH"  // CSI n;m H  (CUP) Cursor Position
#define EC_CSI_SCP EC_FE_CSI"s"       // CSI s      (SCP) Save Cursor Position
#define EC_CSI_RCP EC_FE_CSI"u"       // CSI u      (RCP) Load Cursor Position
#define EC_CSI_SU EC_FE_CSI"%dS"      // CSI n S    (SU) Scroll UP
#define EC_CSI_SD EC_FE_CSI"%dT"      // CSI n T    (SD) Scroll DOWN
#define EC_CSI_FRGB EC_FE_CSI"38;2;%d;%d;%dm"    // CSI 38;2;<R>;<G>;<B> (Foreground 16bit - RGB) Set FG 16bit
#define EC_CSI_BRGB EC_FE_CSI"48;2;%d;%d;%dm"    // CSI 48;2;<R>;<G>;<B> (Background 16bit - RGB) Set BG 16bit
#define EC_CSI_FINDX EC_FE_CSI"38;5;%dm"         // CSI 38;5;<INDEX>     (Foreground 8bit - 256 Color Index)
#define EC_CSI_BINDX EC_FE_CSI"48;5;%dm"         // CSI 48;5;<INDEX>     (Background 8bit - 256 Color Index)
#define EC_CSI_TCEB EC_FE_CSI"[?12h" // CSI ? 12 h (ATT160) Cursor Enable Blinking
#define EC_CSI_TCDB EC_FE_CSI"[?12l" // CSI ? 12 l (ATT160) Cursor Disable Blinking

#define EC_CSI_CNL EC_FE_CSI"%dE"     // CSI n E (CNL) Cursor Move Newline
#define EC_CSI_CPL EC_FE_CSI"%dF"     // CSI n F (CPL) Cursor Move PrevLine
#define EC_CSI_CHA EC_FE_CSI"%dG"     // CSI n G (CHA) Cursor Horizontal Absolute
#define EC_CSI_VPA EC_FE_CSI"%dd"     // CSI n G (CHA) Vertical Line Position Absolute
#define EC_CSI_EASB EC_FE_CSI"?1049h"       // CSI n G (CHA) Enable Screen ALTBUFF
#define EC_CSI_DASB EC_FE_CSI"?1049l"       // CSI n G (CHA) Disable Screen ALTBUFF
#define EC_CSI_DECTCEM_H EC_FE_CSI"?25h"    // CSI ? 25 h (DECTCEM) Cursor Show
#define EC_CSI_DECTCEM_L EC_FE_CSI"?25l"    // CSI ? 25 l (DECTCEM) Cursor Hide
#define EC_CSI_DECSTR EC_FE_CSI"!p"         // CSI n T    (SD) Soft Reset

#define EC_COLOR_DEFAULT    'D' // Foreground white, background black
#define EC_COLOR_RED        'R'
#define EC_COLOR_GREEN      'G'
#define EC_COLOR_YELLOW     'Y'
#define EC_COLOR_BLUE       'B'
#define EC_COLOR_MAGENTA    'M'
#define EC_COLOR_CYAN       'C'
#define EC_COLOR_WHITE      'W'
#define EC_COLOR_RESET      '0'
// -------------------
#define EC_STYLE_BACKGROUND '#'
#define EC_STYLE_BOLD       '*'
#define EC_STYLE_ITALIC     '/'
#define EC_STYLE_UNDERLINE  '_'
#define EC_STYLE_STRIKE     '-'
#define EC_STYLE_SLOWBLINK  '^'
#define EC_STYLE_RAPIDBLINK '!'
#define EC_STYLE_FAINT      '.'
#define EC_STYLE_BRIGHT     '$'
#define EC_STYLE_RESET      '0'

// --- {ENUM DEF} ---

typedef enum EC_4BITCOLOR {
    EC_4BITCOLOR_FBlack = 30,
    EC_4BITCOLOR_FRed,
    EC_4BITCOLOR_FGreen,
    EC_4BITCOLOR_FYellow,
    EC_4BITCOLOR_FBlue,
    EC_4BITCOLOR_FMagent,
    EC_4BITCOLOR_FCyan,
    EC_4BITCOLOR_FWhite,
    EC_4BITCOLOR_BBlack = 40,
    EC_4BITCOLOR_BRed,
    EC_4BITCOLOR_BGreen,
    EC_4BITCOLOR_BYellow,
    EC_4BITCOLOR_BBlue,
    EC_4BITCOLOR_BMagent,
    EC_4BITCOLOR_BCyan,
    EC_4BITCOLOR_BWhite
} EC_4BITCOLOR;
typedef enum EC_ResultEnum {
    EC_RES_Success   = 0,
    EC_RES_Failed    = 1,
    EC_RES_NoHandle  = 2,
    EC_RES_FailedToAllocMem  = 3,
    EC_RES_InvalidArg = 4
} EC_ResultEnum;
typedef enum EC_ColorModeEnum {
    EC_COLORMODE_Disabled = 0,
    EC_COLORMODE_Enabled  = 1
} EC_ColorModeEnum;
typedef enum EC_ClearScreenEnum {
    EC_CLEARSCREEN_CursorToEnd   = 0,    //  Clear from cursor to the End of the screen
    EC_CLEARSCREEN_CursorToStart = 1,    //  Clear from cursor to the beginning of the screen;
    EC_CLEARSCREEN_ClearScreen   = 2,    //  Clear the whole screen
    EC_CLEARSCREEN_ClearScrollBuffer = 3 //  Clear the scroll buffer
} EC_ClearScreenEnum;
typedef enum EC_ClearLineEnum {
    EC_CLEARLINE_CursorToEnd   = 0,
    EC_CLEARLINE_CursorToStart = 1,
    EC_CLEARLINE_ClearLine     = 2
} EC_ClearLineEnum;

// --- {FUNCTION DEF}
/* prints color formatted string to the screen.
 * it only parses the color formatting rules if EC_SetColorMode() has been called with EC_COLORMODE_Enabled.
 * else it will remove all color formatting rules from the string.
 *
 * Parameters :
 *  - [0] char* cpFormat -> the string containing standard formatting rules + color formatting rules.
 *  - [1] ... -> variable number of arguments to format the string with
 *
 *  Return :
 *   - EC_ResultEnum [EC_RES_Success, EC_RES_Failed, EC_RES_FailedToAllocMem]
 */
EC_ResultEnum EC_Print(char* cpFormat, ...);
/* prints color formatted string to the screen.
 * this function parses and prints colored formatted string regardless of the color mode.
 *
 * Parameters :
 *  - [0] char* cpFormat -> the string containing standard formatting rules + color formatting rules.
 *  - [1] ... -> variable number of arguments to format the string with
 *
 *  Return :
 *   - EC_ResultEnum [EC_RES_Success, EC_RES_Failed, EC_RES_FailedToAllocMem]
 */
EC_ResultEnum EC_PrintColored(char* cpFormat, ...);
/* sets the color mode of EC_Print(), if mode is 1, enables color parsing of EC_Print().
 * if 0 Disables color parsing and instead, EC_Print() removes all colors from the format
 * NOTE - EC_PrintColored() doesn't check the color mode and always parses.
 *
 * Parameters :
 *  - [0] EC_ColorModeEnum mode -> EC_COLORMODE_Enabled or EC_COLORMODE_Disabled
 *
 *  Return :
 *   - EC_ResultEnum [EC_RES_Success, EC_RES_InvalidArg]
 * */
EC_ResultEnum EC_SetColorMode(EC_ColorModeEnum mode);
/* Clears all of the screen or parts of it based on the mode passed to the function.
 *
 * Parameters :
 *  - [0] EC_ClearScreenEnum mode
 *     - EC_CLEARSCREEN_CursorToEnd       (0) clear from the cursor position to end of the screen.
 *     - EC_CLEARSCREEN_CursorToStart     (1) clear from the cursor position to start of the screen.
 *     - EC_CLEARSCREEN_ClearScreen       (2) clear the whole screen.
 *     - EC_CLEARSCREEN_ClearScrollBuffer (3) clear the screens scroll buffer (this won't clear the screen)
 *
 * Return :
 *  - EC_ResultEnum [EC_RES_Success, EC_RES_InvalidArg]
 */
EC_ResultEnum EC_ClearScreen(EC_ClearScreenEnum mode);
/* Clears the line that the cursor is currently on.
 *
 * Parameters :
 *  - [0] EC_ClearLineEnum mode
 *     - EC_CLEARLINE_CursorToEnd   (0) clear from the cursor position to end of the line.
 *     - EC_CLEARLINE_CursorToStart (1) clear from the cursor position to start of the line.
 *     - EC_CLEARLINE_ClearLine     (2) clear the whole line
 *
 * Return :
 *  - EC_ResultEnum [EC_RES_Success, EC_RES_InvalidArg]
 */
EC_ResultEnum EC_ClearLine(EC_ClearLineEnum mode);

// --- {Cursor Related Functions} ---
/* moves the cursor up by X lines
 * Parameter :
 *  - [0] int count -> the count of lines to move the cursor up by.
 *
 *  NO-RETURN
 */
void EC_CursorMoveUp(int count);
/* moves the cursor down by X lines
 * Parameter :
 *  - [0] int count -> the count of lines to move the cursor down by.
 *
 *  NO-RETURN
 */
void EC_CursorMoveDown(int count);
/* moves the cursor forward by X lines
 * Parameter :
 *  - [0] int count -> the count of lines to move the cursor forward by.
 *
 *  NO-RETURN
 */
void EC_CursorMoveForward(int count);
/* moves the cursor backward by X lines
 * Parameter :
 *  - [0] int count -> the count of lines to move the cursor backward by.
 *
 *  NO-RETURN
 */
void EC_CursorMoveBackward(int count);
/* moves the cursor to next X line.
 * Parameter :
 *  - [0] int count -> the count of lines to go forward too.
 *
 *  NO-RETURN
 */
void EC_CursorMoveNextLine(int count);
/* moves the cursor to previous X lines
 * Parameter :
 *  - [0] int count -> the count of lines to go back too.
 *
 *  NO-RETURN
 */
void EC_CursorMovePrevLine(int count);
/* moves the cursor to the X column with keeping cursors row position
 * Parameter :
 *  - [0] int column -> the column to move to.
 *
 *  NO-RETURN
 */
void EC_CursorMoveHorizontalAbs(int column);
/* Hides the cursor.
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_CursorHide(void);
/* Shows the cursor if hidden.
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_CursorShow(void);
/* Enables the blinking action of the cursor
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_CursorEnableBlink(void);
/* disables the blinking action of the cursor
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_CursorDisableBlink(void);
/* sets the position of cursor to Y row of the X column.
 *
 * Parameter :
 *  - [0] int row -> row to set the cursor to.
 *  - [1] int column -> column to set the cursor to.
 *  NO-RETURN
 */
void EC_CursorSetPos(int row, int column);
/* Saves the position of the cursor.
 * this save can be restored by EC_CursorRestorePos()
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_CursorSavePos(void);
/* Restores the cursor position saved by EC_CursorSavePos()
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_CursorRestorePos(void);

// --- {Console Screen Related Functions} ---
/* set the foreground color of the console, to a specific 16 to 24bit (RGB) color.
 *
 *  Parameters :
 *   -[0] unsigned char R -> the RED value of RGB from 0 to 255.
 *   -[1] unsigned char G -> the GREEN value of RGB from 0 to 255.
 *   -[2] unsigned char B -> the BLUE value of RGB from 0 to 255.
 *
 *  NO-RETURN
 */
void EC_ScreenSetForegroundRGB(unsigned char R, unsigned char G, unsigned char B);
/* set the background color of the console, to a specific 16 to 24bit (RGB) color.
 *
 *  Parameters :
 *   -[0] unsigned char R -> the RED value of RGB from 0 to 255.
 *   -[1] unsigned char G -> the GREEN value of RGB from 0 to 255.
 *   -[2] unsigned char B -> the BLUE value of RGB from 0 to 255.
 *
 *  NO-RETURN
 */
void EC_ScreenSetBackgroundRGB(unsigned char R, unsigned char G, unsigned char B);
/* set the foreground color of the console, to a specific 8bit color index
 * see https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit for more info and the color table
 *
 *  Parameters :
 *   -[0] unsigned char index -> the index of the color in the color table from 0 to 255
 *
 *  NO-RETURN
 */
void EC_ScreenSetForegroundINDX(unsigned char index);
/* set the background color of the console, to a specific 8bit color index
 * see https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit for more info and the color table
 *
 *  Parameters :
 *   -[0] unsigned char index -> the index of the color in the color table from 0 to 255
 *
 *  NO-RETURN
 */
void EC_ScreenSetBackgroundINDX(unsigned char index);
/* enables the alternative screen buffer.
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_ScreenEnableAlterBuff(void);
/* restores the alternative screen buffer.
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_ScreenDisableAlterBuff(void);
/* resets the console character set to ASCII (better used after EC_ScreenEnableDecCharSet())
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_ScreenEnableAsciiCharSet(void);
/* enables the Dec line drawing mode
 * NOTE - For more info : https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#designate-character-set
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_ScreenEnableDecCharSet(void);
/* scrolls up the screen X times.
 *  Parameters :
 *   -[0] int count -> the scroll up count.
 *  NO-RETURN
 */
void EC_ScreenScrollUp(int count);
/* scrolls down the screen X times.
 *  Parameters :
 *   -[0] int count -> the scroll down count.
 *  NO-RETURN
 */
void EC_ScreenScrollDown(int count);

// --- {Other Functions} ---
/* sets the title of the console window to the specified string.
 * NOTE - this is not supported by most terminals
 * Parameters :
 *  -[0] char* cpTitle -> the string to set the title to.
 *
 *  NO-RETURN
 */
void EC_SetWindowTitle(char* cpTitle);
/* resets most attributes of the console.
 * this may include : Cursor Visibility - Keypad mode - Console margins - Character set - Graphics Rendition - Saved cursor state
 * NOTE - For more info : https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#soft-reset
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_SoftReset(void);
/* makes a beep sound.
 * this sound defers based on your OS
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_Beep(void);
/* prints a hyperlinked text to the screen.
 * NOTE - This is not supported by all terminals.
 * Parameter :
 *  - [0] char* cpTitle -> the text that is going to be shown.
 *  - [1] char* cpLink  -> the link that the text is pointing to.
 *
 *  NO-RETURN
 */
void EC_PrintHyperLink(char* cpTitle, char* cpLink);

// --- {Formatting Functions} ---
/* a safe function that removes the color formatting of the given string
 *
 * Parameter :
 *  -[0] {OUT} char* outRemoved -> the output buffer that the formatted string will be stored in
 *  -[1] {IN}  size_t destLen -> length of outRemoved in characters.
 *  -[2] {IN}  const char* cpFormat -> the string containing color formatting rules.
 *
 *  Return :
 *   - int -> the length of the formatted string. if outRemoved is smaller than destLen you can use this to reallocate
 *   the buffer and try again.
 */
int EC_Format_s(char* outFormatted, size_t destlen, const char* cpFormat);
/* a safe function that replaces color formatting rules in a string by there co-responding ANSI Sequence.
 *
 * Parameter :
 *  -[0] {OUT} char* outRemoved -> the output buffer that the formatted string will be stored in
 *  -[1] {IN}  size_t destLen -> length of outRemoved in characters.
 *  -[2] {IN}  const char* cpFormat -> the string containing color formatting rules.
 *
 *  Return :
 *   - int -> the length of the formatted string. if outRemoved is smaller than destLen you can use this to reallocate
 *   the buffer and try again.
 */
int EC_FormatRemove_s(char* outRemoved, size_t destLen, const char* cpFormat);


#ifdef _WIN32
#include <windows.h>

// --- {WINDOWS ONLY ENUMS} ---
typedef enum EC_WindowsColorsEnum {
    EC_WINDOWS_COLOR_RESET  = 0x0,
    EC_WINDOWS_COLOR_BLACK  = 0x1,
    EC_WINDOWS_COLOR_BLUE   = 0x2,
    EC_WINDOWS_COLOR_GREEN  = 0x4,
    EC_WINDOWS_COLOR_AQUA   = 0x8,
    EC_WINDOWS_COLOR_RED    = 0x10,
    EC_WINDOWS_COLOR_PURPLE = 0x20,
    EC_WINDOWS_COLOR_YELLOW = 0x40,
    EC_WINDOWS_COLOR_WHITE  = 0x80,
    EC_WINDOWS_STYLE_INTENSITY  = 0x100,
    EC_WINDOWS_STYLE_BACKGROUND = 0x200
} EC_WindowsColorsEnum;

// --- {WINDOWS ONLY FUNCTIONS} ---
/* gets the cursor position using windows api functions in form of COORD struct.
 *
 * Parameters :
 *  -[0] COORD* outCoord -> a pointer to a COORD struct to write the cursor position to.
 *
 *  Return :
 *   - EC_ResultEnum [EC_RES_Failed, EC_RES_Success]
 */
EC_ResultEnum EC_CursorGetPosW(COORD* outCoord);
/* Enables VirtualTerminal Processing for the console. this is required in windows for parsing and using
 * ANSI Sequences.
 *
 *  NO-PARAM
 *  Return :
 *   - EC_ResultEnum [EC_RES_Failed, EC_RES_Success, EC_RES_NoHandle]
 */
EC_ResultEnum EC_ConsoleEnableVTMode(void);
/* Restores the previous mode. this most be used after using EC_ConsoleEnableVTMode()
 *
 *  NO-PARAM
 *  Return :
 *   - EC_ResultEnum [EC_RES_Failed, EC_RES_Success, EC_RES_NoHandle]
 */

EC_ResultEnum EC_ConsoleResetMode(void);
/* Sets color of the console without the need of VirtualTerminal (VT100).
 * this function uses the builtin windows "color" console function.
 *
 * Parameters :
 *  -[0] EC_WindowsColorsEnum/int nColor -> a color value based on windows color table.
 *
 *  Return :
 *   - EC_ResultEnum [EC_RES_Failed, EC_RES_Success]
 */
EC_ResultEnum EC_ScreenSetConsoleColorW(EC_WindowsColorsEnum nColor);

#endif

#endif //EASYCLI_H