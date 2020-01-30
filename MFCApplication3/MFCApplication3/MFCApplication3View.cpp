
// MFCApplication3View.cpp : CMFCApplication3View ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
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
// ����������
GLuint gWVPLocation;
GLuint gColorLocation;

// �������
Camera* pGameCamera = NULL;
PersProjInfo gPersProjInfo;



const char* pVSFileName = "shader.vs";
const char* pFSFileName = "shader.fs";

// CMFCApplication3View ����/����

void BindCube(cube &c);

void CMFCApplication3View::RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);   // �����Ļ����Ȼ���

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);   

    Pipeline p;
    p.Rotate(0.0f, 0.0f, 0.0f);
    p.WorldPos(0.0f, 0.0f, 0.0f);
    // ����任

	p.SetCamera(GetDocument()->pos, GetDocument()->dir, Vector3f(0,0,1));

    p.SetPerspectiveProj(gPersProjInfo);

    // ��������λ��
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
	// TODO: �ڴ˴���ӹ������
	this->m_GLPixelIndex = 0;
    this->m_hGLContext = NULL;
}

BOOL CMFCApplication3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMFCApplication3View ����

void CMFCApplication3View::OnDraw(CDC* )
{
	CMFCApplication3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}
// CMFCApplication3View ��ӡ

BOOL CMFCApplication3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

// CMFCApplication3View ���

