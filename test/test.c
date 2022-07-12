#include <stdio.h>
#include "EasyCLI.h"



int main(void){
    char* testColors = "RGYBMCW";
    EC_SetColorMode(1);
    printf("a colored block in all of EasyCLIs 4bit colors");
    for (int i = 0; i < 8; i++){
        if (i % 3 == 0) printf("\n");
        EC_Print("{#%c}  {0}", testColors[i]);
    }
    EC_Print("\n{^}This is slow blinking{0} - {!}this is rapid blinking{0}");
    EC_Print("\n{*}This is BOLD{0} - {.}this is Faint{0}");
    EC_Print("\n{_}This is Undrlined{0} - {-}this is Striked{0}");
    EC_Print("\n{/}This is Italic{0}\n");
    for (int i = 0; i < 8; i++){
        EC_Print("{#%c}{%c}TEST{0}\n", testColors[i], testColors[7 - i]);
    }
    EC_ScreenSetForegroundRGB(255, 0, 0);
    printf("RGB (255, 0, 0)\n");
    EC_ScreenSetForegroundRGB(154, 200, 53);
    printf("RGB (154, 200, 53)\n");
    EC_ScreenSetForegroundRGB(20, 210, 255);
    printf("RGB (20, 210, 255)\n");
    EC_ScreenSetForegroundRGB(175, 65, 112);
    printf("RGB (175, 65, 112)\n");
    EC_ScreenSetForegroundINDX(145);
    printf("INDX 145\n");
    EC_ScreenSetForegroundINDX(23);
    printf("INDX 23\n");
    EC_ScreenSetForegroundINDX(202);
    printf("INDX 202\n");
    EC_ScreenSetForegroundINDX(2);
    printf("INDX 2\n");
    EC_Print("{0}Press any key to enable Alternative Buffer..\n");
    getchar();
    EC_ScreenEnableAlterBuff();
    printf("this is an alternative buffer!\npress any key to disable alternative buffer\n");
    getchar();
    EC_ScreenDisableAlterBuff();
    printf("Welcome back to the main buffer :)\n");
    EC_PrintHyperLink("This should be a google hyperlink", "https://www.google.com");
    printf("\npress any key to scroll up 10 times\n");
    getchar();
    EC_ScreenScrollUp(10);
    printf("press any key to scroll down 10 times\n");
    getchar();
    EC_ScreenScrollDown(10);
    printf("This is hello world in DEC line Drawing\\/\n");
    EC_ScreenEnableDecCharSet();
    printf("Hello World\n");
    EC_ScreenEnableAsciiCharSet();
    printf("Press any key to get a beep sound\n");
    getchar();
    EC_Beep();
    printf("press any key to soft reset");
    EC_SoftReset();
    return 0;
}
