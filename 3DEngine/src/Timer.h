#pragma once

class Timer
{
public:
	Timer();
	~Timer();

	/** Seconds */
	float TotalTime() const;
	float DeltaTime() const;

	void Reset(); //Before message loop
	void Start(); //When unpaused
	void Stop();  //When paused
	void Tick();  //Every frame

	bool IsStopped() const { return m_Stopped; }

private:
	double m_SecondsPerCount;
	double m_DeltaTime;

	__int64 m_BaseTime;
	__int64 m_PausedTime;
	__int64 m_StopTime;
	__int64 m_PrevTime;
	__int64 m_CurrTime;

	bool m_Stopped;
};