#ifdef _DEBUG
void CMFCApplication3View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication3Doc* CMFCApplication3View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication3Doc)));
	return (CMFCApplication3Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication3View ��Ϣ�������
bool CMFCApplication3View::SetWindowPixelFormat(HDC hDC)
{
    //���崰�ڵ����ظ�ʽ
    PIXELFORMATDESCRIPTOR pixelDesc=
    {
        sizeof(PIXELFORMATDESCRIPTOR),           //nSize�ṹ����
        1,                                       //nVersion�ṹ�汾
        PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL| 
        PFD_DOUBLEBUFFER|PFD_SUPPORT_GDI,        //dwFlags����OpenGL��δ�������
         /*
         wFlags�ܽ������±�־��
            PFD_DRAW_TO_WINDOW ʹ֮���ڴ��ڻ��������豸���ڻ�ͼ��
            PFD_DRAW_TO_BITMAP ʹ֮�����ڴ��е�λͼ��ͼ��
            PFD_SUPPORT_GDI ʹ֮�ܵ���GDI������ע�����ָ����PFD_DOUBLEBUFFER�����ѡ���Ч����
            PFD_SUPPORT_OpenGL ʹ֮�ܵ���OpenGL������
            PFD_GENERIC_FORMAT �����������ظ�ʽ��Windows GDI��������ɵ�����Ӳ���豸��������֧�֣�����ָ����һ�
            PFD_NEED_PALETTE ���߻������Ƿ���Ҫ��ɫ�壬�����������ɫ��ʹ��24�� 32λɫ�����Ҳ��Ḳ�ǵ�ɫ�壻
            PFD_NEED_SYSTEM_PALETTE �����־ָ���������Ƿ��ϵͳ��ɫ�嵱���������ɫ���һ���֣�
            PFD_DOUBLEBUFFER ָ��ʹ����˫��������ע��GDI������ʹ����˫�������Ĵ����л�ͼ����
            PFD_STEREO ָ�����һ������Ƿ�����ͼ������֯��
                        PFD_SWAP_LAYER_BUFFERS
         */
        PFD_TYPE_RGBA,  //iPixelType,��ɫģʽ����������PFD_TYPE_RGBA��ζ��ÿһλ(bit)�������rgb��������ֵ��PFD_TYPE_COLORINDEX��ζ��ÿһλ��������ڲ�ɫ���ұ��е�����ֵ
        24,   //cColorBits������ָ��һ����ɫ��λ������RGBA��˵��λ��������ɫ�к졢�̡�����������ռ��λ��������ɫ������ֵ��˵��ָ���Ǳ��е���ɫ����
        0,0,0,0,0,0,  //cRedBits��cRedShifts��cGreenBits��cGreenShifts��cBlueBits��cBlueShifts��,�����������ã�һ����0
                  //cRedBits��cGreenBits��cBlueBits������������Ӧ������ʹ�õ�λ����
                      //cRedShift��cGreenShift��cBlue-Shift������������������ɫ��ʼ��ƫ������ռ��λ����
        0,                                       //cAlphaBits,RGB��ɫ������Alpha��λ��                            
        0,                                 //cAlphaShift���Ѿ��������ã���0                   
        0,                                       //cAcuumBits�ۼƻ����λ��
        0,0,0,0,                                 //cAcuumRedBits/cAcuumGreenBits/cAcuumBlueBits/cAcuumAlphaBits,�����������ã���0
        32,                                      //cDepthBits��Ȼ����λ��
        0,                                       //cStencilBits,ģ�建���λ��
        0,                                       //cAuxBuffers�����������λ����һ����0
        PFD_MAIN_PLANE,                          //iLayerType,˵����������ͣ��ɺ�����0�������ڵİ汾������
                                                 //PFD_MAIN_PLANE,PFD_OVER_LAY_PLANE,PFD_UNDERLAY_PLANE
        0,0,0,0                                  //bReserved,dwLayerMask,dwVisibleMask,dwDamageMask,������0
    };

    this->m_GLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);//ѡ������������ظ�ʽ
    /*
    ChoosePixelFormat��������������һ����hDc����һ����һ��ָ��PIXELFORMATDESCRIPTOR�ṹ��ָ��&pixelDesc
    �ú������ش����ظ�ʽ������ֵ,�������0���ʾʧ�ܡ�
    ���纯��ʧ�ܣ�����ֻ�ǰ�����ֵ��Ϊ1���� DescribePixelFormat�õ����ظ�ʽ������
    ����������һ��û�õ�֧�ֵ����ظ�ʽ����ChoosePixelFormat���᷵������Ҫ������ظ�ʽ��ӽ���һ��ֵ
    һ�����ǵõ�һ�����ظ�ʽ������ֵ����Ӧ�����������ǾͿ��Ե���SetPixelFormat�������ظ�ʽ������ֻ������һ�Ρ�
    */
    if(this->m_GLPixelIndex==0)
    {//ѡ��ʧ��
        this->m_GLPixelIndex = 1;//Ĭ�ϵ����ظ�ʽ
        //��Ĭ�ϵ����ظ�ʽ��������
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
    //WglCreateContext��������һ���µ�OpenGL��Ⱦ������(RC)
    //����������������ڻ��Ƶ���hdc���ص��豸
    //�����Ⱦ�������к��豸������(dc)һ�������ظ�ʽ.
    this->m_hGLContext = wglCreateContext(hDC);//����RC

    if(this->m_hGLContext==NULL)
    {//����ʧ��
        return FALSE;
    }

    /*
    wglMakeCurrent �����趨OpenGL��ǰ�߳�(�߳������)����Ⱦ������
    �Ժ�����߳����е�OpenGL���ö��������hdc��ʶ���豸�ϻ��ơ�
    ��Ҳ����ʹ��wglMakeCurrent �������ı�����̵߳ĵ�ǰ��Ⱦ����
    ʹ֮�����ǵ�ǰ����Ⱦ������
    */
    if(wglMakeCurrent(hDC,this->m_hGLContext)==FALSE)
    {//ѡΪ��ǰRCʧ��
        return FALSE;
    }
    return TRUE;
}

int CMFCApplication3View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  �ڴ������ר�õĴ�������
    //�õ�һ�����ڶ���CWnd����������ָ��ľ����HWND��  
    HWND hWnd = this->GetSafeHwnd();  
    //GetDC�ú�������һָ�����ڵĿͻ������������Ļ����ʾ�豸�����Ļ����ľ��
    //�Ժ������GDI������ʹ�øþ�������豸�����Ļ����л�ͼ��
    HDC hDC = ::GetDC(hWnd);

    if(this->SetWindowPixelFormat(hDC)==FALSE)
    {//�������ظ�ʽ
        return 0;
    }
    if(this->CreateViewGLContext(hDC)==FALSE)
    {//����RC��ѡΪ����
        return 0;
    }
    if(!this->InitGL())
    {//��ʼ��openGL
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

    // TODO: �ڴ˴������Ϣ����������
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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CView::OnPaint()
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
	int num = GetDocument()->seq.totalNumber - GetDocument()->seq.present;
	for (int i=0; i < num; i++)
	{
		On32775();
		Sleep(500);
	}
}

void CMFCApplication3View::On32777()
{
	// TODO: �ڴ���������������
	int num = GetDocument()->seq.present;
	for (int i=0; i < num; i++)
	{
		OnEditUndo();
		Sleep(500);
	}
}
