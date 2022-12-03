#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ctime>
#include <ddraw.h>
#include <string>
#include "audio.h"
#include "gamelib.h"
#include "Timer.h"
namespace game_framework {
	Timer::Timer() {
		_time = 0;
	}
	void Timer::LoadBitmap() {
		int _bits[10] = { timer_0, timer_1, timer_2, timer_3, timer_4, timer_5, timer_6, timer_7, timer_8, timer_9 };
		for (int i = 0; i < 10; i++) {
			if (i < 6) min1.AddBitmap(_bits[i], RGB(0, 0, 0));
			min2.AddBitmap(_bits[i], RGB(0, 0, 0));
			if (i < 6) sec1.AddBitmap(_bits[i], RGB(0, 0, 0));
			sec2.AddBitmap(_bits[i], RGB(0, 0, 0));
		}
		sec1.SetDelayCount(300);
		sec2.SetDelayCount(30);
		min1.SetDelayCount(30 * 600);
		min2.SetDelayCount(30 * 60);
		mid.LoadBitmap(timer_dot, RGB(0, 0, 0));
	}
	void Timer::OnMove() {
		min1.SetTopLeft(740, 50);
		min2.SetTopLeft(773, 50);
		sec1.SetTopLeft(829, 50);
		sec2.SetTopLeft(862, 50);

		min1.OnMove();
		min2.OnMove();
		sec1.OnMove();
		sec2.OnMove();

		mid.SetTopLeft(797, 50);
	}
	void Timer::addOneSec() {
		_time += 1;
	}
	void Timer::initialTime() {
		_time = 0;
		min1.Reset();
		min2.Reset();
		sec1.Reset();
		sec2.Reset();
	}
	void Timer::OnShow() {
		min1.OnShow();
		min2.OnShow();
		mid.ShowBitmap();
		sec1.OnShow();
		sec2.OnShow();
	}
	int Timer::getTime() {
		return int(_time / 30);
	}
}