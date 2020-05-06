#pragma once

#include <Windows.h>
#include <graphics.h>
#include <cstdlib>

#pragma comment(lib,"MSIMG32.lib")

enum class STYLE
{
	BS_DEFAULT = 0, BS_ONLYTEXT = 1, BS_IMAGE = 2           //BS_DEFALUT:默认按钮，包括外边框和文字；BS_ONLYTEXT:只有文字的按钮;BS_IMAGE：图像按钮
};

class Button
{
	TCHAR text[20];											//按钮文本
	RECT rect;												//按钮范围
	IMAGE *img;												//背景图片
	IMAGE *focusimg;										//焦点背景图片
	COLORREF textcolor;										//文本颜色
	COLORREF textfocuscolor;								//文本响应颜色
	COLORREF bkcolor;										//背景颜色
	COLORREF bkfocuscolor;									//背景响应颜色
	COLORREF linecolor;										//边缘线颜色
	bool drawline;											//是否绘制边缘线
	bool focus;												//是否获得焦点
	bool click;												//按下按钮
	bool isfocus(MOUSEMSG);                                 //判断光标是否在按钮范围内                                                
	STYLE style;                                            //按钮风格
public:
	Button();
	void setStyle(STYLE);                                   //设置按钮风格
	void setPos(int,int);									//设置按钮位置
	void setSize(int,int);									//设置按钮大小
	bool setImage(IMAGE*,IMAGE*);                           //设置图像按钮图案
	void setbkColor(COLORREF);								//设置背景颜色
	void setColor(COLORREF);								//设置文本颜色
	void setbkfocusColor(COLORREF);							//设置背景焦点颜色
	void setfocusColor(COLORREF);							//设置文本焦点颜色
	void setlineColor(COLORREF);							//设置边缘线颜色
	void setText(TCHAR* str);								//设置按钮文本
	void draw();											//绘制  
	void isDrawline(bool);									//设置是否绘制边框线
	bool isclick(MOUSEMSG);									//按下按钮时为true
	void operator=(const Button&);							//重载运算符=
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
		bool checknext();									//判断下一节点是否为空
		Link_Button* getnext();								//获取下一节点
		void setnext(Link_Button*);							//设置下一节点
		void setbutton(Button p);							//设置该节点
		void show();										//显示该节点
		bool listen(MOUSEMSG);								//按钮监听
	};
	Link_Button *first;
public:
	Scene();
	~Scene();
	void AddButton(Button pbutton);							//向场景中添加一个按钮
	void Show();											//显示场景中的节点
	int Listener(MOUSEMSG);									//消息监听
	void Clear();											//清空场景中的按钮节点
};

//该函数来源于EasyX官方网站
void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg);