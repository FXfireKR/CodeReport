#pragma once
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <array>
#include <random>
#include <memory>
#include <cmath>

struct POINTDATA
{
	int x, y, r;
};

class CApplicationDlg : public CDialogEx
{
// 생성입니다.
public:
	CApplicationDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_APPLICATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBtnClick_Randomize();
	afx_msg void OnBtnClick_Reset();
	afx_msg void OnDestroy();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnRandomizeTick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRandomizeDone(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void _Initialize();
	void _GetEditboxValues();
	void _ClearBackspace();
	void _CalcCircle();
	void _Draw();
	void _DrawBigCircle(int x, int y, int iRadius);
	void _DrawCircle(int x, int y, int iRadius);
	void _RefreshStaticText(int index, int x, int y);
	bool _IsInCircleOutline(int x, int y, int cx, int cy, int rad);
	bool _IsInCircle(int x, int y, int cx, int cy, int rad);

	void _StartRandomizeThread();
	void _StopRandomizeThread();
	void _CleanupRandomizeThread();

	int _ClampInt(int value, int minValue, int maxValue) const;
	bool _IsAlmostLine() const;

	static UINT RandomizeThreadProc(LPVOID pParam);

private:
	CImage m_image;
	const int WIDTH = 800;
	const int HEIGHT = 680;

	CWnd* m_pEditBoxCircleRad = nullptr;
	CWnd* m_pEditBoxCircleThick = nullptr;
	std::array<CWnd*, 3> m_arrCoordTexts;

	int m_iClickCircleRad = 8;
	int m_iCircleThickness = 3;

	std::vector<POINTDATA> m_vecPt;
	bool m_bTargeting = false;
	UINT m_uFocusIdx = 0;

	CWinThread* m_pThread = nullptr;
	std::atomic_bool m_bThreading = false;

	std::mutex m_threadMutex;
	std::condition_variable m_threadCv;
	bool m_bStopThread = false;
};