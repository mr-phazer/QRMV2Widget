#pragma once

#include <vector>
#include <Windows.h>

namespace SystemLib
{
	class system_clock
	{
	public:

		system_clock(float _max_time)
		{
			m_max_time = _max_time;
			initController();
			resetLocalTime();
		}

		system_clock()
		{
			initController();
			initController();
			resetLocalTime();
		}

		void setFrameCount(size_t _frame_count)
		{
			m_frame_count = _frame_count;

			/*if(m_poSlider)
			{
				m_poSlider->setMinimum(0);
				m_poSlider->setMinimum(_frame_count);
			}*/
		}

		void setFrameRate(float _frame_rate)
		{
			m_frame_rate = static_cast<float>(_frame_rate);
		}

		void setMaxTime(float _max_time)
		{
			m_max_time = _max_time;
			//	resetLocalTime();
		}

		void setIncrement(float _inc_time)
		{
			m_increment_time = _inc_time;
			//	resetLocalTime();
		}

		void setTime(float _time)
		{
			m_stop_time = _time;
			m_bStopFlag = true;
		}

		void pause(float _add = 0)
		{
			if (m_bStopFlag) return;

			m_stop_time = this->getLocalTime();

			m_bStopFlag = true;
		}

		void stop()
		{
			m_stop_time = 0;
			m_bStopFlag = true;
		}

		bool isStopped()
		{
			return m_bStopFlag;
		}

		void resume()
		{
			if (!m_bLoop)
			{
				resetLocalTime(0);
				m_bStopFlag = false;
				return;
			}

			resetLocalTime(m_stop_time);
			m_bStopFlag = false;
		}

		void skipFrameForward()
		{
			if (!m_bStopFlag)
			{
				m_stop_time = getLocalTime();
				m_bStopFlag = true;
			}
			else
			{
				m_stop_time += (1.0f / m_frame_rate);
			}
		}

		void skipFrameBackward()
		{
			if (!m_bStopFlag)
			{
				m_stop_time = getLocalTime();
				m_bStopFlag = true;
			}
			else
			{
				m_stop_time -= (1.0f / m_frame_rate); // go a frame back

				if (m_stop_time < 0)
				{
					// if "frame time" < 0
					// set frame time to "end time" + "the amount of time < 0"
					m_stop_time = (float)m_frame_count * (1.0f / m_frame_rate) + abs(m_stop_time - ((int)m_stop_time));
				}
			}
		}

		static void initController();

		static float getSeconds();
		static float getSecondsSinceAppStart();

		float frameLength()
		{
			if (m_frame_rate == 0.0f || m_frame_rate < 0.0f)
				return 0.0f;

			return (1.0f / m_frame_rate);
		}

		float animLength()
		{
			if (m_frame_rate == 0.0f || m_frame_rate < 0.0f)
				return 0.0f;

			return (1.0f / m_frame_rate) * (float)m_frame_count;
		}

		void toEnd()
		{
			// set the animation at the end - 1/20 framelength´
			resetLocalTime(animLength() - frameLength() * 1.1f);
			m_stop_time = animLength() - frameLength() * 1.1f;
		}

		void toStart()
		{
			resetLocalTime(0);
			m_stop_time = 0;
		}

		// get the current local time
		float animTimePosition()
		{
			if (m_frame_count < 1)
				return 0.0f;

			// make sure to always restart a 0

			float fAnimLen = ((float)(m_frame_count - 1.0f) * (1.0f / m_frame_rate));

			float time = (m_bStopFlag) ? (m_stop_time) : getLocalTime(); // get local time / the stopped timed

			int number_of_anim_lenghts = static_cast<int>(time / fAnimLen); // trucante number of anim lengths

			if (number_of_anim_lenghts > 0)
				int _debug = 1;

			float local_time = time - number_of_anim_lenghts * fAnimLen;

			if (!m_bLoop && local_time >= fAnimLen)
			{
				m_bEndReached = true;
				if (!m_bStopFlag)
					pause();

				return fAnimLen - frameLength() / 5;
			}

			if (local_time < 0)
			{
				local_time = fAnimLen + local_time;
				int i = 1;
			}
			//m_poSlider->setSliderPosition((int)local_time);

			return local_time; // substract  time to get time in "local anim time"
		}

		// start the timer ass the specificed time
		void resetLocalTime(float _start_at = 0.0)
		{
			m_start_time = getSeconds() - _start_at;
		}

		float getLocalTime()
		{
			float time_elapsed = getSeconds() - m_start_time; // +m_start_at;

			//if (m_max_time > 0)
			//{
			//	float substract = trunc(time_elapsed / m_max_time) * m_max_time;
			//	return time_elapsed - substract;
			//}
			//else
			{
				return time_elapsed;
			}
		}

		size_t getFrameCount()
		{
			return m_frame_count;
		}

		float getFrameRate()
		{
			return m_frame_rate;
		}

		void setLoop(bool _bState)
		{
			m_bLoop = _bState;
		}

		bool loop()
		{
			return m_bLoop;
		}

		bool endReached()
		{
			return m_bEndReached;
		}

	protected:
		float m_start_time = 0; // reference point

		float m_stop_time = 0;

		float m_max_time = 0;
		float m_increment_time = 0;

		float m_start_at = 0;

		float m_frame_rate = 10.0f;

		size_t m_frame_count = 0;

		bool m_bStopFlag = false;
		bool m_bLoop = true;
		bool m_bEndReached = false;

		//QSlider* m_poSlider = nulltr;
	protected:
		static uint64_t sm_qwFrencuency;
		static uint64_t sm_qwAppStartValue;
	};
};