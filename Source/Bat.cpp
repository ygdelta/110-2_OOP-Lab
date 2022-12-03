#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <cmath>
#include "audio.h"
#include "gamelib.h"
#include "Bat.h"
#include <string>
namespace game_framework {
	Bat::Bat() {
		_x = 500;
		_y = 600;
		blood = 10;
		movingDown = movingLeft = movingUp = false;
		movingRight = true;
		hit_times = 0;
	}
	Bat::Bat(int x, int y, int _blood) {
		_x = x;
		_y = y;
		blood = _blood;
		movingDown = movingLeft = movingUp = false;
		movingRight = true;
		hit_times = 0;
	}
	Bat::~Bat() {

	}
	void Bat::initialize(int x, int y, int _blood) {
		_x = x;
		_y = y;
		blood = _blood;
		movingDown = movingLeft = movingUp = false;
		movingRight = true;
		hit_times = 0;
	}
	void Bat::LoadBitmap() {
		_images[0].LoadBitmap(E_Bat1_1, RGB(255, 255, 255));
		_images[1].LoadBitmap(E_Bat1_2, RGB(255, 255, 255));
		_images[2].LoadBitmap(EXP, RGB(255, 255, 255));
	}
	bool Bat::dieInThisHit(int dmg) {
		if (blood - dmg <= 0) {			//判斷是否會死亡
			return true;
		}
		return false;
	}

