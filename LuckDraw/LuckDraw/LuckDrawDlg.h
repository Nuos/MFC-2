
// LuckDrawDlg.h : 头文件
//

#pragma once

#include <vector>
#include "csv_parser.hpp"
#include "afxwin.h"

// CLuckDrawDlg 对话框
class CLuckDrawDlg : public CDialogEx
{
// 构造
public:
	CLuckDrawDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LUCKDRAW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	std::vector<SDataInfo> m_datas;
	void InitDatabase();

	int m_curValue;//当前的随机值(默认为-1)
	bool bRandom;//是否正在进行随机操作
	void ToggleRandom();//开关随机操作
	void RandomData();//随机一个数据
	void UpdateRectAfterFullScreen(CRect & rImage);//全屏之后刷新控件位置, rImage是背景图的位置

	// 当前显示的数据
	CStatic m_CurData;
	CFont m_Font;
	CFont * m_pFont;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
