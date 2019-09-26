#include <debugnet.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/ctrl.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <stdio.h>
#include "debugScreen.h"

#define ip_server "192.168.1.9"
#define port_server 18194

#define stickDeadzone 10

#define printf psvDebugScreenPrintf
extern debugNetConfiguration *dconfig;

const float stickMultiplier = (float)101/(128-stickDeadzone);
int rc(int a);

int main()
{
	int32_t on;
    psvDebugScreenInit();
    debugNetInit(ip_server,port_server,DEBUG);
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
    SceCtrlData ctrl;
    sceNetSetsockopt(dconfig->SocketFD, SCE_NET_SOL_SOCKET, SCE_NET_SO_BROADCAST, (const void*)&on, sizeof(on));
    debugNetUDPPrintf("%s\n","command");
    printf("Hello\n");
    /*printf("world\n");
    printf("\rworld!\n");
    printf("\e[2Eworld!\n");*/
    do{
        sceKernelDelayThread(4000*1000);
        unsigned int previousButtons = ctrl.buttons;
        sceCtrlPeekBufferPositive(0, &ctrl, 1);
        unsigned int pressedButtons = ((previousButtons ^ ctrl.buttons) & ctrl.buttons); //determine which buttons have just been depressed; 1 for pressed, 0 for anything else
        char buf[51];
        int len = sceNetRecv(dconfig->SocketFD, buf, sizeof(buf)/sizeof(char)-1, 0);
        printf("\r%i %s", len, buf);
    }while(ctrl.buttons != (SCE_CTRL_START | SCE_CTRL_SELECT));
    
	debugNetFinish();
	sceKernelExitProcess(0);
	return 0;
}

int rc(int a){
    if(a < 128 + stickDeadzone && a >= 128 - stickDeadzone) return 0;
    if(a < 128) return stickMultiplier * (a - 128 + 1 + stickDeadzone);
    if(a >= 128) return stickMultiplier * (a - 128 - stickDeadzone);
}
