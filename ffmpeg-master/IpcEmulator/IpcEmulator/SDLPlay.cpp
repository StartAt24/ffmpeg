#include "SDLPlay.h"
#include "common_def.h"
#include <stdio.h>
#include <QWidget>
#include "mgr.h"

#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2.lib")

/*//SDL消息线程，目前没有能控制，open了之后 消息就一直上传了，导致不能控制播放流程；

//Refresh Event
#define  SFM_REFRESH_EVENT		 (SDL_USEREVENT + 1)
#define  SFM_BREAK_EVENT		 (SDL_USEREVENT + 2)

 int g_thread_exit = 0;
 int g_thread_pause = 0;

 
 int sfp_refresh_thread(void *opaque){

	 g_thread_exit = 0;
	 g_thread_pause = 0;

	 while (g_thread_exit == 0) {
		 if (!g_thread_pause){
			 SDL_Event event;
			 event.type = SFM_REFRESH_EVENT;
			 SDL_PushEvent(&event);
		 }
		 SDL_Delay(20);
	 }
	 //Quit
	 SDL_Event event;
	 event.type = SFM_BREAK_EVENT;
	 SDL_PushEvent(&event);
	 g_thread_exit = 0;
	 g_thread_pause = 0;
	 return 0;
 }
 */

CSDLPlay::CSDLPlay(CMgr *mgr)
{
	m_pMgr = mgr;
	SDLInit();
}


CSDLPlay::~CSDLPlay()
{
	SDLClose();
}

int CSDLPlay::SDLInit()
{
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		printf("SDL: could not initialize SDL - %s\n", SDL_GetError());
		return DJJ_FAILURE;
	}
	return DJJ_SUCCESS;
}

int CSDLPlay::SDLCreateWindow(int screen_width, int screen_height, HWND hWindow)
{
	m_iScreenW = screen_width;
	m_iScreenH = screen_height;

	//m_pScreen = SDL_CreateWindow("Test player!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	//	m_iScreenW, m_iScreenH, SDL_WINDOW_OPENGL);
	m_pScreen = SDL_CreateWindowFrom(hWindow);
	m_hWnd = hWindow;

	if (!m_pScreen)
	{
		printf("SDL: could not create window - exiting: %s\n", SDL_GetError());
		return DJJ_FAILURE;
	}

	m_pSdlRenderer = SDL_CreateRenderer(m_pScreen, -1, 0);
	m_pSdlTexture = SDL_CreateTexture(m_pSdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, m_iScreenW, m_iScreenH);

	m_tSdlRect.x = 0;
	m_tSdlRect.y = 0;
	m_tSdlRect.w = m_iScreenW;
	m_tSdlRect.h = m_iScreenH;

	//m_pVideo_tid = SDL_CreateThread(sfp_refresh_thread, NULL, NULL);

	return DJJ_SUCCESS;
}


int CSDLPlay::SDLShow(uint8_t* buf, int linsize)
{
//	SDL_WaitEvent(&m_uEvent);
//	if (m_uEvent.type == SFM_REFRESH_EVENT)
//	{
		//解码线程开始解码得到一帧数据，这边开始显示;
		SDL_UpdateTexture(m_pSdlTexture, NULL, buf, linsize);
		SDL_RenderClear(m_pSdlRenderer);
		SDL_RenderCopy(m_pSdlRenderer, m_pSdlTexture, NULL, NULL);
		SDL_RenderPresent(m_pSdlRenderer);
		if (m_pMgr->m_videoType == LOCAL_FILE)
		{
			if (40 - m_pMgr->m_iCodecCost > 0)
				SDL_Delay(40 - m_pMgr->m_iCodecCost);
		}
//	}
//	else if (m_uEvent.type == SDL_KEYDOWN)
//	{
		//Pause
//		if (m_uEvent.key.keysym.sym == SDLK_SPACE)
//			g_thread_pause = !g_thread_pause;	
//	}
//	else if (m_uEvent.type == SDL_QUIT)
//	{
//		g_thread_exit = 1;
//	}
//	else if (m_uEvent.type == SFM_BREAK_EVENT)
//	{
//		return DJJ_FAILURE;
//	}
	return DJJ_SUCCESS;
}

int CSDLPlay::SDLClose()
{
	if (m_pScreen)
	{
		SDL_DestroyWindow(m_pScreen);
	}
	SDL_Quit();
	return DJJ_SUCCESS;
}

int CSDLPlay::SDLCreateRenderer(int screen_width, int screen_height)
{
	m_pSdlRenderer = SDL_CreateRenderer(m_pScreen, -1, 0);
	m_pSdlTexture = SDL_CreateTexture(m_pSdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, screen_width, screen_height);
	return DJJ_SUCCESS;
}

int CSDLPlay::SDLDestery()
{
	QWidget * videoWidget = QWidget::find((WId)m_hWnd);
	SDL_DestroyTexture(m_pSdlTexture);
	SDL_DestroyRenderer(m_pSdlRenderer);
	//重新设定SDLWindowSize 不然会导致分辨率没有改变；
	SDL_SetWindowSize(m_pScreen, videoWidget->width(), videoWidget->height());
	return DJJ_SUCCESS;
}

int CSDLPlay::SDLFlashShow(uint8_t* buf, int linsize)
{
	SDL_UpdateTexture(m_pSdlTexture, NULL, buf, linsize);
	SDL_RenderClear(m_pSdlRenderer);
	SDL_RenderCopy(m_pSdlRenderer, m_pSdlTexture, NULL, NULL);
	SDL_RenderPresent(m_pSdlRenderer);
	return DJJ_SUCCESS;
}