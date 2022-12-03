#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ctime>
#include <ddraw.h>
#include <string>
#include "audio.h"
#include "gamelib.h"
#include "Whip.h"
namespace game_framework {
	Whip::Whip() {
		level = 1;
		whip_count = 0;
		range = 1.0;
		damage = 10;
		_cd = 50;
		attacking = false;
	}
	void Whip::LoadBitmap() {
		//count = 1
		_animations[0][0].AddBitmap(skill_Lwhip_1_1, RGB(0, 0, 0));
		_animations[0][0].AddBitmap(skill_WHIP_B, RGB(0, 0, 0));
		_animations[0][1].AddBitmap(skill_Rwhip_1_1, RGB(0, 0, 0));
		_animations[0][1].AddBitmap(skill_WHIP_B, RGB(0, 0, 0));
		//count = 2
		_animations[1][0].AddBitmap(skill_Lwhip_2_1, RGB(0, 0, 0));
		_animations[1][0].AddBitmap(skill_Lwhip_2_2, RGB(0, 0, 0));
		_animations[1][0].AddBitmap(skill_WHIP_B, RGB(0, 0, 0));
		_animations[1][1].AddBitmap(skill_Rwhip_2_1, RGB(0, 0, 0));
		_animations[1][1].AddBitmap(skill_Rwhip_2_2, RGB(0, 0, 0));
		_animations[1][1].AddBitmap(skill_WHIP_B, RGB(0, 0, 0));
		//count = 3
		_animations[2][0].AddBitmap(skill_Lwhip_3_1, RGB(0, 0, 0));
		_animations[2][0].AddBitmap(skill_Lwhip_3_2, RGB(0, 0, 0));
		_animations[2][0].AddBitmap(skill_Lwhip_3_3, RGB(0, 0, 0));
		_animations[2][0].AddBitmap(skill_WHIP_B, RGB(0, 0, 0));
		_animations[2][1].AddBitmap(skill_Rwhip_3_1, RGB(0, 0, 0));
		_animations[2][1].AddBitmap(skill_Rwhip_3_2, RGB(0, 0, 0));
		_animations[2][1].AddBitmap(skill_Rwhip_3_3, RGB(0, 0, 0));
		_animations[2][1].AddBitmap(skill_WHIP_B, RGB(0, 0, 0));
		//count = 4
		_animations[3][0].AddBitmap(skill_Lwhip_4_1, RGB(0, 0, 0));
		_animations[3][0].AddBitmap(skill_Lwhip_4_2, RGB(0, 0, 0));
		_animations[3][0].AddBitmap(skill_Lwhip_4_3, RGB(0, 0, 0));
		_animations[3][0].AddBitmap(skill_Lwhip_4_4, RGB(0, 0, 0));
		_animations[3][0].AddBitmap(skill_WHIP_B, RGB(0, 0, 0));
		_animations[3][1].AddBitmap(skill_Rwhip_4_1, RGB(0, 0, 0));
		_animations[3][1].AddBitmap(skill_Rwhip_4_2, RGB(0, 0, 0));
		_animations[3][1].AddBitmap(skill_Rwhip_4_3, RGB(0, 0, 0));
		_animations[3][1].AddBitmap(skill_Lwhip_4_4, RGB(0, 0, 0));
		_animations[3][1].AddBitmap(skill_WHIP_B, RGB(0, 0, 0));
		//count = 5
		_animations[4][0].AddBitmap(skill_Lwhip_4_1, RGB(0, 0, 0));
		_animations[4][0].AddBitmap(skill_Lwhip_4_2, RGB(0, 0, 0));
		_animations[4][0].AddBitmap(skill_Lwhip_4_3, RGB(0, 0, 0));
		_animations[4][0].AddBitmap(skill_Lwhip_4_4, RGB(0, 0, 0));
		_animations[4][0].AddBitmap(skill_Lwhip_5_5, RGB(0, 0, 0));
		_animations[4][0].AddBitmap(skill_WHIP_B, RGB(0, 0, 0));
		_animations[4][1].AddBitmap(skill_Rwhip_4_1, RGB(0, 0, 0));
		_animations[4][1].AddBitmap(skill_Rwhip_4_2, RGB(0, 0, 0));
		_animations[4][1].AddBitmap(skill_Rwhip_4_3, RGB(0, 0, 0));
		_animations[4][1].AddBitmap(skill_Lwhip_4_4, RGB(0, 0, 0));
		_animations[4][1].AddBitmap(skill_Lwhip_5_5, RGB(0, 0, 0));
		_animations[4][1].AddBitmap(skill_WHIP_B, RGB(0, 0, 0));
	}
	void Whip::OnMove(int x, int y, bool watchingLeft) {
		int x0, y0;
		//�U���ŭ�l�y��
		switch (whip_count) {
		case 0:
			watchingLeft ? x0 = x - 150 : x0 = x + 8;
			y0 = y + 16;
			break;
		case 1:
			x0 = x - 150;
			y0 = y - 18;
			break;
		case 2:
			x0 = x - 150;
			y0 = y - 25;
			break;
		case 3:
			x0 = x - 150;
			y0 = y - 40;
			break;
		case 4:
			x0 = x - 150;
			y0 = y - 40;
			break;
		default:
			x0 = x - 150;
			y0 = y - 40;
			break;
		}
		//�M�w�j�p�᪺�y��
		if (watchingLeft) {
			if (whip_count != 0) {
				_x = x0 - (int)(0.5 * _animations[whip_count][1].getFirstBmpWidth() * (range - 1)); //_x = x0 - 0.5 * �ϼe * (factor - 1)
			}
			else {
				_x = x0 - (int)(_animations[whip_count][0].getFirstBmpWidth() * (range - 1)); //_x = x0 - �ϼe * (factor - 1)
			}
		}
		else {
			if (whip_count != 0) {
				_x = x0 - (int)(0.5 * _animations[whip_count][1].getFirstBmpWidth() * (range - 1));
			}
			else {
				_x = x0; //x�y�ЬۦP, �������ץ�
			}
		}
		_y = y0 - (int)(_animations[whip_count][1].getFirstBmpHeight() * 0.5 * (range - 1)); //_y = y0 - 0.5 * �ϰ� * (factor - 1)
		//SetDelay
		for (int i = 0; i < 5; i++) {
			_animations[i][0].SetDelayCount(1);
			_animations[i][1].SetDelayCount(1);
		}
		//OnMove
		for (int i = 0; i < 5; i++) {
			_animations[i][0].OnMove();
			_animations[i][1].OnMove();
		}
	}
	void Whip::OnShow(bool left) {
		_cd -= 1;
		if (_cd == 0) {
			attacking = true;
			_animations[whip_count][0].Reset();
			_animations[whip_count][1].Reset();
		}
		if (left) {
			if (_cd <= 0) {
				_animations[whip_count][0].SetTopLeft(_x, _y);
				_animations[whip_count][0].OnShow(range);
				if (_animations[whip_count][0].IsFinalBitmap()) {
					_cd = 25;
					attacking = false;
				}
			}
		}
		else {
			if (_cd <= 0) {
				_animations[whip_count][1].SetTopLeft(_x, _y);
				_animations[whip_count][1].OnShow(range);
				if (_animations[whip_count][1].IsFinalBitmap()) {
					_cd = 25;
					attacking = false;
				}
			}
		}
	}
	void Whip::setLevel(int level) {
		this->level = level;
	}
	void Whip::setRange(int range) {
		this->range = range;
	}
	void Whip::addDmg(int i)
	{
		damage += i;
	}
	int Whip::getDmg() {
		return damage;
	}
	int Whip::getX() {
		return _x;
	}
	int Whip::getY() {
		return _y;
	}
	bool Whip::isAttacking() {
		return attacking;
	}
	void Whip::set_count(int level) {
		whip_count = level;
	}
}