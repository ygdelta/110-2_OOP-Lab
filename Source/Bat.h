namespace game_framework {
#pragma once
	class Bat {
	public:
		Bat();
		Bat(int x, int y, int _blood);
		~Bat();
		void LoadBitmap();
		void OnMove(int x, int y);
		void OnShow();
		void setMovingUp(bool state);
		void setMovingDown(bool state);
		void setMovingLeft(bool state);
		void setMovingRight(bool state);
		void setX(int x);
		void setY(int y);
		void attack();
		void setBlood(int hp);
		void initialize(int x, int y, int _blood);
		void beAttack(int x, int y, int damage, int count, bool attaking, int& cDmg);   //�y���ˮ`(�Q)
		bool isAlive();
		bool beHit(int x, int y, int damage, int count, bool attaking);					//�P�_�O�_�Q����
		bool dieInThisHit(int damage);													//�ΥH�p�������ΡA�P�_�����O�_�|�b����damage�ɦ��`
		int  getX();
		int  getY();
	private:
		int				_x, _y;			//�����y��
		int				attack_damage;
		int				blood;
		int				hit_times;
		CMovingBitmap	_images[3];		//�����R��ɹϹ�
		CAnimation		_animation;		//�������ʮɰʵe
		bool			movingUp;
		bool			movingDown;
		bool			movingLeft;
		bool			movingRight;
		double			slope;
	};
}