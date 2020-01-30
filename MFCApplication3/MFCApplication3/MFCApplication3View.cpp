
// MFCApplication3View.cpp : CMFCApplication3View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication3.h"
#endif

#include "MFCApplication3Doc.h"
#include "MFCApplication3View.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>

#include <freeglut.h>

#include "ogldev_util.h"
#include "ogldev_glut_backend.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"
#include "cube.h"
#include "ogldev_math_3d.h"
#include "Move.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GLsizei WINDOW_WIDTH = 1024;
GLsizei WINDOW_HEIGHT = 768;

// CMFCApplication3View

IMPLEMENT_DYNCREATE(CMFCApplication3View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication3View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_CHAR()
	ON_COMMAND(ID_EDIT_UNDO, &CMFCApplication3View::OnEditUndo)
	ON_COMMAND(ID_32775, &CMFCApplication3View::On32775)
	ON_COMMAND(ID_32777, &CMFCApplication3View::On32777)
	ON_COMMAND(ID_32778, &CMFCApplication3View::On32778)
END_MESSAGE_MAP()

GLuint VBO;
GLuint FLOOR;
GLuint IBO[4];
// 更换变量名
GLuint gWVPLocation;
GLuint gColorLocation;

// 相机对象
Camera* pGameCamera = NULL;
PersProjInfo gPersProjInfo;



const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

// CMFCApplication3View 构造/析构

void BindCube(cube &c);

void CMFCApplication3View::RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);   // 清除屏幕及深度缓存

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);   

    Pipeline p;
    p.Rotate(0.0f, 0.0f, 0.0f);
    p.WorldPos(0.0f, 0.0f, 0.0f);
    // 相机变换

	p.SetCamera(GetDocument()->pos, GetDocument()->dir, Vector3f(0,0,1));

    p.SetPerspectiveProj(gPersProjInfo);

    // 传递最终位置
    glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());

	glUniform3f(gColorLocation, 1.0f, 1.0f, 1.0f);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, FLOOR);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	for(int i=0; i<GetDocument()->ru->n; i++)
	{
		BindCube(GetDocument()->ru->Cube[i]);
		glEnableVertexAttribArray(0);
		if(i%2 == 0)
			glUniform3f(gColorLocation, 0.9215f, 0.9215f, 0.9215f);
		else
			glUniform3f(gColorLocation, 0.9215f, 0.0f, 0.0f);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[1]);
		glDrawElements(GL_QUADS, 8, GL_UNSIGNED_INT, 0);

		if(i%2 == 0)
			glUniform3f(gColorLocation, 0.9215f, 0.0f, 0.0f);
		else
			glUniform3f(gColorLocation, 0.9215f, 0.9215f, 0.9215f);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[2]);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);

		glUniform3f(gColorLocation, 0.0f, 0.0f, 0.0f);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[3]);
		glDrawElements(GL_LINES, 18, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		
	}
//    glutSwapBuffers();
	
}

static void CreateVertexBuffer()
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void BindCube(cube &c)
{
	glBufferData(GL_ARRAY_BUFFER, 6*sizeof(Vector3f), c.getPoints(), GL_STATIC_DRAW);
}

static void CreateIndexBuffer()
{
	unsigned int Indices[] = {
		5,2,1,0,3,
		4,0,1,2,3,
		2,3,4,5,0,
		1,5,4,1,2,
		2,5,2,3,0,
		3,3,4,0,1,
		1,5,4,5,0,4};
	
    glGenBuffers(4, IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(Indices[0]), Indices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8*sizeof(Indices[0]), &Indices[6], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(Indices[0]), &Indices[14], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18*sizeof(Indices[0]), &Indices[18], GL_DYNAMIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(1);
    }

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0]= strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);
    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    string vs, fs;

    if (!ReadFile(pVSFileName, vs)) {
        exit(1);
    };

    if (!ReadFile(pFSFileName, fs)) {
        exit(1);
    };

    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glUseProgram(ShaderProgram);

    gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
	gColorLocation = glGetUniformLocation(ShaderProgram, "Color");
    assert(gWVPLocation != 0xFFFFFFFF);
}