	void Bat::OnMove(int x, int y) {
		const int step = 4;
		double dx = x - _x;
		double dy = y - _y;
		double sin;
		double cos;
		if (this->isAlive()) {
			if (!(dx == 0 && dy == 0)) {
				slope = dy / dx;
				sin = abs(dy / sqrt(pow(dx, 2) + pow(dy, 2)));
				cos = abs(dx / sqrt(pow(dx, 2) + pow(dy, 2)));
			}
			if (dx < 0) {
				movingLeft = true;
				movingRight = false;
				_x -= int(step * cos);
			}
			else if (dx > 0) {
				movingRight = true;
				movingLeft = false;
				_x += int(step * cos);
			}
			else {
				_x = x;
				movingRight = false;
				movingLeft = false;
			}
			if (dy < 0) {
				movingUp = true;
				movingDown = false;
				_y -= int(step * sin);
			}
			else if (dy > 0) {
				movingDown = true;
				movingUp = false;
				_y += int(step * sin);
			}
			else {
				_y = y;
				movingUp = false;
				movingDown = false;
			}
		}
	}
	void Bat::OnShow() {
		if (this->isAlive()) {
			if (movingLeft) {
				_images[0].SetTopLeft(_x, _y);
				_images[0].ShowBitmap();
			}
			else {
				_images[1].SetTopLeft(_x, _y);
				_images[1].ShowBitmap();
			}
		}
		else {
			_images[2].SetTopLeft(_x, _y);
			_images[2].ShowBitmap(2);
		}
	}
	void Bat::setMovingDown(bool state) {
		movingDown = state;
	}
	void Bat::setMovingUp(bool state) {
		movingUp = state;
	}
	void Bat::setMovingLeft(bool state) {
		movingLeft = state;
	}
	void Bat::setMovingRight(bool state) {
		movingRight = state;
	}
	int Bat::getX() {
		return _x;
	}
	int Bat::getY() {
		return _y;
	}
	void Bat::attack() {

	}
	void Bat::beAttack(int x, int y, int damage, int count, bool attacking, int &cDmg) {
		int batRange[4][2] = { {_x,_y},{_x + 50,_y},{_x,_y + 40},{_x + 50,_y + 40} };
		bool judge[4] = {};
		switch (count) {
		case 0: //attack1 = 200*34
			for (int i = 0; i < 4; i++) {
				judge[i] = batRange[i][0] >= x && batRange[i][0] <= (x + 200) && batRange[i][1] <= (y + 40) && batRange[i][1] >= y;
			}
			if (attacking) {	//攻擊成功
				if (judge[0] || judge[1] || judge[2] || judge[3]) {
					blood -= damage;
					cDmg += damage;
					hit_times++;
				}
			}
			break;
		case 1: //200*68
			for (int i = 0; i < 4; i++) {
				judge[i] = batRange[i][0] >= x && batRange[i][0] <= (x + 400) && batRange[i][1] <= (y + 60) && batRange[i][1] >= y;
			}
			if (attacking) {	//攻擊成功
				if (judge[0] || judge[1] || judge[2] || judge[3]) {
					blood -= damage;
					cDmg += damage;
					hit_times++;
				}
			}
			break;
		case 2:
			for (int i = 0; i < 4; i++) {
				judge[i] = batRange[i][0] >= x && batRange[i][0] <= (x + 400) && batRange[i][1] <= (y + 75) && batRange[i][1] >= y;
			}
			if (attacking) {	//攻擊成功
				if (judge[0] || judge[1] || judge[2] || judge[3]) {
					blood -= damage;
					cDmg += damage;
					hit_times++;
				}
			}
			break;
		case 3:
			for (int i = 0; i < 4; i++) {
				judge[i] = batRange[i][0] >= x && batRange[i][0] <= (x + 400) && batRange[i][1] <= (y + 80) && batRange[i][1] >= y;
			}
			if (attacking) {	//攻擊成功
				if (judge[0] || judge[1] || judge[2] || judge[3]) {
					blood -= damage;
					cDmg += damage;
					hit_times++;
				}
			}
			break;
		case 4:
			for (int i = 0; i < 4; i++) {
				judge[i] = batRange[i][0] >= x && batRange[i][0] <= (x + 400) && batRange[i][1] <= (y + 90) && batRange[i][1] >= y;
			}
			if (attacking) {	//攻擊成功
				if (judge[0] || judge[1] || judge[2] || judge[3]) {
					blood -= damage;
					cDmg += damage;
					hit_times++;
				}
			}
			break;
		default:
			for (int i = 0; i < 4; i++) {
				judge[i] = batRange[i][0] >= x && batRange[i][0] <= (x + 400) && batRange[i][1] <= (y + 90) && batRange[i][1] >= y;
			}
			if (attacking) {	//攻擊成功
				if (judge[0] || judge[1] || judge[2] || judge[3]) {
					blood -= damage;
					cDmg += damage;
					hit_times++;
				}
			}
			break;
		}
	}
	bool Bat::beHit(int x, int y, int damage, int count, bool attacking) {
		int batRange[4][2] = { {_x,_y},{_x + 50,_y},{_x,_y + 40},{_x + 50,_y + 40} };
		bool judge[4] = {};
		switch (count) {
		case 0: //attack1 = 200*34
			for (int i = 0; i < 4; i++) {
				judge[i] = batRange[i][0] >= x && batRange[i][0] <= (x + 200) && batRange[i][1] <= (y + 40) && batRange[i][1] >= y;
			}
			if (attacking) {
				return judge[0] || judge[1] || judge[2] || judge[3];
				break;
			}
			else {
				return 0;
				break;
			}
		case 1: //200*68
			for (int i = 0; i < 4; i++) {
				judge[i] = batRange[i][0] >= x && batRange[i][0] <= (x + 400) && batRange[i][1] <= (y + 60) && batRange[i][1] >= y;
			}
			if (attacking) {
				return judge[0] || judge[1] || judge[2] || judge[3];
				break;
			}
			else {
				return 0;
				break;
			}
		case 2:
			for (int i = 0; i < 4; i++) {
				judge[i] = batRange[i][0] >= x && batRange[i][0] <= (x + 400) && batRange[i][1] <= (y + 75) && batRange[i][1] >= y;
			}
			if (attacking) {
				return judge[0] || judge[1] || judge[2] || judge[3];
				break;
			}
			else {
				return 0;
				break;
			}
		case 3:
			for (int i = 0; i < 4; i++) {
				judge[i] = batRange[i][0] >= x && batRange[i][0] <= (x + 400) && batRange[i][1] <= (y + 80) && batRange[i][1] >= y;
			}
			if (attacking) {
				return judge[0] || judge[1] || judge[2] || judge[3];
				break;
			}
			else {
				return 0;
				break;
			}
		case 4:
			for (int i = 0; i < 4; i++) {
				judge[i] = batRange[i][0] >= x && batRange[i][0] <= (x + 400) && batRange[i][1] <= (y + 90) && batRange[i][1] >= y;
			}
			if (attacking && (judge[0] || judge[1] || judge[2] || judge[3])) {
				return 1;
				break;
			}
			else {
				return 0;
				break;
			}
		default:
			for (int i = 0; i < 4; i++) {
				judge[i] = batRange[i][0] >= x && batRange[i][0] <= (x + 400) && batRange[i][1] <= (y + 90) && batRange[i][1] >= y;
			}
			if (attacking && (judge[0] || judge[1] || judge[2] || judge[3])) {
				return 1;
				break;
			}
			else {
				return 0;
				break;
			}
		}
	}
	bool Bat::isAlive() {
		return blood > 0;
	}
	void Bat::setBlood(int hp) {
		blood = hp;
	}
	void Bat::setX(int x) {
		_x = x;
	}
	void Bat::setY(int y) {
		_y = y;
	}
}