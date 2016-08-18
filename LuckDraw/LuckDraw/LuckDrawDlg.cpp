
// LuckDrawDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LuckDraw.h"
#include "LuckDrawDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLuckDrawDlg 对话框

#define TIMER_RANDOM 1

bool gFullScreen = true;

CLuckDrawDlg::CLuckDrawDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LUCKDRAW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLuckDrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_CurData);
}

BEGIN_MESSAGE_MAP(CLuckDrawDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

// CLuckDrawDlg 消息处理程序

BOOL CLuckDrawDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//初始化数据库
	InitDatabase();
	//设置字体
	m_Font.CreatePointFont(150, TEXT("Consolar"), NULL);
	m_CurData.SetFont(&m_Font);
	m_pFont = m_CurData.GetFont();
	LOGFONT lf;
	m_pFont->GetLogFont(&lf);
	lf.lfHeight = 50;
	m_pFont->CreateFontIndirectW(&lf);
	m_CurData.SetFont(m_pFont, true);
	//设置全屏
	if (gFullScreen)
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLuckDrawDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		if (m_curValue != -1)
		{
			CDC *  pDCShow;
			pDCShow = m_CurData.GetDC();
			CDC  memDC;
			CRect rect;
			CBitmap memBmp, *pOldBmp;
			m_CurData.GetClientRect(rect);
			memDC.CreateCompatibleDC(pDCShow);
			memBmp.CreateCompatibleBitmap(&memDC, rect.Width(), rect.Height());
			pOldBmp = memDC.SelectObject(&memBmp);
			// 更新窗口
			m_CurData.UpdateWindow();
			// 背景用白色填充
			memDC.FillSolidRect(rect, RGB(0, 255, 255));
			// 设置为透明模式
			memDC.SetBkMode(TRANSPARENT);
			memDC.SetTextColor(RGB(255, 255, 0));
			std::string tDisplayText = m_datas[m_curValue].name + " ";
			tDisplayText.append(m_datas[m_curValue].phone);
			CFont * pOldFont = memDC.SelectObject(m_pFont);
			CString tString = s2ws(tDisplayText).c_str();
			CSize textSize = memDC.GetTextExtent(tString);
			memDC.TextOut(rect.Width() / 2 - textSize.cx / 2, rect.Height() / 2 - textSize.cy / 2, tString);
			// 将内存DC中的内容拷贝到设备DC中
			pDCShow->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

			// 清理
			memDC.SelectObject(pOldBmp);
			memBmp.DeleteObject();
			memDC.DeleteDC();
			m_CurData.ReleaseDC(pDCShow);
		}
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLuckDrawDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLuckDrawDlg::UpdateRectAfterFullScreen(CRect & rImage)
{
	CRect tWindowRect;
	//获得全屏后的窗口大小
	GetWindowRect(&tWindowRect);

	//计算最终图片的宽高
	float imagePosX, imagePosY, imageFinalWidth, imageFinalHeight, imageScale;
	imageFinalWidth = tWindowRect.Width();
	imageScale = imageFinalWidth / 10630;//根据最终宽度获得图片缩放比率
	imageFinalHeight = 4134 * imageScale;

	//计算图片显示的位置
	imagePosX = 0;
	imagePosY = tWindowRect.Height() / 2 - imageFinalHeight / 2;//用屏幕的一半高度减去图片的一半高度
	//返回背景图的位置
	rImage.SetRect(imagePosX, imagePosY, imageFinalWidth, imagePosY + imageFinalHeight);

	//设置昵称显示框的位置
	float textScalePosLeft, textScalePosTop, textScaleWidth, textScaleHeight, textFinalPosX, textFinalPosY;
	textScalePosLeft = 2074.0f / 10630;
	textFinalPosX = textScalePosLeft * imageFinalWidth;//用原始比例乘以缩放后的宽度
	textScalePosTop = 1489.0f / 4134;
	textFinalPosY = imagePosY + textScalePosTop * imageFinalHeight;//用原始比例乘以缩放后的高度，加上图片的偏移
	textScaleWidth = (8439.0f - 2074) / 10630;
	textScaleHeight = (2066.0f - 1489) / 4134;
	//m_CurData.MoveWindow(textFinalPosX, textFinalPosY, textFinalPosX + textScaleWidth * imageFinalWidth, textFinalPosY + textScaleHeight * imageFinalHeight);
	m_CurData.MoveWindow(textFinalPosX, textFinalPosY, textScaleWidth * imageFinalWidth, textScaleHeight * imageFinalHeight);
}

BOOL CLuckDrawDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CDialogEx::OnEraseBkgnd(pDC);
	HBITMAP m_hBitmap;
	HDC m_hBkDC;
	CRect tImageDrawRect;
	UpdateRectAfterFullScreen(tImageDrawRect);
	m_hBitmap = ::LoadBitmap(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));
	m_hBkDC = ::CreateCompatibleDC(pDC->m_hDC);
	if (m_hBitmap && m_hBkDC)
	{
		::SelectObject(m_hBkDC, m_hBitmap);
		::StretchBlt(pDC->m_hDC, tImageDrawRect.left, tImageDrawRect.top, tImageDrawRect.Width(), tImageDrawRect.Height(), m_hBkDC, 0, 0, 10630, 4134, SRCCOPY);
		::DeleteObject(m_hBitmap);
		::DeleteObject(m_hBkDC);
	}
	//return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}


HBRUSH CLuckDrawDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 0));
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}


void CLuckDrawDlg::InitDatabase()
{
	csv_parser myDB("test.csv");
	myDB.ParseAllData(m_datas);
	bRandom = false;
	m_curValue = -1;
	return;
}

void CLuckDrawDlg::ToggleRandom()
{
	bRandom = !bRandom;
	if (!bRandom)
	{
		KillTimer(TIMER_RANDOM);
	}
	else
	{
		srand((unsigned)time(NULL));
		SetTimer(TIMER_RANDOM, 100, NULL);
	}
}

void CLuckDrawDlg::RandomData()
{
	m_curValue = rand() % m_datas.size();
	//std::string tDisplayText = m_datas[m_curValue].name + " ";
	//tDisplayText.append(m_datas[m_curValue].phone);
	//m_CurData.SetWindowTextW(s2ws(tDisplayText).c_str());
	//TRACE("=======%s\n", m_datas[cur].name.c_str());
	//以下为3中刷新文字信息的方法
	CRect rc;
	m_CurData.GetWindowRect(&rc);
	//ScreenToClient(&rc);
	InvalidateRect(rc, FALSE);

	/*m_CurData.ShowWindow(SW_HIDE);
	m_CurData.ShowWindow(SW_SHOW);*/

	//RedrawWindow();
}

void CLuckDrawDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case TIMER_RANDOM:
	{
		RandomData();
		break;
	}
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CLuckDrawDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	TRACE("===keydown====%d\n", nChar);
	switch (nChar)
	{
	case VK_OEM_3:
	case VK_PROCESSKEY:
		OnOK();
		break;
	case VK_SPACE:
		ToggleRandom();
		break;
	case VK_OEM_2:
		ModifyStyle(WS_CAPTION, 0, 0);
		MoveWindow(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		break;
	default:
		break;
	}
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}
