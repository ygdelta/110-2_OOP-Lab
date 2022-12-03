#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <cmath>
#include "audio.h"
#include "gamelib.h"
#include <string>
#include "ExpBar.h"
namespace game_framework {
	ExpBar::ExpBar() {
		persentage = 0;
	}
	void ExpBar::LoadBitmap() {
		_bg.LoadBitmap(expBar_bg);
		_exp.LoadBitmap(expBar_exp);
	}
	void ExpBar::OnMove(int exp, int level) {
		//升級參考資料:https://vampire-survivors.fandom.com/wiki/Level_up
		int totalNeed = 0;
		if (level <= 19) {
			totalNeed = (10 * level) + 5;
		}
		else if (level <= 39) {
			totalNeed = 13 * (level - 19) + 2000;
		}
		else if (level > 39) {
			totalNeed = 16 * (level - 39) + 45900;
		}
		persentage = (int)(((float)exp / totalNeed) * 100);
	}
	void ExpBar::OnShow() {
		for (int i = 0; i < 100; i++) {
			_bg.SetTopLeft(16 * i, 0);
			_bg.ShowBitmap();
		}
		for (int i = 0; i < persentage; i++) {
			_exp.SetTopLeft(16 * i, 0);
			_exp.ShowBitmap();
		}
	}
}