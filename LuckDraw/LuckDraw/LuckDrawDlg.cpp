
// LuckDrawDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LuckDraw.h"
#include "LuckDrawDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLuckDrawDlg �Ի���

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

// CLuckDrawDlg ��Ϣ�������

BOOL CLuckDrawDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//��ʼ�����ݿ�
	InitDatabase();
	//��������
	m_Font.CreatePointFont(150, TEXT("Consolar"), NULL);
	m_CurData.SetFont(&m_Font);
	m_pFont = m_CurData.GetFont();
	LOGFONT lf;
	m_pFont->GetLogFont(&lf);
	lf.lfHeight = 50;
	m_pFont->CreateFontIndirectW(&lf);
	m_CurData.SetFont(m_pFont, true);
	//����ȫ��
	if (gFullScreen)
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLuckDrawDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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
			// ���´���
			m_CurData.UpdateWindow();
			// �����ð�ɫ���
			memDC.FillSolidRect(rect, RGB(0, 255, 255));
			// ����Ϊ͸��ģʽ
			memDC.SetBkMode(TRANSPARENT);
			memDC.SetTextColor(RGB(255, 255, 0));
			std::string tDisplayText = m_datas[m_curValue].name + " ";
			tDisplayText.append(m_datas[m_curValue].phone);
			CFont * pOldFont = memDC.SelectObject(m_pFont);
			CString tString = s2ws(tDisplayText).c_str();
			CSize textSize = memDC.GetTextExtent(tString);
			memDC.TextOut(rect.Width() / 2 - textSize.cx / 2, rect.Height() / 2 - textSize.cy / 2, tString);
			// ���ڴ�DC�е����ݿ������豸DC��
			pDCShow->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

			// ����
			memDC.SelectObject(pOldBmp);
			memBmp.DeleteObject();
			memDC.DeleteDC();
			m_CurData.ReleaseDC(pDCShow);
		}
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLuckDrawDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLuckDrawDlg::UpdateRectAfterFullScreen(CRect & rImage)
{
	CRect tWindowRect;
	//���ȫ����Ĵ��ڴ�С
	GetWindowRect(&tWindowRect);

	//��������ͼƬ�Ŀ��
	float imagePosX, imagePosY, imageFinalWidth, imageFinalHeight, imageScale;
	imageFinalWidth = tWindowRect.Width();
	imageScale = imageFinalWidth / 10630;//�������տ�Ȼ��ͼƬ���ű���
	imageFinalHeight = 4134 * imageScale;

	//����ͼƬ��ʾ��λ��
	imagePosX = 0;
	imagePosY = tWindowRect.Height() / 2 - imageFinalHeight / 2;//����Ļ��һ��߶ȼ�ȥͼƬ��һ��߶�
	//���ر���ͼ��λ��
	rImage.SetRect(imagePosX, imagePosY, imageFinalWidth, imagePosY + imageFinalHeight);

	//�����ǳ���ʾ���λ��
	float textScalePosLeft, textScalePosTop, textScaleWidth, textScaleHeight, textFinalPosX, textFinalPosY;
	textScalePosLeft = 2074.0f / 10630;
	textFinalPosX = textScalePosLeft * imageFinalWidth;//��ԭʼ�����������ź�Ŀ��
	textScalePosTop = 1489.0f / 4134;
	textFinalPosY = imagePosY + textScalePosTop * imageFinalHeight;//��ԭʼ�����������ź�ĸ߶ȣ�����ͼƬ��ƫ��
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
	//����Ϊ3��ˢ��������Ϣ�ķ���
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
