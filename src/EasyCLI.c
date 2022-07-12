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

#include "../include/EasyCLI.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>


static bool EC_IsColorMode = false; // EC_SetColorMode() sets this value. if true, EC_Print() will print colored text.

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
EC_ResultEnum EC_SetColorMode(EC_ColorModeEnum mode){
    if ((mode != EC_COLORMODE_Enabled) & (mode != EC_COLORMODE_Disabled)) return EC_RES_InvalidArg;
    EC_IsColorMode = mode;
    return EC_RES_Success;
}
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
EC_ResultEnum EC_Print(char* cpFormat, ...){
    EC_ResultEnum res;
    va_list vaList;
    char* colorBuffer = NULL;
    char* vaBuffer    = NULL;

    va_start(vaList, cpFormat);
    int lenFormatted = vsnprintf(0, 0 , cpFormat, vaList);
    if (lenFormatted < 0){
        res = EC_RES_Failed;
        goto cleanup;
    }
    va_end(vaList);

    va_start(vaList, cpFormat);
    vaBuffer = (char*) malloc(++lenFormatted * sizeof(char));
    if (vaBuffer == NULL){
        res = EC_RES_FailedToAllocMem;
        goto cleanup;
    }
    vsnprintf(vaBuffer, lenFormatted, cpFormat, vaList);
    va_end(vaList);
    colorBuffer = (char*) calloc(lenFormatted + 256, sizeof(char));
    if (colorBuffer == NULL){
        res = EC_RES_FailedToAllocMem;
        goto cleanup;
    }
    if (EC_IsColorMode) {
        int result = EC_Format_s(colorBuffer, lenFormatted + 256, vaBuffer);
        if (result == -1){
            res = EC_RES_Failed;
            goto cleanup;
        }
        while (result != (int) strlen(colorBuffer)){
            colorBuffer = (char*) realloc(colorBuffer, (result + 512) * sizeof(char));
            if (colorBuffer == NULL){
                res = EC_RES_FailedToAllocMem;
                goto cleanup;
            }
            result = EC_Format_s(colorBuffer, (result + 512) * sizeof(char), vaBuffer);
            if (result == -1){
                res = EC_RES_Failed;
                goto cleanup;
            }
        }
    }else{
        if (EC_FormatRemove_s(colorBuffer, lenFormatted + 256, vaBuffer) == -1){
            res = EC_RES_Failed;
            goto cleanup;
        }
    }
    printf("%s", colorBuffer);
    res = EC_RES_Success;
    cleanup:
        free(vaBuffer);
        free(colorBuffer);
    return res;
}
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
EC_ResultEnum EC_PrintColored(char* cpFormat, ...){
    EC_ResultEnum res;
    va_list vaList;
    char* colorBuffer = NULL;
    char* vaBuffer    = NULL;

    va_start(vaList, cpFormat);
    int lenFormatted = vsnprintf(0, 0 , cpFormat, vaList);
    if (lenFormatted < 0){
        res = EC_RES_Failed;
        goto cleanup;
    }
    va_end(vaList);

    va_start(vaList, cpFormat);
    vaBuffer = (char*) malloc(++lenFormatted * sizeof(char));
    if (vaBuffer == NULL){
        res = EC_RES_FailedToAllocMem;
        goto cleanup;
    }
    vsnprintf(vaBuffer, lenFormatted, cpFormat, vaList);
    va_end(vaList);
    colorBuffer = (char*) calloc(lenFormatted + 256, sizeof(char));
    if (colorBuffer == NULL){
        res = EC_RES_FailedToAllocMem;
        goto cleanup;
    }
    int result = EC_Format_s(colorBuffer, lenFormatted + 256, vaBuffer);
    if (result == -1){
        res = EC_RES_Failed;
        goto cleanup;
    }
    while (result != (int) strlen(colorBuffer)){
        colorBuffer = (char*) realloc(colorBuffer, (result + 512) * sizeof(char));
        if (colorBuffer == NULL){
            res = EC_RES_FailedToAllocMem;
            goto cleanup;
        }
        result = EC_Format_s(colorBuffer, (result + 512) * sizeof(char), vaBuffer);
        if (result == -1){
            res = EC_RES_Failed;
            goto cleanup;
        }
    }

    printf("%s", colorBuffer);
    res = EC_RES_Success;
    cleanup:
    free(vaBuffer);
    free(colorBuffer);
    return res;
}
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
EC_ResultEnum EC_ClearScreen(EC_ClearScreenEnum mode){
    if (mode < 0 || mode > 3) return EC_RES_InvalidArg;
    printf(EC_CSI_ED, mode);
    return EC_RES_Success;
}
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
EC_ResultEnum EC_ClearLine(EC_ClearLineEnum mode){
    if (mode < 0 || mode > 2) return EC_RES_InvalidArg;
    printf(EC_CSI_EL, mode);
    return EC_RES_Success;
}

