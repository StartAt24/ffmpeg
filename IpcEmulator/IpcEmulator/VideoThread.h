#pragma once
#include <QThread>
#include <QString>

#include "SDLPlay.h"
#include "Cffmpeg.h"
#include "RcvFrame.h"
#include "GiveFrame.h"
#include "DJJQue.h"
//#include <VehicleSnapApi.h>

class CVideoThread :
	public QThread
{
	Q_OBJECT
public:
	explicit CVideoThread();
	~CVideoThread();
	void SetVideoFile(QString filepath);
	void SetWindow(HWND hwnd);
	void ThreadInit();
protected:
	void run();
	int  DecodeAndShow(QString videofile);
	int  GiveFrame(unsigned char *yuvBuf);
private:
	QString			m_sVideoFile;
	volatile bool	m_bRunning;
	bool			m_bPause;
	bool			m_bDecoding;
	bool			m_bExitDecode;
	Cffmpeg			*m_pFFmpeg;
	CSDLPlay		*m_pSDLPlay;
	HWND			m_hWnd;
	CGiveFrame		*m_pGive;
	CRcvFrame		*m_pRcv;
	DJJQue			*m_pQue;
};

