
// ApplicationDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Application.h"
#include "ApplicationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

#define WM_APP_RANDOMIZE_TICK  (WM_APP + 200)
#define WM_APP_RANDOMIZE_DONE  (WM_APP + 201)

struct RANDOMIZE_TICK_DATA
{
	int count = 0;
	long long elapsedMs = 0;
	CPoint points[3];
};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CApplicationDlg 대화 상자
CApplicationDlg::CApplicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_APPLICATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_APP_RANDOMIZE_TICK, &CApplicationDlg::OnRandomizeTick)
	ON_MESSAGE(WM_APP_RANDOMIZE_DONE, &CApplicationDlg::OnRandomizeDone)
	ON_BN_CLICKED(IDC_BUTTON1, &CApplicationDlg::OnBtnClick_Randomize)
	ON_BN_CLICKED(IDC_BUTTON2, &CApplicationDlg::OnBtnClick_Reset)
END_MESSAGE_MAP()


// CApplicationDlg 메시지 처리기

BOOL CApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	_Initialize();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CApplicationDlg::OnRandomizeTick(WPARAM wParam, LPARAM lParam)
{
	std::unique_ptr<RANDOMIZE_TICK_DATA> pData(
		reinterpret_cast<RANDOMIZE_TICK_DATA*>(lParam)
	);

	if (pData == nullptr)
		return 0;

	if (!m_bThreading)
		return 0;

	if (m_vecPt.size() < 3)
		return 0;

	_GetEditboxValues();

	for (int i = 0; i < 3; ++i)
	{
		m_vecPt[i].x = pData->points[i].x;
		m_vecPt[i].y = pData->points[i].y;
		m_vecPt[i].r = m_iClickCircleRad;
	}

	_Draw();

	CString title;
	title.Format(
		L"Randomize %d / 10, Thread Work: %lld ms",
		pData->count,
		pData->elapsedMs
	);

	SetWindowTextW(title);

	return 0;
}

LRESULT CApplicationDlg::OnRandomizeDone(WPARAM wParam, LPARAM lParam)
{
	_CleanupRandomizeThread();

	{
		std::lock_guard<std::mutex> lock(m_threadMutex);
		m_bStopThread = false;
	}

	m_bThreading = false;

	SetWindowTextW(L"Randomize Done");

	return 0;
}

void CApplicationDlg::_Initialize()
{
	m_pEditBoxCircleRad = GetDlgItem(IDC_EDIT1);
	assert(m_pEditBoxCircleRad);

	m_pEditBoxCircleThick = GetDlgItem(IDC_EDIT2);
	assert(m_pEditBoxCircleThick);

	CString str;
	str.Format(_T("%d"), m_iClickCircleRad);
	m_pEditBoxCircleRad->SetWindowTextW(str);

	str.Format(_T("%d"), m_iCircleThickness);
	m_pEditBoxCircleThick->SetWindowTextW(str);

	// reserve memory
	m_vecPt.reserve(3);

	// cimage bk
	int iBpp = 8;
	m_image.Create(WIDTH, HEIGHT, iBpp);
	if (iBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; ++i)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_image.SetColorTable(0, 256, rgb);
	}

	// coord static text
	m_arrCoordTexts.fill(nullptr);
	
	m_arrCoordTexts[0] = GetDlgItem(IDC_STATIC1);
	m_arrCoordTexts[1] = GetDlgItem(IDC_STATIC2);
	m_arrCoordTexts[2] = GetDlgItem(IDC_STATIC3);

	assert(m_arrCoordTexts[0]);
	assert(m_arrCoordTexts[1]);
	assert(m_arrCoordTexts[2]);

	m_arrCoordTexts[0]->ShowWindow(FALSE);
	m_arrCoordTexts[1]->ShowWindow(FALSE);
	m_arrCoordTexts[2]->ShowWindow(FALSE);
}

