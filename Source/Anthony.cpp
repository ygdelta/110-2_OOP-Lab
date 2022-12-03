#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Anthony.h"
#include "Bat.h"
#include <string>
namespace game_framework {
	Anthony::Anthony() {
		x = 768;//左上
		y = 484;
		index = 1;
		velocity = 9;
		movingDown = movingUp = movingLeft = movingRight = FALSE;
		blood = 100;//
		hit = 0;//
		t = 1;
		levelup = false;
	}
	void Anthony::initialize() {
		level = 0;
		exp = 0;
		x = 768;//左上
		y = 484;
		index = 1;
		velocity = 9;
		movingDown = movingUp = movingLeft = movingRight = FALSE;
		t = 1;
		blood = 100;//
		hit = 0; //
		levelup = false;
	}
	void Anthony::LoadBitmap() {
		CMovingBitmap image1, image2;
		image1.LoadBitmap(ANTHONY1, RGB(255, 255, 255));
		image2.LoadBitmap(ANTHONY2, RGB(255, 255, 255));
		right.AddBitmap(ANTHONY1, RGB(255, 255, 255));
		right.AddBitmap(ANTHONY3, RGB(255, 255, 255));
		left.AddBitmap(ANTHONY2, RGB(255, 255, 255));
		left.AddBitmap(ANTHONY4, RGB(255, 255, 255));
		deathAniamtion[0].AddBitmap(death_L_1, RGB(0, 0, 0));
		deathAniamtion[0].AddBitmap(death_L_2, RGB(0, 0, 0));
		deathAniamtion[0].AddBitmap(death_L_3, RGB(0, 0, 0));
		deathAniamtion[0].AddBitmap(death_L_4, RGB(0, 0, 0));
		deathAniamtion[1].AddBitmap(death_R_1, RGB(255, 255, 255));
		deathAniamtion[1].AddBitmap(death_R_2, RGB(255, 255, 255));
		deathAniamtion[1].AddBitmap(death_R_3, RGB(255, 255, 255));
		deathAniamtion[1].AddBitmap(death_R_4, RGB(255, 255, 255));
		right.SetDelayCount(7);
		left.SetDelayCount(7);
		deathAniamtion[0].SetDelayCount(7);
		deathAniamtion[1].SetDelayCount(7);
		_images.push_back(image1);
		_images.push_back(image2);
		HP.LoadBitmap(BLOOD); 
		HP_down.LoadBitmap(GRAY_BLOOD);
	}
	bool XNOR(bool a, bool b) {
		return a == b ? true : false;
	}
	void Anthony::OnMove() {
		int STEP_SIZE = velocity;
		if (movingLeft && XNOR(movingUp, movingDown)) {
			if (x - STEP_SIZE >= 0)
				x -= STEP_SIZE;
			index = 1;
		}
		if (movingRight && XNOR(movingUp, movingDown)) {
			if (x + STEP_SIZE <= 1500)
				x += STEP_SIZE;
			index = 0;
		}
		if (movingUp && XNOR(movingRight, movingLeft)) {
			if (y - STEP_SIZE >= 0)
				y -= STEP_SIZE;
		}
		if (movingDown && XNOR(movingRight, movingLeft)) {
			if (y + STEP_SIZE <= 930)
				y += STEP_SIZE;
		}
		if (movingUp && !movingLeft && movingRight) {
			int step = (int)((float)STEP_SIZE / 1.414);
			if (x + step <= 1500)
				x += step;
			if (y - step >= 0)
				y -= step;
			index = 0;
		}
		if (movingUp && !movingRight && movingLeft) {
			int step = (int)((float)STEP_SIZE / 1.414);
			if (x - step >= 0)
				x -= step;
			if (y - step >= 0)
				y -= step;
			index = 1;
		}
		if (movingDown && !movingRight && movingLeft) {
			int step = (int)((float)STEP_SIZE / 1.414);
			if (x - step >= 0)
				x -= step;
			if (y + step <= 930)
				y += step;
			index = 1;
		}
		if (movingDown && !movingLeft && movingRight) {
			int step = (int)((float)STEP_SIZE / 1.414);
			if (x + step <= 1500)
				x += step;
			if (y + step <= 930)
				y += step;
			index = 0;
		}
		///////升級判斷///////
		if (level <= 19) {
			if (exp >= (10 * level) + 5) {
				exp -= (10 * level) + 5;
				level += 1;
				levelup = true;
			}
			else {
				levelup = false;
			}
		}
		else if (level <= 39) {
			if (exp >= 13 * level) {
				exp -= 13 * level;
				level += 1;
				levelup = true;
			}
			else {
				levelup = false;
			}
		}
		else if (level > 39) {
			if (exp >= 16 * level) {
				exp -= 16 * level;
				level += 1;
				levelup = true;
			}
			else {
				levelup = false;
			}
		}
		right.OnMove();
		left.OnMove();
		deathAniamtion[0].OnMove();
		deathAniamtion[1].OnMove();
	}
	void Anthony::OnShow() {
		if (!this->isAlive()) {	// 判斷是否存活		
			if (watchingLeft) {						// 死亡時播放deathAnimation
				movingUp = movingDown = movingLeft = movingRight = false;
				deathAniamtion[0].SetTopLeft(x, y);
				deathAniamtion[0].OnShow();
			}
			else {
				movingUp = movingDown = movingLeft = movingRight = false;
				deathAniamtion[1].SetTopLeft(x, y);
				deathAniamtion[1].OnShow();
			}
		}
		else {
			if (index == 0) {						// 存活時播放的內容
				if (movingRight || movingDown || movingUp || (movingRight && movingUp) || (movingRight && movingDown)) {
					right.SetTopLeft(x, y);
					right.OnShow();
					watchingLeft = FALSE;
				}
				else {
					_images[0].SetTopLeft(x, y);
					_images[0].ShowBitmap();
				}
			}
			else {
				if (movingLeft || movingDown || movingUp || (movingLeft && movingUp) || (movingLeft && movingDown)) {
					left.SetTopLeft(x, y);
					left.OnShow();
					watchingLeft = TRUE;
				}
				else {
					_images[1].SetTopLeft(x, y);
					_images[1].ShowBitmap();
				}
			}
		}
		//HP
		for (int i = 0; i < 50; i++) {
			HP.SetTopLeft(x - 69 + i * 4, y + 75);
			HP.ShowBitmap(0.2);
		}

		if (hit < 100) {
			if (hit % 2 == 0) {
				for (int i = 0; i < hit / 2; i++) {
					HP_down.SetTopLeft(x + 127 - (i * 4), y + 75);
					HP_down.ShowBitmap(0.2);
				}
			}
			else {
				for (int i = 0; i < hit / 2; i++) {
					HP_down.SetTopLeft(x + 127 - (i * 4), y + 75);
					HP_down.ShowBitmap(0.2);
				}
			}

		}
		else {
			for (int i = 0; i < hit / 2; i++) {
				HP_down.SetTopLeft(x + 127 - (i * 4), y + 75);
				HP_down.ShowBitmap(0.2);
			}
		}
	}
	int Anthony::getX() {
		return x;
	}
	int Anthony::getY() {
		return y;
	}
	void Anthony::setUp(bool set) {
		movingUp = set;
	}
	void Anthony::setDown(bool set) {
		movingDown = set;
	}
	void Anthony::setLeft(bool set) {
		movingLeft = set;
	}
	void Anthony::setRight(bool set) {
		movingRight = set;
	}
	void Anthony::addExp(int item) {
		switch (item) {
		case 0:			//第一種寶石
			exp += 1;
			break;
		case 1:
			break;
		case 2:
			break;
		}
	}
	bool Anthony::movingUpState() {
		return movingUp;
	}
	bool Anthony::movingDownState() {
		return movingDown;
	}
	bool Anthony::movingLeftState() {
		return movingLeft;
	}
	bool Anthony::movingRightState() {
		return movingRight;
	}
	bool Anthony::isWatchingLeft() {
		return watchingLeft;
	}
	void Anthony::hitMonster(Bat bat) {
		//50*40  60,66
		int bat_posi[4][2] = { {bat.getX(),bat.getY()},{bat.getX() + 50,bat.getY()},{bat.getX(),bat.getY() + 40},{bat.getX() + 50,bat.getY() + 40} };
		bool judge[4] = {};
		for (int i = 0; i < 4; i++) {
			judge[i] = bat_posi[i][0] >= x && bat_posi[i][0] <= (x + 40) && bat_posi[i][1] <= (y + 40) && bat_posi[i][1] >= y;
		}
		if ((judge[0] || judge[1] || judge[2] || judge[3]) && bat.isAlive() == true) {
			hit++;
			blood -= 1;
		}
	}
	int Anthony::getExp() {
		return exp;
	}
	int Anthony::getLevel() {
		return level;
	}
	bool Anthony::isAlive() {
		if (blood <= 0) {
			return 0;
		}
		else {
			return 1;
		}
	}
	bool Anthony::levelUP() {
		return levelup;
	}
	void Anthony::heal() {
		if (this->levelUP() && hit <= 15) {
			hit = 0;
			blood = 100;
		}
		else if (this->levelUP() && hit > 15) {
			hit -= 15;
			blood += 15;
		}
	}
	bool Anthony::deathAnimationDone()
	{
		if (!this->isAlive()) {
			if (deathAniamtion[0].IsFinalBitmap() || deathAniamtion[1].IsFinalBitmap()) {
				return true;
			}
		}
		return false;
	}
}