/* moves the cursor up by X lines
 * Parameter :
 *  - [0] int n -> move the cursor up n times
 *
 *  NO-RETURN
 */
void EC_CursorMoveUp(int n){
    printf(EC_CSI_CUU, n);
}
/* moves the cursor down by X lines
 * Parameter :
 *  - [0] int n -> move the cursor down n times
 *
 *  NO-RETURN
 */
void EC_CursorMoveDown(int n){
    printf(EC_CSI_CUD, n);
}
/* moves the cursor forward by X lines
 * Parameter :
 *  - [0] int n -> move the cursor forward n times
 *
 *  NO-RETURN
 */
void EC_CursorMoveForward(int n){
    printf(EC_CSI_CUF, n);
}
/* moves the cursor backward by X lines
 * Parameter :
 *  - [0] int n -> move the cursor backward n times
 *
 *  NO-RETURN
 */
void EC_CursorMoveBackward(int n){
    printf(EC_CSI_CUB, n);
}
/* moves the cursor to next X line.
 * Parameter :
 *  - [0] int n -> move the cursor to the n line after current line
 *
 *  NO-RETURN
 */
void EC_CursorMoveNextLine(int n){
    printf(EC_CSI_CNL, n);
}
/* moves the cursor to previous X lines
 * Parameter :
 *  - [0] int n -> move the cursor to the n line before current line
 *
 *  NO-RETURN
 */
void EC_CursorMovePrevLine(int n){
    printf(EC_CSI_CPL, n);
}
/* moves the cursor to the X column with keeping cursors row position
 * Parameter :
 *  - [0] int n -> move the cursor to column n without changing its vertical position
 *
 *  NO-RETURN
 */
void EC_CursorMoveHorizontalAbs(int n){
    printf(EC_CSI_CHA, n);
}

/* Hides the cursor.
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_CursorHide(void){
    printf(EC_CSI_DECTCEM_L);
}
/* Shows the cursor if hidden.
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_CursorShow(void)
{
    printf(EC_CSI_DECTCEM_H);
}
/* Enables the blinking action of the cursor
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_CursorEnableBlink(void) {
    printf(EC_CSI_TCEB);
}
/* disables the blinking action of the cursor
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_CursorDisableBlink(void){
    printf(EC_CSI_TCDB);
}
/* sets the position of cursor to Y row of the X column.
 *
 * Parameter :
 *  - [0] int row -> row to set the cursor to.
 *  - [1] int column -> column to set the cursor to.
 *  NO-RETURN
 */
void EC_CursorSetPos(int row, int column){
    printf(EC_CSI_CUP, row, column);
}
/* Saves the position of the cursor.
 * this save can be restored by EC_CursorRestorePos()
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_CursorSavePos(void){
    printf(EC_CSI_SCP);
}
/* Restores the cursor position saved by EC_CursorSavePos()
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_CursorRestorePos(void){
    printf(EC_CSI_RCP);
}

/* set the foreground color of the console, to a specific 16 to 24bit (RGB) color.
 *
 *  Parameters :
 *   -[0] unsigned char R -> the RED value of RGB from 0 to 255.
 *   -[1] unsigned char G -> the GREEN value of RGB from 0 to 255.
 *   -[2] unsigned char B -> the BLUE value of RGB from 0 to 255.
 *
 *  NO-RETURN
 */
void EC_ScreenSetForegroundRGB(unsigned char R, unsigned char G, unsigned char B) {
    printf(EC_CSI_FRGB, R, G, B);
}
/* set the background color of the console, to a specific 16 to 24bit (RGB) color.
 *
 *  Parameters :
 *   -[0] unsigned char R -> the RED value of RGB from 0 to 255.
 *   -[1] unsigned char G -> the GREEN value of RGB from 0 to 255.
 *   -[2] unsigned char B -> the BLUE value of RGB from 0 to 255.
 *
 *  NO-RETURN
 */
void EC_ScreenSetBackgroundRGB(unsigned char R, unsigned char G, unsigned char B){
    printf(EC_CSI_BRGB, R, G, B);
}
/* set the foreground color of the console, to a specific 8bit color index
 * see https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit for more info and the color table
 *
 *  Parameters :
 *   -[0] unsigned char index -> the index of the color in the color table from 0 to 255
 *
 *  NO-RETURN
 */
