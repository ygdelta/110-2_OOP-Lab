#include<vector>
#include "Bat.h"
namespace game_framework {
	class Anthony {//Character1
	public:
		Anthony();
		void LoadBitmap();
		void OnMove();
		void OnShow();
		void setUp(bool set);
		void setDown(bool set);
		void setLeft(bool set);
		void setRight(bool set);
		void addExp(int item);
		int  getX();
		int  getY();
		int  getExp();
		int  getLevel();
		bool movingUpState();
		bool movingDownState();
		bool movingLeftState();
		bool movingRightState();
		bool isWatchingLeft();
		bool isAlive();
		bool levelUP();
		void hitMonster(Bat bat);		// 與怪物碰撞
		void initialize();
		void heal();					// 補血
		bool deathAnimationDone();		// 死亡動畫是否播放完畢
	private:
		int							x, y, index, exp, level;
		int							blood;					// 血量
		int							hit;
		int							velocity;
		int							t;
		bool						movingUp;
		bool						movingDown;
		bool						movingLeft;
		bool						movingRight;
		bool						watchingLeft;			// 用來判斷角色朝的方向
		bool						levelup;
		CAnimation					deathAniamtion[2];		// 角色死亡動畫 index: "0"為左邊, "1"為右邊
		CAnimation					right, left;
		CMovingBitmap				HP;
		CMovingBitmap				HP_down;
		std::vector<CMovingBitmap>  _images;
	};
}
