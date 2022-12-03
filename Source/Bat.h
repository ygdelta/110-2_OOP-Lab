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
		void beAttack(int x, int y, int damage, int count, bool attaking, int& cDmg);   //造成傷害(被)
		bool isAlive();
		bool beHit(int x, int y, int damage, int count, bool attaking);					//判斷是否被打到
		bool dieInThisHit(int damage);													//用以計算擊殺用，判斷蝙蝠是否會在受到damage時死亡
		int  getX();
		int  getY();
	private:
		int				_x, _y;			//蝙蝠座標
		int				attack_damage;
		int				blood;
		int				hit_times;
		CMovingBitmap	_images[3];		//蝙蝠靜止時圖像
		CAnimation		_animation;		//蝙蝠移動時動畫
		bool			movingUp;
		bool			movingDown;
		bool			movingLeft;
		bool			movingRight;
		double			slope;
	};
}