CMFCApplication3View::CMFCApplication3View()
{
	// TODO: 在此处添加构造代码
	this->m_GLPixelIndex = 0;
    this->m_hGLContext = NULL;
}

BOOL CMFCApplication3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCApplication3View 绘制

void CMFCApplication3View::OnDraw(CDC* )
{
	CMFCApplication3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}
// CMFCApplication3View 打印

BOOL CMFCApplication3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

// CMFCApplication3View 诊断

#ifdef _DEBUG
void CMFCApplication3View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication3Doc* CMFCApplication3View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication3Doc)));
	return (CMFCApplication3Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication3View 消息处理程序
bool CMFCApplication3View::SetWindowPixelFormat(HDC hDC)
{
    //定义窗口的像素格式
    PIXELFORMATDESCRIPTOR pixelDesc=
    {
        sizeof(PIXELFORMATDESCRIPTOR),           //nSize结构长度
        1,                                       //nVersion结构版本
        PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL| 
        PFD_DOUBLEBUFFER|PFD_SUPPORT_GDI,        //dwFlags告诉OpenGL如何处理像素
         /*
         wFlags能接收以下标志：
            PFD_DRAW_TO_WINDOW 使之能在窗口或者其他设备窗口画图；
            PFD_DRAW_TO_BITMAP 使之能在内存中的位图画图；
            PFD_SUPPORT_GDI 使之能调用GDI函数（注：如果指定了PFD_DOUBLEBUFFER，这个选项将无效）；
            PFD_SUPPORT_OpenGL 使之能调用OpenGL函数；
            PFD_GENERIC_FORMAT 假如这种象素格式由Windows GDI函数库或由第三方硬件设备驱动程序支持，则需指定这一项；
            PFD_NEED_PALETTE 告诉缓冲区是否需要调色板，本程序假设颜色是使用24或 32位色，并且不会覆盖调色板；
            PFD_NEED_SYSTEM_PALETTE 这个标志指明缓冲区是否把系统调色板当作它自身调色板的一部分；
            PFD_DOUBLEBUFFER 指明使用了双缓冲区（注：GDI不能在使用了双缓冲区的窗口中画图）；
            PFD_STEREO 指明左、右缓冲区是否按立体图像来组织。
                        PFD_SWAP_LAYER_BUFFERS
         */
        PFD_TYPE_RGBA,  //iPixelType,颜色模式，包括两种PFD_TYPE_RGBA意味着每一位(bit)组代表着rgb各分量的值。PFD_TYPE_COLORINDEX意味着每一位组代表着在彩色查找表中的索引值
        24,   //cColorBits定义了指定一个颜色的位数。对RGBA来说，位数是在颜色中红、绿、蓝各分量所占的位数。对颜色的索引值来说，指的是表中的颜色数。
        0,0,0,0,0,0,  //cRedBits、cRedShifts、cGreenBits、cGreenShifts、cBlueBits、cBlueShifts用,基本不被采用，一般置0
                  //cRedBits、cGreenBits、cBlueBits用来表明各相应分量所使用的位数。
                      //cRedShift、cGreenShift、cBlue-Shift用来表明各分量从颜色开始的偏移量所占的位数。
        0,                                       //cAlphaBits,RGB颜色缓存中Alpha的位数                            
        0,                                 //cAlphaShift，已经不被采用，置0                   
        0,                                       //cAcuumBits累计缓存的位数
        0,0,0,0,                                 //cAcuumRedBits/cAcuumGreenBits/cAcuumBlueBits/cAcuumAlphaBits,基本不被采用，置0
        32,                                      //cDepthBits深度缓存的位数
        0,                                       //cStencilBits,模板缓存的位数
        0,                                       //cAuxBuffers，辅助缓存的位数，一般置0
        PFD_MAIN_PLANE,                          //iLayerType,说明层面的类型，可忽略置0，是早期的版本，包括
                                                 //PFD_MAIN_PLANE,PFD_OVER_LAY_PLANE,PFD_UNDERLAY_PLANE
        0,0,0,0                                  //bReserved,dwLayerMask,dwVisibleMask,dwDamageMask,必须置0
    };

    this->m_GLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);//选择最相近的像素格式
    /*
    ChoosePixelFormat接受两个参数：一个是hDc，另一个是一个指向PIXELFORMATDESCRIPTOR结构的指针&pixelDesc
    该函数返回此像素格式的索引值,如果返回0则表示失败。
    假如函数失败，我们只是把索引值设为1并用 DescribePixelFormat得到像素格式描述。
    假如你申请一个没得到支持的像素格式，则ChoosePixelFormat将会返回与你要求的像素格式最接近的一个值
    一旦我们得到一个像素格式的索引值和相应的描述，我们就可以调用SetPixelFormat设置像素格式，并且只需设置一次。
    */
    if(this->m_GLPixelIndex==0)
    {//选择失败
        this->m_GLPixelIndex = 1;//默认的像素格式
        //用默认的像素格式进行设置
        //
        if(DescribePixelFormat(hDC,this->m_GLPixelIndex,sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
        {
            return FALSE;
        }
    }

    if(SetPixelFormat(hDC,this->m_GLPixelIndex,&pixelDesc)==FALSE)
    {
        return FALSE;
    }
    return TRUE;
}

bool CMFCApplication3View::CreateViewGLContext(HDC hDC)
{
    //WglCreateContext函数创建一个新的OpenGL渲染描述表(RC)
    //此描述表必须适用于绘制到由hdc返回的设备
    //这个渲染描述表将有和设备上下文(dc)一样的像素格式.
    this->m_hGLContext = wglCreateContext(hDC);//创建RC

    if(this->m_hGLContext==NULL)
    {//创建失败
        return FALSE;
    }

    /*
    wglMakeCurrent 函数设定OpenGL当前线程(线程相关性)的渲染环境。
    以后这个线程所有的OpenGL调用都是在这个hdc标识的设备上绘制。
    你也可以使用wglMakeCurrent 函数来改变调用线程的当前渲染环境
    使之不再是当前的渲染环境。
    */
    if(wglMakeCurrent(hDC,this->m_hGLContext)==FALSE)
    {//选为当前RC失败
        return FALSE;
    }
    return TRUE;
}

int CMFCApplication3View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  在此添加您专用的创建代码
    //得到一个窗口对象（CWnd的派生对象）指针的句柄（HWND）  
    HWND hWnd = this->GetSafeHwnd();  
    //GetDC该函数检索一指定窗口的客户区域或整个屏幕的显示设备上下文环境的句柄
    //以后可以在GDI函数中使用该句柄来在设备上下文环境中绘图。
    HDC hDC = ::GetDC(hWnd);

    if(this->SetWindowPixelFormat(hDC)==FALSE)
    {//设置像素格式
        return 0;
    }
    if(this->CreateViewGLContext(hDC)==FALSE)
    {//创建RC并选为所用
        return 0;
    }
    if(!this->InitGL())
    {//初始化openGL
        return 0;
    }
    return 0;
}

bool CMFCApplication3View::InitGL()
{
	int argc = 1;
	char ** argv = new char*("a");
	glShadeModel(GL_SMOOTH);
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Make round points, not square points
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Antialias the lines
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return FALSE;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
    CreateVertexBuffer();
    CreateIndexBuffer();

	Vector3f floor[4];
	floor[0] = Vector3f(-15.0f, -15.0f, 0);
	floor[1] = Vector3f( 15.0f, -15.0f, 0);
	floor[2] = Vector3f( 15.0f,  15.0f, 0);
	floor[3] = Vector3f(-15.0f,  15.0f, 0);
	
	glGenBuffers(1, &FLOOR);
	glBindBuffer(GL_ARRAY_BUFFER, FLOOR);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);


    CompileShaders();
	

    gPersProjInfo.FOV = 60.0f;
    gPersProjInfo.zNear = 1.0f;
    gPersProjInfo.zFar = 300.0f;


	return TRUE;
}

