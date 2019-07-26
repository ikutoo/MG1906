#pragma once
#include "engine/sprite.h"

#define OBCHILD_MAX 11
#define OBJECT_NUM_MAX 10

typedef struct {
	float x, y;
	float u, v;
}VtPm_t;

//匯つのテクスチャ�`についての��夛悶
typedef struct {
	float x, y, z;//嶄伉泣
	VERTEX_3D Vertex[6];        //宙鮫喘��泣6��
} ObChild_t;

typedef struct {
	int Type;    // 0:鮫中に峠佩、1:鮫中に換岷
	int Img;    //鮫��
	int ImgSize;
	int ImgX1, ImgX2, ImgY1, ImgY2;
	float LargeX, LargeY;//�k罪の寄きさ(Typeが1の�rはLargeXがLargeZの叨護をする)
	float Zhaba;
	float FromZ, ToZ;    //どこからどこまで安佩きを�O協するか
	float FadeFromZ, FadeToZ;    //どこからどこまでフェ�`ドを�O協するか(��える鵬�gフェ�`ドアウト、�Fれる鵬�gフェ�`ドインする)
	int ObchindMax;
	ObChild_t ObChild[OBCHILD_MAX];
} Object_t;

class CBackground3d : public DxEngine::CNode
{
public:
	CBackground3d();
	~CBackground3d() {}

	void updateV(double vDeltaTime) override;
	void drawV() override;

private:
	Object_t m_Objects[OBJECT_NUM_MAX];
	int m_ObjectNum = 0;

	void __initObject(Object_t *Ob, int ImgHandle, int ImgSize, int ImgX1, int ImgY1, int ImgX2, int ImgY2, float LargeX, float LargeY,
		int Type, float FromZ, float FadeFromZ, float FadeToZ, float ToZ, float GraphX, float GraphY, int ObchildMax);
	void __clacObject();
	void __sortObject();
};