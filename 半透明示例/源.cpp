#include "base.h"
#include "resource.h"
#include <Windows.h>
#include <graphics.h>

#pragma comment(lib,"winmm.lib")
// UI��̬���ӿ�
#pragma comment(lib,"EasyXGUI.lib")

const int SCREENW = 960;
const int SCREENH = 540;

// ʹ���ھ��У����ı��С
void SetWindowCenter(HWND);
// ��img��ָ�Ĵ�Сͼ���뱳��ͼ��alphaֵ���л�ϣ����������img��,��tcolorΪ�棬��img�Ƚ���ָ��ɫ������뱳�����
// ��������Ϊimg��С
void SetAlphaImageLayer(IMAGE* objimg, IMAGE* sourceimg, int x, int y, float alpha,bool tcolor = false,COLORREF transparentColor = WHITE);
// ���ó�������
void setfontset(LOGFONT*);
// ����ָ�������С
void SetFontHeight(LOGFONT* ofont, int size);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR szCmdLine, _In_ int iCmdShow)
{
	initgraph(SCREENW, SCREENH, NOCLOSE);
	SetWindowCenter(GetHWnd());
	setbkmode(TRANSPARENT);

	// �����Ϣ�ṹ��
	MOUSEMSG msg;
	// ��ʱ��
	DWORD sectime = 0;
	// ��ʾ֡�����õ��ַ���
	wchar_t fpsinfo[5];
	unsigned count = 0, fps = 0;
	// ������ʹ�õ�����
	LOGFONT font;
	setfontset(&font);
	bool mainloop = true;
	RECT rect = { 0,70,960,120 };
	IMAGE bkimg, objimg, titleimg, logo;
	IMAGE buttonnormalimg,buttonfocusimg;
	loadimage(&bkimg, TEXT("bk.jpg"), 960, 540);
	//loadimage(&logo, TEXT("�L�T��ϣ.png"), 400, 300);
	loadimage(&logo, TEXT("PNG"), TEXT("LOGO"), 400, 300);
	//ע�͵��Ĳ��ּ���objimgͼ��
	//loadimage(&objimg, TEXT("pre.jpg"), 960, 400);
	loadimage(&buttonnormalimg, TEXT("PNG"), TEXT("Bnormal"), 150, 30);
	loadimage(&buttonfocusimg, TEXT("PNG"), TEXT("Bfocus"), 150, 30);

	// �˳���ť
	Button exit_button;
	TCHAR text[] = L"�˳�";
	exit_button.setStyle(STYLE::BS_IMAGE);
	exit_button.setText(text);
	exit_button.setColor(WHITE);
	exit_button.setfocusColor(WHITE);
	exit_button.setlineColor(RGB(14, 105, 61));
	exit_button.isDrawline(true);
	exit_button.setSize(150, 30);
	exit_button.setPos(405, 425);
	exit_button.setImage(&buttonnormalimg, &buttonfocusimg);

	// ��objimgͼ��ĳ�ʼ��
	objimg.Resize(960, 400);
	SetWorkingImage(&objimg);
	setfillcolor(BLACK);
	solidrectangle(0, 0, objimg.getwidth(), objimg.getheight());
	SetWorkingImage(NULL);

	// titleimgͼ��ĳ�ʼ��
	titleimg.Resize(960, 50);
	SetWorkingImage(&titleimg);
	setfillcolor(BLACK);
	solidrectangle(0, 0, titleimg.getwidth(), titleimg.getheight());
	SetWorkingImage(NULL);

	// ʹobjimg��titleimg�뱳������͸������
	SetAlphaImageLayer(&objimg, &bkimg, 0, 70, 0.70f);
	SetAlphaImageLayer(&titleimg, &bkimg, 0, 70, 0.80f);
	// SetAlphaImageLayer(&logo, &objimg, 560, 100, 1.0f, true);

	BeginBatchDraw();
	while (mainloop)
	{
		// ���������Ϣ
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (exit_button.isclick(msg))
				mainloop = false;
		}
		// ���������Ϣ
		if (GetAsyncKeyState(VK_ESCAPE) && 0x8000)
			mainloop = false;

		// ÿ�����֡��
		if (sectime + 1000L <= timeGetTime())
		{
			fps = count;
			count = 0;
			sectime = timeGetTime();
		}
		// ÿ֡����
		count++;

		// ��Ⱦ
		cleardevice();
		putimage(0, 0, &bkimg);
		putimage(0, 70, &objimg);
		putimage(0, 70, &titleimg);
		transparentimage(NULL, 560, 260, &logo);
		settextcolor(WHITE);
		rect.top = 70;
		rect.bottom = 120;
		SetFontHeight(&font, 32);
		drawtext(TEXT("��͸��ͼ��ʾ��"), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		rect.top = 120;
		rect.bottom = 470;
		SetFontHeight(&font, 24);
		drawtext(TEXT("����һ���ı���������ʾһЩ������\n1234567890��abcdefg�����������������Τ��ۣ�\n��ͬ·���´���ͬ��ini�Ը������壬������ʾ��\n��͸��ʾ��.ini\n[General]\nFont=����\n\nע�⣬ini�ļ��ı�����ΪANSI�������������ֶ�ȡ����\n���⣬�˲���������ռ�õ�IMAGE��������������С���ʵ�ͼƬ��\n���½�logo\"�L�T��ϣ��������ʧ\"����Ƕ��Դ��������ʧ�ġ�"), &rect, DT_WORDBREAK);
		settextcolor(RED);
		outtextxy(0, 422, TEXT("�����˳���ť��Esc���˳��ó���"));
		settextcolor(WHITE);
		wsprintf(fpsinfo, TEXT("%u"), fps);
		outtextxy(0, 0, fpsinfo);
		exit_button.draw();

		// ���
		FlushBatchDraw();

		Sleep(16);
	}
	EndBatchDraw();
	closegraph();
}