void CMFCApplication3View::OnDestroy()
{
	CView::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
    if(wglGetCurrentContext()!=NULL)
    {
        wglMakeCurrent(NULL,NULL);
    }
    if(this->m_hGLContext!=NULL)
    {
        wglDeleteContext(this->m_hGLContext);
        this->m_hGLContext = NULL;
    }
}

void CMFCApplication3View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	WINDOW_WIDTH = cx;
	WINDOW_HEIGHT = cy;
    if (WINDOW_HEIGHT==0)                                        // Prevent A Divide By Zero By
    {
        WINDOW_HEIGHT=1;                                        // Making Height Equal One
    }
	
	gPersProjInfo.Height = WINDOW_HEIGHT;
    gPersProjInfo.Width = WINDOW_WIDTH;

    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);                        // Reset The Current Viewport 
}

void CMFCApplication3View::OnPaint()
{
	this->RenderSceneCB();;
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CView::OnPaint()
	SwapBuffers(dc.m_hDC);
}

Vector3f UpAndDown(float angle, Vector3f temp1)
{
	float t = sqrt(temp1.x * temp1.x + temp1.y * temp1.y);
	float t1 = cos(angle) * t - sin(angle) * temp1.z;
	return Vector3f(temp1.x * t1 / t, temp1.y * t1 / t, sin(angle) * t + cos(angle) * temp1.z);
}