void EC_ScreenSetForegroundINDX(unsigned char index){
    printf(EC_CSI_FINDX, index);
}
/* set the background color of the console, to a specific 8bit color index
 * see https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit for more info and the color table
 *
 *  Parameters :
 *   -[0] unsigned char index -> the index of the color in the color table from 0 to 255
 *
 *  NO-RETURN
 */
void EC_ScreenSetBackgroundINDX(unsigned char index){
    printf(EC_CSI_BINDX, index);
}
/* enables the alternative screen buffer.
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_ScreenEnableAlterBuff(void){
    printf(EC_CSI_EASB);
}
/* restores the alternative screen buffer.
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_ScreenDisableAlterBuff(void){
    printf(EC_CSI_DASB);
}
/* resets the console character set to ASCII (better used after EC_ScreenEnableDecCharSet())
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_ScreenEnableAsciiCharSet(void){
    printf(EC_FE_ASCIILD);
}
/* enables the Dec line drawing mode
 * NOTE - For more info : https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#designate-character-set
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_ScreenEnableDecCharSet(void){
    printf(EC_FE_DECLD);
}
/* scrolls up the screen X times.
 *  Parameters :
 *   -[0] int n -> the scroll up n times.
 *  NO-RETURN
 */
void EC_ScreenScrollUp(int n) {
    printf(EC_CSI_SU, n);
}
/* scrolls down the screen X times.
 *  Parameters :
 *   -[0] int n -> the scroll down n times.
 *  NO-RETURN
 */
void EC_ScreenScrollDown(int n) {
    printf(EC_CSI_SD, n);
}

/* sets the title of the console window to the specified string.
 * NOTE - this is not supported by most terminals
 * Parameters :
 *  -[0] char* cpTitle -> the string to set the title to.
 *
 *  NO-RETURN
 */
void EC_SetWindowTitle(char* cpTitle){
    printf(EC_OSC_SWT, cpTitle);
}
/* resets most attributes of the console.
 * this may include : Cursor Visibility - Keypad mode - Console margins - Character set - Graphics Rendition - Saved cursor state
 * NOTE - For more info : https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#soft-reset
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_SoftReset(void){
    printf(EC_CSI_DECSTR);
}
/* makes a beep sound.
 * this sound defers based on your OS
 *  NO-PARAM
 *  NO-RETURN
 */
void EC_Beep(void){
    printf(EC_C0_BELL);
}
/* prints a hyperlinked text to the screen.
 * NOTE - This is not supported by all terminals.
 * Parameter :
 *  - [0] char* cpTitle -> the text that is going to be shown.
 *  - [1] char* cpLink  -> the link that the text is pointing to.
 *
 *  NO-RETURN
 */