void SetAlphaImageLayer(IMAGE* objimg, IMAGE* sourceimg, int x, int y, float alpha, bool tcolor,COLORREF transparentColor)
{
	DWORD* obj = GetImageBuffer(objimg);
	IMAGE tmp;
	// ��ԭʼͼ���л�ȡ�ü���ͼ��
	SetWorkingImage(sourceimg);
	getimage(&tmp, x, y, objimg->getwidth(), objimg->getheight());
	SetWorkingImage(NULL);
	DWORD* source = GetImageBuffer(&tmp);

	//transparentColor = 0xff000000 | BGR(transparentColor);
	
	for (int i = 0; i < objimg->getheight() * objimg->getwidth(); i++)
	{
		if (tcolor && (obj[i] & 0xff000000) >> 24 <= 75)
			obj[i] = source[i];
		else
			obj[i] = RGB(GetRValue(obj[i]) * alpha + GetRValue(source[i]) * (1 - alpha), GetGValue(obj[i]) * alpha + GetGValue(source[i]) * (1 - alpha), GetBValue(obj[i]) * alpha + GetBValue(source[i]) * (1 - alpha));
	}
}

void setfontset(LOGFONT *font)
{
	wchar_t str[32];
	gettextstyle(font);
	GetPrivateProfileString(TEXT("General"), TEXT("Font"), TEXT("����"), str, 32, TEXT(".\\��͸��ʾ��.ini"));
	wcscpy_s(font->lfFaceName, str);
	font->lfHeight = 32;
	font->lfQuality = PROOF_QUALITY | ANTIALIASED_QUALITY;
	settextstyle(font);
}

void SetFontHeight(LOGFONT* ofont, int size)
{
	gettextstyle(ofont);
	ofont->lfHeight = size;
	settextstyle(ofont);
}

void SetWindowCenter(HWND hwnd)
{
	int x = (GetSystemMetrics(SM_CXSCREEN) - SCREENW) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - SCREENH) / 2;
	RECT rect;

	GetWindowRect(hwnd, &rect);
	MoveWindow(hwnd, x, y, rect.right - rect.left, rect.bottom - rect.top, true);
}