void CMFCApplication3View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Vector3f temp = GetDocument()->dir;
	switch (nChar)
	{
	case VK_UP:
		GetDocument()->dir = UpAndDown(0.1f, GetDocument()->dir);
		break;
	case VK_DOWN:
		GetDocument()->dir = UpAndDown(-0.1f, GetDocument()->dir);
		break;
	case VK_RIGHT:
		GetDocument()->dir.x = cos(0.1f) * temp.x - sin(0.1f) * temp.y;
		GetDocument()->dir.y = sin(0.1f) * temp.x + cos(0.1f) * temp.y;
		break;
	case VK_LEFT:
		GetDocument()->dir.x = cos(-0.1f) * temp.x - sin(-0.1f) * temp.y;
		GetDocument()->dir.y = sin(-0.1f) * temp.x + cos(-0.1f) * temp.y;
		break;
	case VK_F1:
		GetDocument()->pos.z += 0.1f;
		break;
	case VK_SPACE:
		GetDocument()->pos.z -= 0.1f;
		break;
	default:
		;
		break;
	}
	OnPaint();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMFCApplication3View::OnLButtonDown(UINT nFlags, CPoint point)
{

	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Move MOVE;

	int re = MOVE.DoModal();
	
	if(re == IDOK)
	{
		if ((MOVE.place >= 1 && MOVE.place < GetDocument()->seq.dimension) || (MOVE.place < 0 && MOVE.place >= - GetDocument()->seq.dimension / 2))
		{
			for(int i = 0; i < GetDocument()->seq.totalNumber - GetDocument()->seq.present; i++)
			{
				GetDocument()->seq.place.pop_back();
				GetDocument()->seq.movement.pop_back();
			}

			GetDocument()->seq.present += 1;
			GetDocument()->seq.totalNumber = GetDocument()->seq.present;

			int angle = 1;
			if(MOVE.place%2 == 0)
				angle = -1;
				if(MOVE.Movement == "clockwise")
				{
					for(int i=0; i<90; i++)
					{
						GetDocument()->ru->rotate(MOVE.place, angle);
						OnPaint();
					}
					GetDocument()->seq.place.push_back(MOVE.place);
					GetDocument()->seq.movement.push_back(1);
				}
			else if(MOVE.Movement == "anti-clockwise")
				{
					for(int i=0; i<90; i++)
					{
						GetDocument()->ru->rotate(MOVE.place, -angle);
						OnPaint();
					}
					GetDocument()->seq.place.push_back(MOVE.place);
					GetDocument()->seq.movement.push_back(-1);
				}
			else
			{
				for(int i=0; i<90; i++)
				{
					GetDocument()->ru->rotate(MOVE.place, 2);
					OnPaint();
				}
				GetDocument()->seq.place.push_back(MOVE.place);
				GetDocument()->seq.movement.push_back(0);
			}
		}
		
}


	CView::OnLButtonDown(nFlags, point);
}