void CApplicationDlg::_GetEditboxValues()
{
	assert(m_pEditBoxCircleRad);
	assert(m_pEditBoxCircleThick);

	CString str1;
	m_pEditBoxCircleRad->GetWindowTextW(str1);
	int nVal = _ttoi(str1);
	if (nVal > 0)
		m_iClickCircleRad = nVal;

	m_pEditBoxCircleThick->GetWindowTextW(str1);
	nVal = _ttoi(str1);
	if (nVal > 0)
		m_iCircleThickness = nVal;
}

void CApplicationDlg::_ClearBackspace()
{
	for (int y = 0; y < HEIGHT; ++y)
	{
		BYTE* pRow = static_cast<BYTE*>(m_image.GetPixelAddress(0, y));
		memset(pRow, 0xFF, WIDTH);
	}
}

void CApplicationDlg::_CalcCircle()
{
	if (m_vecPt.size() < 3)
		return;

	if (_IsAlmostLine())
		return;

	double x1 = m_vecPt[0].x;
	double x2 = m_vecPt[1].x;
	double x3 = m_vecPt[2].x;

	double y1 = m_vecPt[0].y;
	double y2 = m_vecPt[1].y;
	double y3 = m_vecPt[2].y;

	double d = 2.0 * (
		x1 * (y2 - y3) +
		x2 * (y3 - y1) +
		x3 * (y1 - y2)
		);

	const double EPSILON = 1e-6;
	if (std::abs(d) < EPSILON)
		return;

	double a = x1 * x1 + y1 * y1;
	double b = x2 * x2 + y2 * y2;
	double c = x3 * x3 + y3 * y3;

	double cx = (
		a * (y2 - y3) +
		b * (y3 - y1) +
		c * (y1 - y2)
		) / d;

	double cy = (
		a * (x3 - x2) +
		b * (x1 - x3) +
		c * (x2 - x1)
		) / d;

	double dx = cx - x1;
	double dy = cy - y1;
	double r = std::sqrt(dx * dx + dy * dy);

	if (!std::isfinite(cx) || !std::isfinite(cy) || !std::isfinite(r))
		return;

	double viewDiag = std::sqrt(
		static_cast<double>(WIDTH * WIDTH + HEIGHT * HEIGHT)
	);

	if (r > viewDiag * 2.0)
		return;

	_DrawBigCircle(
		static_cast<int>(std::lround(cx)),
		static_cast<int>(std::lround(cy)),
		static_cast<int>(std::lround(r))
	);
}

void CApplicationDlg::_Draw()
{
	_ClearBackspace();

	// circle
	if (m_vecPt.size() == 3) {
		_CalcCircle();
	}

	// point
	for (int i = 0; i < 3; ++i) {
		if (i < m_vecPt.size()) {
			const auto& iter = m_vecPt[i];
			_DrawCircle(iter.x, iter.y, m_iClickCircleRad);
			_RefreshStaticText(i, iter.x, iter.y);
		}
		else {
			m_arrCoordTexts[i]->ShowWindow(FALSE);
		}
	}

	CClientDC dc(this);
	m_image.Draw(dc, 0, 0);
}

void CApplicationDlg::_DrawBigCircle(int x, int y, int iRadius)
{
	if (iRadius <= 0)
		return;

	int thick = m_iCircleThickness;

	if (thick < 1)
		thick = 1;

	if (thick > iRadius)
		thick = iRadius;

	int left = _ClampInt(x - iRadius, 0, WIDTH - 1);
	int right = _ClampInt(x + iRadius, 0, WIDTH - 1);
	int top = _ClampInt(y - iRadius, 0, HEIGHT - 1);
	int bottom = _ClampInt(y + iRadius, 0, HEIGHT - 1);

	long long outerR = iRadius;
	long long innerR = iRadius - thick;

	long long outerSq = outerR * outerR;
	long long innerSq = innerR * innerR;

	for (int i = top; i <= bottom; ++i)
	{
		BYTE* pRow = static_cast<BYTE*>(m_image.GetPixelAddress(left, i));

		for (int j = left; j <= right; ++j)
		{
			long long dx = j - x;
			long long dy = i - y;

			long long distSq = dx * dx + dy * dy;

			if (distSq <= outerSq && distSq >= innerSq)
			{
				pRow[j - left] = 0x00;
			}
		}
	}
}

