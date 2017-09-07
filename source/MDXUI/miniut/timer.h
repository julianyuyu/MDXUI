#pragma once

#include "inc.h"

class MUTTimer
{
#define FPS_INTERVAL	1000 /*calc FPS per 1000ms */
public:
	MUTTimer() { Init(); }
	MUTTimer(const MUTTimer&) {}
	virtual ~MUTTimer() {}

	virtual int CalcFPS()
	{
		m_FrameCount++;

		LARGE_INTEGER CurrentTime;
		QueryPerformanceCounter(&CurrentTime);
		__int64 delta = CurrentTime.QuadPart - m_LastTimeFPS.QuadPart;
		float t = (float)delta / m_TickCountPerMS;
		if ((int)t > FPS_INTERVAL)
		{
			//if (t > 500.f)
			//	t = 500.f; // for the first time, lasttime == 0, then t will be large.
			m_FPS = (int)((float)(m_FrameCount * 1000) / t);
			m_LastTimeFPS = CurrentTime;
			m_FrameCount = 0;
		}
		return m_FPS;
	}

	virtual float CalcFrameTimeS()
	{
		return CalcFrameTimeMS()/1000.f;
	}

	virtual float CalcFrameTimeMS()
	{
		LARGE_INTEGER CurrentTime;
		QueryPerformanceCounter(&CurrentTime);
		//ULONG delta = (CurrentTime.LowPart - m_LastTime.LowPart);
		__int64 delta = CurrentTime.QuadPart - m_LastTime.QuadPart;
		m_LastTime = CurrentTime;
		float t = (float)delta / m_TickCountPerMS;
		if (t > 50000.f)
			t = 50000.f; // for the first time, lasttime == 0, then t will be large.
		return t;
	}

private:
	virtual bool Init()
	{
		m_FPS = 0;
		m_FrameCount = 0;
		QueryPerformanceFrequency(&m_Frequency);
		m_TickCountPerMS = (float)(m_Frequency.QuadPart) / 1000.f;
		m_LastTime.QuadPart = 0;
		m_LastTimeFPS.QuadPart = 0;
		//QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);
		return true;
	}
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_LastTime;
	float m_TickCountPerMS;

	// for FPS calc
	int m_FrameCount;
	int m_FPS;
	LARGE_INTEGER m_LastTimeFPS;
};