void EC_PrintHyperLink(char* cpTitle, char* cpLink){
    printf(EC_OSC_HL, cpLink, cpTitle);
}

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
int EC_FormatRemove_s(char* outRemoved, size_t destLen, const char* cpFormat){
    int res;
    size_t currentIndex = 0;
    bool seqStarted = false;
    char* buffer = (char*) calloc(destLen, sizeof(char));
    if (buffer == NULL) {
        res = -1;
        goto cleanup;
    }
    char currentChar;
    for (size_t formatIndex = 0; formatIndex < strlen(cpFormat); formatIndex++){
        if (currentIndex+1 >= destLen) {
            buffer = (char *) realloc(buffer, (currentIndex + 2) * sizeof(char));
            if (buffer == NULL) {
                res = -1;
                goto cleanup;
            }
            buffer[currentIndex+1] = '\000';
        }
        currentChar = cpFormat[formatIndex];
        if (currentChar == EC_SEQUENCE_START) {
            if (seqStarted) {
                buffer[currentIndex] = EC_SEQUENCE_START;
                currentIndex++;
                seqStarted = false;
            } else seqStarted = true;
            continue;
        } else if (currentChar == EC_SEQUENCE_END) {
            if (!seqStarted) {
                buffer[currentIndex] = EC_SEQUENCE_END;
                currentIndex++;
            } else {
                seqStarted = false;
            }
            continue;
        }
        if (!seqStarted){
            buffer[currentIndex] = currentChar;
            currentIndex++;
        }
    }
    if (strlen(buffer) + 1 <= destLen) {
        if (strncpy(outRemoved, buffer, destLen) == NULL) {
            res = -1;
            goto cleanup;
        }
    }
    res = (int) strlen(buffer);
    cleanup:
        free(buffer);
    return res;
}
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
int EC_Format_s(char* outFormatted, size_t destlen, const char* cpFormat){
    int result;
    const size_t lenFormat = strlen(cpFormat);
    const int seqSize = EC_MAX_SEQUENCE;
    size_t workingIndex = 0;
    size_t workingBuffSize = destlen;
    char* workingBuffer = calloc(workingBuffSize, sizeof(char));
    char* seqBuffer     = calloc(EC_MAX_SEQUENCE, sizeof(char));
    char* colorChar     = calloc(5, sizeof(char));
    char currentColor   = 0;
    char currentChar;
    if (workingBuffer == NULL || seqBuffer == NULL || colorChar == NULL){
        result = -1;
        goto cleanup;
    }
    bool seqStarted = false;
    
    strncat(seqBuffer,  EC_FE_CSI, EC_MAX_SEQUENCE);
    for (size_t formatIndex = 0; formatIndex < lenFormat; formatIndex++)
    {
        currentChar = cpFormat[formatIndex];
        if (currentChar == EC_SEQUENCE_START) {
            if (seqStarted) {
                workingBuffer[workingIndex] = EC_SEQUENCE_START;
                workingIndex++;
                seqStarted = false;
            } else seqStarted = true;
            continue;
        } else if (currentChar == EC_SEQUENCE_END)
        {
            if (!seqStarted) {
                workingBuffer[workingIndex] = EC_SEQUENCE_END;
                workingIndex++;
            } else
            {
                seqStarted = false;
                if (currentColor > 30) {
                    snprintf(colorChar, 5 * sizeof(char), "%dm", currentColor);
                    strncat(seqBuffer, colorChar,seqSize - 1);
                } else {
                    size_t seqLen = strlen(seqBuffer);
                    if (seqBuffer[seqLen - 1] == ';'){
                        seqBuffer[seqLen - 1] = 'm';
                    }else {
                        seqBuffer[seqLen] = 'm';
                    }

                }
                if (strlen(workingBuffer) + strlen(seqBuffer) > workingBuffSize){
                    workingBuffSize = strlen(workingBuffer) + strlen(seqBuffer) + 1;
                    workingBuffer = (char*) realloc(workingBuffer, workingBuffSize * sizeof(char));
                }
                strcat(workingBuffer,  seqBuffer);
                workingIndex += strlen(seqBuffer);
                memset(seqBuffer, 0, seqSize * sizeof(char));
                strncat(seqBuffer, EC_FE_CSI, seqSize);
                currentColor = 0;
            }
            continue;
        }
        if (seqStarted){

            switch (currentChar) {
                case EC_COLOR_DEFAULT:
                    strncat(seqBuffer,  "40;", seqSize);
                    currentColor += EC_4BITCOLOR_FWhite;
                    break;
                case EC_COLOR_RESET:
                    currentColor = 0;
                    break;
                case EC_COLOR_RED:
                    currentColor += EC_4BITCOLOR_FRed;
                    break;
                case EC_COLOR_GREEN:
                    currentColor += EC_4BITCOLOR_FGreen;
                    break;
                case EC_COLOR_YELLOW:
                    currentColor += EC_4BITCOLOR_FYellow;
                    break;
                case EC_COLOR_BLUE:
                    currentColor += EC_4BITCOLOR_FBlue;
                    break;
                case EC_COLOR_MAGENTA:
                    currentColor += EC_4BITCOLOR_FMagent;
                    break;
                case EC_COLOR_CYAN:
                    currentColor += EC_4BITCOLOR_FCyan;
                    break;
                case EC_COLOR_WHITE:
                    currentColor += EC_4BITCOLOR_FWhite;
                    break;
                case EC_STYLE_BACKGROUND:
                    currentColor += 10;
                    break;
                case EC_STYLE_BRIGHT:
                    currentColor += 60;
                    break;
                case EC_STYLE_BOLD:
                    strncat(seqBuffer, "1;", seqSize);
                    break;
                case EC_STYLE_FAINT:
                    strncat(seqBuffer, "2;", seqSize);
                    break;
                case EC_STYLE_ITALIC:
                    strncat(seqBuffer, "3;", seqSize);
                    break;
                case EC_STYLE_UNDERLINE:
                    strncat(seqBuffer, "4;", seqSize);
                    break;
                case EC_STYLE_SLOWBLINK:
                    strncat(seqBuffer, "5;", seqSize);
                    break;
                case EC_STYLE_RAPIDBLINK:
                    strncat(seqBuffer, "6;", seqSize);
                    break;
                case EC_STYLE_STRIKE:
                    strncat(seqBuffer, "9;", seqSize);
                    break;
                default:
                    break;
            }
        }else{
            if (strlen(workingBuffer) + 2 > workingBuffSize){
                workingBuffSize = strlen(workingBuffer) + 2;
                workingBuffer = (char*) realloc(workingBuffer, workingBuffSize * sizeof(char));
                if (workingBuffer == NULL){
                    result = -1;
                    goto cleanup;
                }
                workingBuffer[workingIndex+1] = '\00';
            }
            workingBuffer[workingIndex] = currentChar;
            workingIndex++;
        }

    }
    if (workingIndex + 2 <= destlen){
        strncpy(outFormatted, workingBuffer, destlen);
    }

    result = (int) strlen(workingBuffer);
    cleanup:
        free(workingBuffer);
        free(seqBuffer);
        free(colorChar);
    return result;
}