void CMFCApplication3View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Vector3f temp = GetDocument()->dir;
	Vector3f core = GetDocument()->ru->getCore();
	Vector3f dir = GetDocument()->pos - core;
	Vector3f left = dir.Cross(Vector3f(0, 0, 1));
	Vector3f up = dir.Cross(left);
	float t = sqrt(temp.x * temp.x + temp.y * temp.y);
	switch (nChar)
	{
	case 'w':
		GetDocument()->pos += GetDocument()->dir * (0.3f);
		break;
	case 's':
		GetDocument()->pos += GetDocument()->dir * (-0.3f);
		break;
	case 'd':
		GetDocument()->pos.x -= 0.3f * temp.y / t;
		GetDocument()->pos.y += 0.3f * temp.x / t;
		break;
	case 'a':
		GetDocument()->pos.x += 0.3f * temp.y / t;
		GetDocument()->pos.y -= 0.3f * temp.x / t;
		break;
	case 'j':
		GetDocument()->ru->rotate(core, up, -5.0f);
		break;
	case 'l':
		GetDocument()->ru->rotate(core, up, 5.0f);
		break;
	case 'o':
		GetDocument()->ru->rotate(core, dir, 5.0f);
		break;
	case 'u':
		GetDocument()->ru->rotate(core, dir, -5.0f);
		break;
	case 'k':
		GetDocument()->ru->rotate(core, left, -5.0f);
		break;
	case 'i':
		GetDocument()->ru->rotate(core, left, 5.0f);
		break;
	default:
		;
		break;
	}
	OnPaint();
	CView::OnChar(nChar, nRepCnt, nFlags);
}

void CMFCApplication3View::OnEditUndo()
{
	// TODO: 在此添加命令处理程序代码
	if (GetDocument()->seq.present == 0)
		return;
	GetDocument()->seq.present -= 1;
	int move = GetDocument()->seq.movement[GetDocument()->seq.present];
	if (move == 0)
		move = 2;
	if (GetDocument()->seq.place[GetDocument()->seq.present] % 2)
		move *= -1;
	for(int i=0; i<90; i++)
	{
		GetDocument()->ru->rotate(GetDocument()->seq.place[GetDocument()->seq.present], move);
		OnPaint();
	}
}


void CMFCApplication3View::On32775()
{
	// TODO: 在此添加命令处理程序代码
	if (GetDocument()->seq.present == GetDocument()->seq.totalNumber)
		return;
	int move = GetDocument()->seq.movement[GetDocument()->seq.present];
	if (move == 0)
		move = 2;
	if (GetDocument()->seq.place[GetDocument()->seq.present] % 2)
		move *= -1;
	for(int i=0; i<90; i++)
	{
		GetDocument()->ru->rotate(GetDocument()->seq.place[GetDocument()->seq.present], -move);
		OnPaint();
	}
	GetDocument()->seq.present += 1;
}

void CMFCApplication3View::On32778()
{
	// TODO: 在此添加命令处理程序代码
	int num = GetDocument()->seq.totalNumber - GetDocument()->seq.present;
	for (int i=0; i < num; i++)
	{
		On32775();
		Sleep(500);
	}
}

void CMFCApplication3View::On32777()
{
	// TODO: 在此添加命令处理程序代码
	int num = GetDocument()->seq.present;
	for (int i=0; i < num; i++)
	{
		OnEditUndo();
		Sleep(500);
	}
}
