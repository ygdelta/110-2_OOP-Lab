#pragma once
namespace game_framework {
	class Whip {
	public:
		Whip();
		void LoadBitmap();
		void OnMove(int x, int y, bool left);
		void OnShow(bool left);
		void setLevel(int level);
		void setRange(int range);
		void addDmg(int i);
		void set_count(int level);
		int	 getDmg();
		int	 getX();
		int	 getY();
		bool isAttacking();
	private:
		int			_x, _y;
		int			_cd;
		int			damage;
		int			level;
		int			whip_count;
		double		range;
		bool		attacking;
		CAnimation _animations[5][2];
	};
}