#ifdef _WIN32
#include <windows.h>
// windows only functions

static DWORD EC_dwCosnoleOldMode; // EC_ConsoleEnableVTMode() saves the old console mode to this variable.

/* gets the cursor position using windows api functions in form of COORD struct.
 *
 * Parameters :
 *  -[0] COORD* outCoord -> a pointer to a COORD struct to write the cursor position to.
 *
 *  Return :
 *   - EC_ResultEnum [EC_RES_Failed, EC_RES_Success]
 */
EC_ResultEnum EC_CursorGetPosW(COORD* outCoord){
    CONSOLE_SCREEN_BUFFER_INFO ci;
    memset(&ci, 0, sizeof(ci));
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci)) return EC_RES_Failed;
    if (memcpy(outCoord, &ci.dwCursorPosition, sizeof(COORD)) == NULL) return EC_RES_Failed;

    return EC_RES_Success;
}
/* Sets color of the console without the need of VirtualTerminal (VT100).
 * this function uses the builtin windows "color" console function.
 *
 * Parameters :
 *  -[0] EC_WindowsColorsEnum/int nColor -> a color value based on windows color table.
 *
 *  Return :
 *   - EC_ResultEnum [EC_RES_Failed, EC_RES_Success]
 */
EC_ResultEnum EC_ScreenSetConsoleColorW(EC_WindowsColorsEnum nColor) {
    char colorArg[9];
    if (nColor == EC_WINDOWS_COLOR_RESET) {
        snprintf(colorArg, 9, "color 07");
        goto call_sys;
    }
    static int nOut[2];
    int place = 1;
    if (nColor & EC_WINDOWS_STYLE_BACKGROUND) place = 0;
    if (nColor & EC_WINDOWS_COLOR_BLACK)      nOut[place] = 0;
    if (nColor & EC_WINDOWS_COLOR_BLUE)       nOut[place] = 1;
    if (nColor & EC_WINDOWS_COLOR_GREEN)      nOut[place] = 2;
    if (nColor & EC_WINDOWS_COLOR_AQUA)       nOut[place] = 3;
    if (nColor & EC_WINDOWS_COLOR_RED)        nOut[place] = 4;
    if (nColor & EC_WINDOWS_COLOR_PURPLE)     nOut[place] = 5;
    if (nColor & EC_WINDOWS_COLOR_YELLOW)     nOut[place] = 6;
    if (nColor & EC_WINDOWS_COLOR_WHITE)      nOut[place] = 7;
    if (nColor & EC_WINDOWS_STYLE_INTENSITY)  nOut[place] += 8;


    snprintf(colorArg, 9, "color %x%x", nOut[0], nOut[1]);
    call_sys:
    if (system(colorArg) == -1) return EC_RES_Failed;
    return EC_RES_Success;
}
/* Enables VirtualTerminal Processing for the console. this is required in windows for parsing and using
 * ANSI Sequences.
 *
 *  NO-PARAM
 *  Return :
 *   - EC_ResultEnum [EC_RES_Failed, EC_RES_Success, EC_RES_NoHandle]
 */
EC_ResultEnum EC_ConsoleEnableVTMode(void){
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdout == NULL) return EC_RES_NoHandle;
    if (!GetConsoleMode(hStdout, &EC_dwCosnoleOldMode)) return EC_RES_Failed;
    if (!SetConsoleMode(hStdout, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) return EC_RES_Failed;
    return EC_RES_Success;
}
/* Restores the previous mode. this most be used after using EC_ConsoleEnableVTMode()
 *
 *  NO-PARAM
 *  Return :
 *   - EC_ResultEnum [EC_RES_Failed, EC_RES_Success, EC_RES_NoHandle]
 */
EC_ResultEnum EC_ConsoleResetMode(void){
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdout == NULL) return EC_RES_NoHandle;
    if (!SetConsoleMode(hStdout, EC_dwCosnoleOldMode)) return EC_RES_Failed;
    return EC_RES_Success;
}


#endif

