
// LuckDrawDlg.h : ͷ�ļ�
//

#pragma once

#include <vector>
#include "csv_parser.hpp"
#include "afxwin.h"

// CLuckDrawDlg �Ի���
class CLuckDrawDlg : public CDialogEx
{
// ����
public:
	CLuckDrawDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LUCKDRAW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	std::vector<SDataInfo> m_datas;
	void InitDatabase();

	int m_curValue;//��ǰ�����ֵ(Ĭ��Ϊ-1)
	bool bRandom;//�Ƿ����ڽ����������
	void ToggleRandom();//�����������
	void RandomData();//���һ������
	void UpdateRectAfterFullScreen(CRect & rImage);//ȫ��֮��ˢ�¿ؼ�λ��, rImage�Ǳ���ͼ��λ��

	// ��ǰ��ʾ������
	CStatic m_CurData;
	CFont m_Font;
	CFont * m_pFont;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