void CApplicationDlg::_DrawCircle(int x, int y, int iRadius)
{
	if (iRadius <= 0)
		return;

	int left = _ClampInt(x - iRadius, 0, WIDTH - 1);
	int right = _ClampInt(x + iRadius, 0, WIDTH - 1);
	int top = _ClampInt(y - iRadius, 0, HEIGHT - 1);
	int bottom = _ClampInt(y + iRadius, 0, HEIGHT - 1);

	long long radiusSq = 1LL * iRadius * iRadius;

	for (int i = top; i <= bottom; ++i)
	{
		BYTE* pRow = static_cast<BYTE*>(m_image.GetPixelAddress(left, i));

		for (int j = left; j <= right; ++j)
		{
			long long dx = j - x;
			long long dy = i - y;

			long long distSq = dx * dx + dy * dy;

			if (distSq <= radiusSq)
			{
				pRow[j - left] = 0x00;
			}
		}
	}
}

void CApplicationDlg::_RefreshStaticText(int index, int x, int y)
{
	CWnd* pDlgWnd = m_arrCoordTexts[index];
	if (nullptr == pDlgWnd) return;

	CString str;
	str.Format(L"POINT %d ] x: %d, y: %d", index + 1, x, y);
	pDlgWnd->SetWindowTextW(str);
	pDlgWnd->ShowWindow(TRUE);
}

bool CApplicationDlg::_IsInCircleOutline(int x, int y, int cx, int cy, int rad)
{
	double dx = x - cx;
	double dy = y - cy;

	double dist = dx * dx + dy * dy;
	int outR = rad;
	int inR = rad - m_iCircleThickness;

	int outSq = outR * outR;
	int inSq = inR * inR;

	return dist <= outSq && dist >= inSq;
}

bool CApplicationDlg::_IsInCircle(int x, int y, int cx, int cy, int rad)
{
	double dx = x - cx;
	double dy = y - cy;

	double dist = dx * dx + dy * dy;
	return (dist < rad * rad) ? true : false;
}

void CApplicationDlg::_StartRandomizeThread()
{
	_CleanupRandomizeThread();

	{
		std::lock_guard<std::mutex> lock(m_threadMutex);
		m_bStopThread = false;
	}

	CWinThread* pThread = AfxBeginThread(
		RandomizeThreadProc,
		this,
		THREAD_PRIORITY_NORMAL,
		0,
		CREATE_SUSPENDED
	);

	if (pThread == nullptr)
		return;

	pThread->m_bAutoDelete = FALSE;

	m_pThread = pThread;
	m_bThreading = true;

	pThread->ResumeThread();
}

void CApplicationDlg::_StopRandomizeThread()
{
	{
		std::lock_guard<std::mutex> lock(m_threadMutex);
		m_bStopThread = true;
	}

	m_threadCv.notify_all();
}

void CApplicationDlg::_CleanupRandomizeThread()
{
	if (m_pThread == nullptr)
		return;

	::WaitForSingleObject(m_pThread->m_hThread, INFINITE);

	delete m_pThread;
	m_pThread = nullptr;
}

int CApplicationDlg::_ClampInt(int value, int minValue, int maxValue) const
{
	if (value < minValue)
		return minValue;

	if (value > maxValue)
		return maxValue;

	return value;
}

bool CApplicationDlg::_IsAlmostLine() const
{
	if (m_vecPt.size() < 3)
		return true;

	const POINTDATA& a = m_vecPt[0];
	const POINTDATA& b = m_vecPt[1];
	const POINTDATA& c = m_vecPt[2];

	double abx = b.x - a.x;
	double aby = b.y - a.y;

	double acx = c.x - a.x;
	double acy = c.y - a.y;

	double area2 = std::abs(abx * acy - aby * acx);

	double abSq = abx * abx + aby * aby;

	double bcx = c.x - b.x;
	double bcy = c.y - b.y;
	double bcSq = bcx * bcx + bcy * bcy;

	double cax = a.x - c.x;
	double cay = a.y - c.y;
	double caSq = cax * cax + cay * cay;

	double maxLenSq = abSq;
	if (bcSq > maxLenSq) maxLenSq = bcSq;
	if (caSq > maxLenSq) maxLenSq = caSq;

	if (maxLenSq < 1.0)
		return true;

	double base = std::sqrt(maxLenSq);
	double height = area2 / base;

	const double MIN_HEIGHT = 4.0;

	return height < MIN_HEIGHT;
}

void CApplicationDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	_GetEditboxValues();	

	// Focusing check
	for (int i = 0; i < m_vecPt.size(); ++i)
	{
		POINTDATA& iter = m_vecPt[i];
		if (_IsInCircle(point.x, point.y, iter.x, iter.y, iter.r))
		{
			m_bTargeting = true;
			m_uFocusIdx = i;
			return;
		}
	}

	// input check
	if (m_vecPt.size() < 3)
	{
		m_vecPt.push_back({ point.x, point.y, m_iClickCircleRad });
	}

	_Draw();
}

void CApplicationDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTargeting) return;
	if (m_vecPt.size() <= m_uFocusIdx) return;

	POINTDATA& refData = m_vecPt[m_uFocusIdx];

	refData.x = point.x;
	refData.y = point.y;

	_Draw();
}

void CApplicationDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bTargeting = false;
}

void CApplicationDlg::OnBtnClick_Randomize()
{
	if (m_vecPt.size() < 3)
		return;

	_GetEditboxValues();

	if (m_bThreading)
	{
		_StopRandomizeThread();
		return;
	}

	_StartRandomizeThread();
}

void CApplicationDlg::OnBtnClick_Reset()
{
	m_vecPt.clear();
	_Draw();
}

void CApplicationDlg::OnDestroy()
{
	_StopRandomizeThread();
	_CleanupRandomizeThread();

	CDialogEx::OnDestroy();
}

UINT CApplicationDlg::RandomizeThreadProc(LPVOID pParam)
{
	CApplicationDlg* pDlg = reinterpret_cast<CApplicationDlg*>(pParam);
	if (pDlg == nullptr)
		return 0;

	HWND hWnd = pDlg->GetSafeHwnd();

	using Clock = std::chrono::steady_clock;

	constexpr int RepeatCount = 10;
	const auto Interval = std::chrono::milliseconds(500);

	std::random_device rd;
	std::mt19937 rng(rd());

	std::uniform_int_distribution<int> distX(0, pDlg->WIDTH - 1);
	std::uniform_int_distribution<int> distY(0, pDlg->HEIGHT - 1);

	auto nextTime = Clock::now();

	for (int count = 1; count <= RepeatCount; ++count)
	{
		nextTime += Interval;

		{
			std::unique_lock<std::mutex> lock(pDlg->m_threadMutex);

			bool stopped = pDlg->m_threadCv.wait_until(
				lock,
				nextTime,
				[pDlg]()
				{
					return pDlg->m_bStopThread;
				}
			);

			if (stopped)
				break;
		}

		auto workBegin = Clock::now();

		auto* pData = new RANDOMIZE_TICK_DATA;
		pData->count = count;

		for (int i = 0; i < 3; ++i)
		{
			pData->points[i].x = distX(rng);
			pData->points[i].y = distY(rng);
		}

		auto workEnd = Clock::now();

		pData->elapsedMs =
			std::chrono::duration_cast<std::chrono::milliseconds>(
				workEnd - workBegin
			).count();

		if (::IsWindow(hWnd))
		{
			BOOL posted = ::PostMessage(
				hWnd,
				WM_APP_RANDOMIZE_TICK,
				0,
				reinterpret_cast<LPARAM>(pData)
			);

			if (!posted)
			{
				delete pData;
				break;
			}
		}
		else
		{
			delete pData;
			break;
		}

		if (nextTime < workEnd)
		{
			nextTime = workEnd;
		}
	}

	if (::IsWindow(hWnd))
	{
		::PostMessage(hWnd, WM_APP_RANDOMIZE_DONE, 0, 0);
	}

	return 0;
}