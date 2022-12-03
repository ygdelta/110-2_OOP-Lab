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
		void hitMonster(Bat bat);		// �P�Ǫ��I��
		void initialize();
		void heal();					// �ɦ�
		bool deathAnimationDone();		// ���`�ʵe�O�_���񧹲�
	private:
		int							x, y, index, exp, level;
		int							blood;					// ��q
		int							hit;
		int							velocity;
		int							t;
		bool						movingUp;
		bool						movingDown;
		bool						movingLeft;
		bool						movingRight;
		bool						watchingLeft;			// �ΨӧP�_����ª���V
		bool						levelup;
		CAnimation					deathAniamtion[2];		// ���⦺�`�ʵe index: "0"������, "1"���k��
		CAnimation					right, left;
		CMovingBitmap				HP;
		CMovingBitmap				HP_down;
		std::vector<CMovingBitmap>  _images;
	};
}
