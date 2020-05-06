#pragma once

#include <Windows.h>
#include <graphics.h>
#include <cstdlib>

#pragma comment(lib,"MSIMG32.lib")

enum class STYLE
{
	BS_DEFAULT = 0, BS_ONLYTEXT = 1, BS_IMAGE = 2           //BS_DEFALUT:Ĭ�ϰ�ť��������߿�����֣�BS_ONLYTEXT:ֻ�����ֵİ�ť;BS_IMAGE��ͼ��ť
};

class Button
{
	TCHAR text[20];											//��ť�ı�
	RECT rect;												//��ť��Χ
	IMAGE *img;												//����ͼƬ
	IMAGE *focusimg;										//���㱳��ͼƬ
	COLORREF textcolor;										//�ı���ɫ
	COLORREF textfocuscolor;								//�ı���Ӧ��ɫ
	COLORREF bkcolor;										//������ɫ
	COLORREF bkfocuscolor;									//������Ӧ��ɫ
	COLORREF linecolor;										//��Ե����ɫ
	bool drawline;											//�Ƿ���Ʊ�Ե��
	bool focus;												//�Ƿ��ý���
	bool click;												//���°�ť
	bool isfocus(MOUSEMSG);                                 //�жϹ���Ƿ��ڰ�ť��Χ��                                                
	STYLE style;                                            //��ť���
public:
	Button();
	void setStyle(STYLE);                                   //���ð�ť���
	void setPos(int,int);									//���ð�ťλ��
	void setSize(int,int);									//���ð�ť��С
	bool setImage(IMAGE*,IMAGE*);                           //����ͼ��ťͼ��
	void setbkColor(COLORREF);								//���ñ�����ɫ
	void setColor(COLORREF);								//�����ı���ɫ
	void setbkfocusColor(COLORREF);							//���ñ���������ɫ
	void setfocusColor(COLORREF);							//�����ı�������ɫ
	void setlineColor(COLORREF);							//���ñ�Ե����ɫ
	void setText(TCHAR* str);								//���ð�ť�ı�
	void draw();											//����  
	void isDrawline(bool);									//�����Ƿ���Ʊ߿���
	bool isclick(MOUSEMSG);									//���°�ťʱΪtrue
	void operator=(const Button&);							//���������=
};

class Scene
{
private:
	class Link_Button {
	private:
		Button button;
		Link_Button* next;
	public:
		Link_Button();
		bool checknext();									//�ж���һ�ڵ��Ƿ�Ϊ��
		Link_Button* getnext();								//��ȡ��һ�ڵ�
		void setnext(Link_Button*);							//������һ�ڵ�
		void setbutton(Button p);							//���øýڵ�
		void show();										//��ʾ�ýڵ�
		bool listen(MOUSEMSG);								//��ť����
	};
	Link_Button *first;
public:
	Scene();
	~Scene();
	void AddButton(Button pbutton);							//�򳡾������һ����ť
	void Show();											//��ʾ�����еĽڵ�
	int Listener(MOUSEMSG);									//��Ϣ����
	void Clear();											//��ճ����еİ�ť�ڵ�
};

//�ú�����Դ��EasyX�ٷ���վ
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg);