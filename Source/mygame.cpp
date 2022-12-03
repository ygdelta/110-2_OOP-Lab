/*
 * mygame.cpp: ���ɮ��x�C��������class��implementation
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"
#include "Whip.h"
#include <iostream>
#include<string>
#include<random>
#include<ctime>
namespace game_framework {
	static int cumulativeDmg;			// �֭p�ˮ`
	static int totalKills;				// �֭p����
	static int surviveTime;				// �s���ɶ�
	static int p_level;					// �F�쵥��
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C���}�Y�e������
	/////////////////////////////////////////////////////////////////////////////
	CGameStateInit::CGameStateInit(CGame *g)
		: CGameState(g){}
	void CGameStateInit::OnInit()
	{
		// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
		//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
		//
		ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
		//
		// �}�l���J���
		//
		Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
		//
		// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
		//
		CAudio::Instance()->Load(AUDIO_GAME_MUSIC, "sounds\\game_music.mp3");
		CAudio::Instance()->Load(AUDIO_HIT_MONSTER, "sounds\\hit_monster.mp3");
		CAudio::Instance()->Load(AUDIO_EXP, "sounds\\exp.mp3");
		CAudio::Instance()->Load(AUDIO_LEVEL_UP, "sounds\\levelup.mp3");
		CAudio::Instance()->Load(AUDIO_GAMEOVER, "sounds\\game_over.mp3");
		bg.LoadBitmap(BACKGROUND);
		start.LoadBitmap(btn_start, RGB(0, 0, 0));
		aboutUs.LoadBitmap(btn_abotUs, RGB(0, 0, 0));
		howToPlay.LoadBitmap(btn_howToPlay, RGB(0, 0, 0));
		aboutView.LoadBitmap(AboutView, RGB(0, 0, 0));
		HowToPlayView.LoadBitmap(HowView, RGB(0, 0, 0));
		for (int i = 0; i < 3; i++) {
			isClicked[i] = false;
		}

	}
	void CGameStateInit::OnBeginState(){}
	void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		const char KEY_SPACE = ' ';
		if (nChar == KEY_SPACE)
			GotoGameState(GAME_STATE_RUN);						// ������GAME_STATE_RUN
		else if (nChar == KEY_ESC)								// Demo �����C������k
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// �����C��
	}
	void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
	{
		if (!isClicked[0] && !isClicked[1] && !isClicked[2]) { //���S�����s�Q�I��
			//�]�w���s����m
			CPoint btnStart[2] = { CPoint(481, 570), CPoint(481 + start.Width(), 570 + start.Height()) };
			CPoint btnHowToPlay[2] = { CPoint(850, 570), CPoint(850 + howToPlay.Width(), 570 + howToPlay.Height()) };
			CPoint btnAboutUs[2] = { CPoint(712, 725), CPoint(712 + aboutUs.Width(), 725 + aboutUs.Height()) };
			CRect start = CRect(btnStart[0], btnStart[1]);
			CRect how = CRect(btnHowToPlay[0], btnHowToPlay[1]);
			CRect about = CRect(btnAboutUs[0], btnAboutUs[1]);
			if (start.PtInRect(point)) {					// �P�_�I������m�O�_�b�d��
				isClicked[0] = false;
				isClicked[1] = false;
				isClicked[2] = false;
				GotoGameState(GAME_STATE_RUN);				// ������GAME_STATE_RUN
			}
			if (how.PtInRect(point)) {
				isClicked[0] = false;
				isClicked[1] = true;
				isClicked[2] = false;
			}
			if (about.PtInRect(point)) {
				isClicked[0] = false;
				isClicked[1] = false;
				isClicked[2] = true;
			}
		}
		else if (isClicked[1] && !isClicked[2]) {			//���Fhow to play
			CPoint judgePt[2] = { CPoint(439, 200), CPoint(439 + HowToPlayView.Width(), 200 + HowToPlayView.Height()) };
			CRect judge = CRect(judgePt[0], judgePt[1]);
			if (!judge.PtInRect(point)) {					//�I����ܮإ~�N�]�wClicked��false
				isClicked[0] = false;
				isClicked[1] = false;
				isClicked[2] = false;
			}
		}
		else if (!isClicked[1] && isClicked[2]) {			//���Fabout us
			CPoint judgePt[2] = { CPoint(439, 200), CPoint(439 + aboutView.Width(), 200 + aboutView.Height()) };
			CRect judge = CRect(judgePt[0], judgePt[1]);
			if (!judge.PtInRect(point)) {					//�I����ܮإ~�N�]�wClicked��false
				isClicked[0] = false;
				isClicked[1] = false;
				isClicked[2] = false;
			}
		}
	}
	void CGameStateInit::OnShow()
	{
		bg.SetTopLeft(32, 0);
		start.SetTopLeft(481, 570);
		howToPlay.SetTopLeft(850, 570);
		aboutUs.SetTopLeft(712, 725);
		aboutView.SetTopLeft(439, 200);
		HowToPlayView.SetTopLeft(439, 200);
		bg.ShowBitmap();
		start.ShowBitmap();
		howToPlay.ShowBitmap();
		aboutUs.ShowBitmap();
		if (isClicked[1]) { // �w���Fhow to play
			HowToPlayView.ShowBitmap();
		}
		if (isClicked[2]) { // �w���Fabout us
			aboutView.ShowBitmap();
		}
	}
	CGameStateSelect::CGameStateSelect(CGame *g)
		: CGameState(g)
	{
	}
	void CGameStateSelect::OnMove()
	{
	}
	void CGameStateSelect::OnBeginState()
	{
	}
	void CGameStateSelect::OnLButtonDown(UINT nFlags, CPoint point)
	{
		GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
	}
	void CGameStateSelect::OnInit()
	{
		//
		// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
		//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
		//
		ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
		//
		// �}�l���J���
		//
		Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
		//
		// �̲׶i�׬�100%
		//
		ShowInitProgress(100);
		background.LoadBitmap(SELECT);
	}
	void CGameStateSelect::OnShow()
	{
		background.ShowBitmap();
	}
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����������A(Game Over)
	/////////////////////////////////////////////////////////////////////////////
	CGameStateOver::CGameStateOver(CGame *g)
		: CGameState(g)
	{
	}
	void CGameStateOver::OnMove()
	{
	}
	void CGameStateOver::OnLButtonDown(UINT nFlags, CPoint point) {
		CPoint btnDone[2] = { CPoint(666, 750), CPoint(666 + doneBtn.Width(), 750 + doneBtn.Height()) };
		CRect judge = CRect(btnDone[0], btnDone[1]);
		if (judge.PtInRect(point)) {
			GotoGameState(GAME_STATE_INIT);
		}
	}
	void CGameStateOver::OnBeginState()
	{
	}
	void CGameStateOver::OnInit()
	{
		//
		// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
		//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
		//
		ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
		//
		// �}�l���J���
		//
		Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
		//
		// �̲׶i�׬�100%
		//
		ShowInitProgress(100);
		over_background.LoadBitmap(overBG);
		doneBtn.LoadBitmap(btn_done, RGB(0, 0, 0));
		resultView.LoadBitmap(img_result, RGB(0, 0, 0));
	}
	void CGameStateOver::OnShow()
	{
		std::string cDmg, sTime, tKill, L;
		cDmg = std::to_string(cumulativeDmg);
		//�B�z�ɶ����/////////////////////////
		int sec = surviveTime % 60;
		int min = surviveTime / 60;
		std::string str_sec = to_string(sec);
		std::string str_min = to_string(min);
		sTime = str_min + ":" + str_sec;
		///////////////////////////////////////
		tKill = std::to_string(totalKills);
		L = std::to_string(p_level + 1);
		over_background.SetTopLeft(32, 0);
		resultView.SetTopLeft(439, 200);
		doneBtn.SetTopLeft(666, 750);
		over_background.ShowBitmap();
		resultView.ShowBitmap();
		doneBtn.ShowBitmap();
		//��֭ܲp�ˮ`
		const CString dmg(cDmg.c_str());
		CDC* pDC1 = CDDraw::GetBackCDC();
		CFont f1, *fp1;
		f1.CreatePointFont(350, "Arial");
		fp1 = pDC1->SelectObject(&f1);
		pDC1->SetBkMode(TRANSPARENT);
		pDC1->SetTextColor(RGB(255, 255, 255));
		pDC1->TextOut(815, 525, dmg);
		pDC1->SelectObject(fp1);
		CDDraw::ReleaseBackCDC();
		//��ܥͦs�ɶ�
		const CString svt(sTime.c_str());
		CDC *pDC2 = CDDraw::GetBackCDC();
		CFont f2, *fp2;
		f2.CreatePointFont(350, "Arial");
		fp2 = pDC2->SelectObject(&f2);
		pDC2->SetBkMode(TRANSPARENT);
		pDC2->SetTextColor(RGB(255, 255, 255));
		pDC2->TextOut(815, 350, svt);
		pDC2->SelectObject(fp2);
		CDDraw::ReleaseBackCDC();
		//��ܹF�쪺����
		const CString lv(L.c_str());
		CDC *pDC3 = CDDraw::GetBackCDC();
		CFont f3, *fp3;
		f3.CreatePointFont(350, "Arial");
		fp3 = pDC3->SelectObject(&f3);
		pDC3->SetBkMode(TRANSPARENT);
		pDC3->SetTextColor(RGB(255, 255, 255));
		pDC3->TextOut(815, 408, lv);
		pDC3->SelectObject(fp3);
		CDDraw::ReleaseBackCDC();
		//��֭ܲp����
		const CString k(tKill.c_str());
		CDC *pDC4 = CDDraw::GetBackCDC();
		CFont f4, *fp4;
		f4.CreatePointFont(330, "Arial");
		fp4 = pDC4->SelectObject(&f4);
		pDC4->SetBkMode(TRANSPARENT);
		pDC4->SetTextColor(RGB(255, 255, 255));
		pDC4->TextOut(815, 470, k);
		pDC4->SelectObject(fp4);
		CDDraw::ReleaseBackCDC();
	}
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
	/////////////////////////////////////////////////////////////////////////////
	CGameStateRun::CGameStateRun(CGame *g)
		: CGameState(g)
	{
	}
	CGameStateRun::~CGameStateRun()
	{

	}
	void CGameStateRun::OnBeginState()
	{
		int time = 0;			// �ɶ���l��(���G��)
		pause = false;			// ��l�Ȱ����A�]�w(�w�]��false)
		end_count = 0;
		//////�H�U�ܼƥΥHCGameStateOver���o���//////
		cumulativeDmg = 0;		// �֭p�ˮ`��l��
		totalKills = 0;			// �֭p������l��
		surviveTime = 0;		// �ͦs�ɶ���l��
		p_level = 0;			// ��F���Ū�l��	
		//////////////////////////////////////////////
		timer.initialTime();
		_anthony.initialize();
		//�Ĥ@�i���� initialize
		for (int i = 0; i < sizeof(_bat1) / sizeof(_bat1[0]); i++) {
			int x = rand() % 1600;
			int y = rand() % 2;
			if (y == 1) {
				_bat1[i].initialize(x, 0, 10);

			}
			else {
				_bat1[i].initialize(x, 900, 10);

			}

		}
		//�ĤG�i���� initialize
		for (int i = 0; i < sizeof(_bat2) / sizeof(_bat2[0]); i++) {
			int x = rand() % 1600;
			int y = rand() % 2;
			if (y == 1) {
				_bat2[i].initialize(x, 0, 20);

			}
			else {
				_bat2[i].initialize(x, 900, 20);

			}
		}
		//�ĤT�i���� initialize
		for (int i = 0; i < sizeof(_bat3) / sizeof(_bat3[0]); i++) {
			int x = rand() % 1600;
			int y = rand() % 2;
			if (y == 1) {
				_bat3[i].initialize(x, 0, 30);

			}
			else {
				_bat3[i].initialize(x, 900, 30);
				_bat3[i].setBlood(30);
			}
		}
		//�ĥ|�i���� initialize
		for (int i = 0; i < sizeof(_bat4) / sizeof(_bat4[0]); i++) {
			int x = rand() % 1600;
			int y = rand() % 2;
			if (y == 1) {
				_bat4[i].initialize(x, 0, 40);
			}
			else {
				_bat4[i].initialize(x, 900, 40);

			}
		}
		CAudio::Instance()->Play(AUDIO_GAME_MUSIC, true);	// ����.WAVE
	}
	void CGameStateRun::OnMove()							// ���ʹC������
	{
		timer.addOneSec();
		time = timer.getTime();
		//Pause
		if (pause) {
			// Do nothing.
		}
		else {
			_anthony.OnMove();
			_whip.OnMove(_anthony.getX(), _anthony.getY(), _anthony.isWatchingLeft());
			timer.OnMove();
			expBar.OnMove(_anthony.getExp(), _anthony.getLevel());

			if (time == 600) {	//�C������(Stage clear)
				pause = true;
				CAudio::Instance()->Stop(AUDIO_GAME_MUSIC);
				CAudio::Instance()->Play(AUDIO_GAMEOVER, false);
				surviveTime = time;
				p_level = _anthony.getLevel();
			}
			if (time > 0 && time <= 120) { //�Ĥ@�i
				for (int i = 0; i < sizeof(_bat1) / sizeof(_bat1[0]); i++) {
					_bat1[i].OnMove(_anthony.getX(), _anthony.getY());
				}
				for (int i = 0; i < sizeof(_bat1) / sizeof(_bat1[0]); i++) {
					_anthony.hitMonster(_bat1[i]);
				}
				for (int i = 0; i < sizeof(_bat1) / sizeof(_bat1[0]); i++) {
					if (_bat1[i].beHit(_whip.getX(), _whip.getY(), _whip.getDmg(), _anthony.getLevel(), _whip.isAttacking()) && _bat1[i].isAlive()) {
						CAudio::Instance()->Play(AUDIO_HIT_MONSTER, false);
						if (_bat1[i].dieInThisHit(_whip.getDmg())) {
							totalKills += 1;			// �`���� + 1
						}
					}
					_bat1[i].beAttack(_whip.getX(), _whip.getY(), _whip.getDmg(), _anthony.getLevel(), _whip.isAttacking(), cumulativeDmg);
					if (!_bat1[i].isAlive()) {		// �P�_�������`
						// �������`�ɮy�СA�����ܬ��g��y
						int x0 = _bat1[i].getX();
						int y0 = _bat1[i].getY();
						if (x0 + 9 >= _anthony.getX() && x0 <= _anthony.getX() + 58) { // �P�_�O�_���o�g��
							if (y0 + 12 >= _anthony.getY() && y0 <= _anthony.getY() + 66) {
								_anthony.addExp(0);
								CAudio::Instance()->Play(AUDIO_EXP, false);
								int x = rand() % 1600;
								int y = rand() % 2;
								if (y == 1) {
									_bat1[i].initialize(x, 0, 10);
								}
								else {
									_bat1[i].initialize(x, 900, 10);
								}
							}
						}
					}
				}
			}
			else if (time > 120 && time <= 300) { // �ĤG�i
				for (int i = 0; i < sizeof(_bat2) / sizeof(_bat2[0]); i++) {
					_bat2[i].OnMove(_anthony.getX(), _anthony.getY());
				}
				for (int i = 0; i < sizeof(_bat2) / sizeof(_bat2[0]); i++) {
					_anthony.hitMonster(_bat2[i]);
				}
				for (int i = 0; i < sizeof(_bat2) / sizeof(_bat2[0]); i++) {
					if (_bat2[i].beHit(_whip.getX(), _whip.getY(), _whip.getDmg(), _anthony.getLevel(), _whip.isAttacking()) && _bat2[i].isAlive()) {
						CAudio::Instance()->Play(AUDIO_HIT_MONSTER, false);
						if (_bat2[i].dieInThisHit(_whip.getDmg())) {
							totalKills += 1;			// �`���� + 1
						}
					}
					_bat2[i].beAttack(_whip.getX(), _whip.getY(), _whip.getDmg(), _anthony.getLevel(), _whip.isAttacking(), cumulativeDmg);
					if (!_bat2[i].isAlive()) {
						int x0 = _bat2[i].getX();
						int y0 = _bat2[i].getY();
						if (x0 + 9 >= _anthony.getX() && x0 <= _anthony.getX() + 58) {
							if (y0 + 12 >= _anthony.getY() && y0 <= _anthony.getY() + 66) {
								_anthony.addExp(0);
								CAudio::Instance()->Play(AUDIO_EXP, false);
								int x = rand() % 1600;
								int y = rand() % 2;
								if (y == 1) {
									_bat2[i].initialize(x, 0, 20);
								}
								else {
									_bat2[i].initialize(x, 900, 20);
								}
							}
						}
					}

				}
			}
			else if (time > 300 && time <= 450) { // �ĤT�i
				for (int i = 0; i < sizeof(_bat3) / sizeof(_bat3[0]); i++) {
					_bat3[i].OnMove(_anthony.getX(), _anthony.getY());
				}
				for (int i = 0; i < sizeof(_bat3) / sizeof(_bat3[0]); i++) {
					_anthony.hitMonster(_bat3[i]);
				}
				for (int i = 0; i < sizeof(_bat3) / sizeof(_bat3[0]); i++) {
					if (_bat3[i].beHit(_whip.getX(), _whip.getY(), _whip.getDmg(), _anthony.getLevel(), _whip.isAttacking()) && _bat3[i].isAlive()) {
						CAudio::Instance()->Play(AUDIO_HIT_MONSTER, false);
						if (_bat3[i].dieInThisHit(_whip.getDmg())) {
							totalKills += 1;			// �`���� + 1
						}
					}
					_bat3[i].beAttack(_whip.getX(), _whip.getY(), _whip.getDmg(), _anthony.getLevel(), _whip.isAttacking(), cumulativeDmg);
					if (!_bat3[i].isAlive()) {
						int x0 = _bat3[i].getX();
						int y0 = _bat3[i].getY();
						if (x0 + 9 >= _anthony.getX() && x0 <= _anthony.getX() + 58) {
							if (y0 + 12 >= _anthony.getY() && y0 <= _anthony.getY() + 66) {
								_anthony.addExp(0);
								CAudio::Instance()->Play(AUDIO_EXP, false);
								int x = rand() % 1600;
								int y = rand() % 2;
								if (y == 1) {
									_bat3[i].initialize(x, 0, 30);
								}
								else {
									_bat3[i].initialize(x, 900, 30);
								}
							}
						}
					}
				}
			}
			else if (time > 450 && time <= 600) { // �ĥ|�i
				for (int i = 0; i < sizeof(_bat4) / sizeof(_bat4[0]); i++) {
					_bat4[i].OnMove(_anthony.getX(), _anthony.getY());
				}
				for (int i = 0; i < sizeof(_bat4) / sizeof(_bat4[0]); i++) {
					_anthony.hitMonster(_bat4[i]);
				}
				for (int i = 0; i < sizeof(_bat4) / sizeof(_bat4[0]); i++) {
					if (_bat4[i].beHit(_whip.getX(), _whip.getY(), _whip.getDmg(), _anthony.getLevel(), _whip.isAttacking()) && _bat4[i].isAlive()) {
						CAudio::Instance()->Play(AUDIO_HIT_MONSTER, false);
						if (_bat4[i].dieInThisHit(_whip.getDmg())) {
							totalKills += 1;			// �`���� + 1
						}
					}
					_bat4[i].beAttack(_whip.getX(), _whip.getY(), _whip.getDmg(), _anthony.getLevel(), _whip.isAttacking(), cumulativeDmg);
					if (!_bat4[i].isAlive()) {
						int x0 = _bat4[i].getX();
						int y0 = _bat4[i].getY();
						if (x0 + 9 >= _anthony.getX() && x0 <= _anthony.getX() + 58) {
							if (y0 + 12 >= _anthony.getY() && y0 <= _anthony.getY() + 66) {
								_anthony.addExp(0);
								CAudio::Instance()->Play(AUDIO_EXP, false);
								int x = rand() % 1600;
								int y = rand() % 2;
								if (y == 1) {
									_bat4[i].initialize(x, 0, 40);
								}
								else {
									_bat4[i].initialize(x, 900, 40);
								}
							}
						}
					}
				}
			}
			_anthony.heal();
			if (_anthony.isAlive() == 0) {		// �C������(GameOver)
				if (_anthony.deathAnimationDone()) {
					pause = true;
				}
				CAudio::Instance()->Stop(AUDIO_GAME_MUSIC);
				CAudio::Instance()->Play(AUDIO_GAMEOVER, false);
				surviveTime = time;
				p_level = _anthony.getLevel();
			}
			if (_anthony.getLevel() < 5) {
				_whip.set_count(_anthony.getLevel());
			}
			if (_anthony.levelUP()) {	//�H�ɵ��W�[�����O�H�μ��񭵼�
				_whip.addDmg(2);
				CAudio::Instance()->Play(AUDIO_LEVEL_UP, false);
			}
		}
	}
	void CGameStateRun::OnInit()  										// �C������Ȥιϧγ]�w
	{
		ShowInitProgress(33);
		// CMovingBitmap LoadBitmap
		background.LoadBitmap(IDB_GameMAP);								// ���J�I�����ϧ�	
		deathBG.LoadBitmap(deathBackGroung);
		btn_end.LoadBitmap(endBtn, RGB(0, 0, 0));
		TitleGameOver.LoadBitmap(gameoverTitle, RGB(255, 255, 255));
		TitleStageClean.LoadBitmap(stageclaerTitle, RGB(255, 255, 255));
		// Object LoadBitmap
		_anthony.LoadBitmap();
		_whip.LoadBitmap();
		timer.LoadBitmap();
		expBar.LoadBitmap();
		for (int i = 0; i < sizeof(_bat1) / sizeof(_bat1[0]); i++) {
			_bat1[i].LoadBitmap();
		}
		for (int i = 0; i < sizeof(_bat2) / sizeof(_bat2[0]); i++) {
			_bat2[i].LoadBitmap();
		}
		for (int i = 0; i < sizeof(_bat3) / sizeof(_bat3[0]); i++) {
			_bat3[i].LoadBitmap();
		}
		for (int i = 0; i < sizeof(_bat4) / sizeof(_bat4[0]); i++) {
			_bat4[i].LoadBitmap();
		}
		ShowInitProgress(50);
		Sleep(300); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
		//
		// ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
		//
	}
	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25; // keyboard���b�Y
		const char KEY_UP = 0x26; // keyboard�W�b�Y
		const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
		const char KEY_DOWN = 0x28; // keyboard�U�b�Y
		if (nChar == KEY_LEFT)
			_anthony.setLeft(true);
		if (nChar == KEY_RIGHT)
			_anthony.setRight(true);
		if (nChar == KEY_UP)
			_anthony.setUp(true);
		if (nChar == KEY_DOWN)
			_anthony.setDown(true);
	}
	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_LEFT = 0x25;		// keyboard���b�Y
		const char KEY_UP = 0x26;		// keyboard�W�b�Y
		const char KEY_RIGHT = 0x27;	// keyboard�k�b�Y
		const char KEY_DOWN = 0x28;		// keyboard�U�b�Y
		end_count = 0;
		if (nChar == KEY_LEFT)
			_anthony.setLeft(false);
		if (nChar == KEY_RIGHT)
			_anthony.setRight(false);
		if (nChar == KEY_UP)
			_anthony.setUp(false);
		if (nChar == KEY_DOWN)
			_anthony.setDown(false);
	}
	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		end_count = 1;
		if (pause) {					// �P�_�O�_���Ȱ����A

			// �w�q�P�_�ϰ�
			CPoint btnEnd[2] = { CPoint(666, 550), CPoint(666 + btn_end.Width(), 550 + btn_end.Height()) };
			CRect judge = CRect(btnEnd[0], btnEnd[1]);
			if (judge.PtInRect(point)) { // �O�_�I���bBitmap�W
				GotoGameState(GAME_STATE_OVER);
			}
		}
	}
	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)		// �B�z�ƹ�
	{
	}
	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)		// �B�z�ƹ����ʧ@
	{
	}
	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		if (end_count == 1) {
			CAudio::Instance()->Stop(AUDIO_GAME_MUSIC);
			CAudio::Instance()->Play(AUDIO_GAMEOVER, false);
			GotoGameState(GAME_STATE_OVER);
		}
	}
	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// �B�z�ƹ����ʧ@
	{
		//eraser.SetMovingRight(false);
	}
	void CGameStateRun::OnShow()
	{
		// CMovingBitmaps Background
		background.ShowBitmap();					// �K�W�I����
		if (pause) {
			if (!_anthony.isAlive()) {
				deathBG.SetTopLeft(-157, -127);		// �K�W���`�I��
				deathBG.ShowBitmap();
			}
		}
		// Objects
		_whip.OnShow(_anthony.isWatchingLeft());
		expBar.OnShow();
		timer.OnShow();
		_anthony.OnShow();
		if (time > 0 && time < 120) {
			for (int i = 0; i < sizeof(_bat1) / sizeof(_bat1[0]); i++) {
				_bat1[i].OnShow();

			}
		}
		else if (time > 120 && time <= 300) {
			for (int i = 0; i < sizeof(_bat2) / sizeof(_bat2[0]); i++) {
				_bat2[i].OnShow();

			}
		}
		else if (time > 300 && time <= 450) {
			for (int i = 0; i < sizeof(_bat3) / sizeof(_bat3[0]); i++) {
				_bat3[i].OnShow();

			}
		}
		else if (time > 450 && time <= 600) {
			for (int i = 0; i < sizeof(_bat4) / sizeof(_bat4[0]); i++) {
				_bat4[i].OnShow();

			}
		}
		// Fonts
		// Show LV
		std::string lvs = "���šG" + to_string(_anthony.getLevel() + 1);
		const CString lv(lvs.c_str());
		CDC* pDC = CDDraw::GetBackCDC();
		CFont f, *fp;
		f.CreatePointFont(200, "Arial");
		fp = pDC->SelectObject(&f);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->TextOut(1450, 0, lv);
		pDC->SelectObject(fp);
		CDDraw::ReleaseBackCDC();
		// CMovingBitmaps Titles
		if (pause) {
			// Set postion
			btn_end.SetTopLeft(666, 550);
			TitleGameOver.SetTopLeft(468, 300);
			TitleStageClean.SetTopLeft(420, 300);
			// Show
			if (!_anthony.isAlive()) {
				TitleGameOver.ShowBitmap(2);
			}
			else {
				TitleStageClean.ShowBitmap(2);
			}
			btn_end.ShowBitmap();
		}
	}
}