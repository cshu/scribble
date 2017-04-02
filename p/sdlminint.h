#pragma once
#include "SDL.h"

static int numofmodules_using_sdl;//is_sdl_init_already_called_successfully;
//not thread safe
#define CALL_SDL_Init_N_SETINT_IF_SUCCESSs(arg0,caseFail,caseSucceed) if(SDL_Init(arg0)){caseFail}else{numofmodules_using_sdl=1;caseSucceed}

static int numofmodules_using_sdlnet;//is_sdlnet_init_already_called_successfully;
//not thread safe
#define CALL_SDLNet_Init_N_SETINT_IF_SUCCESSs(caseFail,caseSucceed) if(SDLNet_Init()){caseFail}else{numofmodules_using_sdlnet=1;caseSucceed}



