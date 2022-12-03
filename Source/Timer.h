#pragma once
namespace game_framework {
	class Timer {
	public:
		Timer();
		void			LoadBitmap();
		void			OnMove();
		void			OnShow();
		void			addOneSec();
		void			initialTime();
		int				getTime();
	private:
		int				_time;
		CMovingBitmap	mid;
		CAnimation		min1, min2, sec1, sec2;
	};
}