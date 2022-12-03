#pragma once
namespace game_framework {
	class ExpBar {
	public:
		ExpBar();
		void LoadBitmap();
		void OnMove(int exp, int level);
		void OnShow();
	private:
		CMovingBitmap _bg, _exp;
		int			  persentage;
	};
}