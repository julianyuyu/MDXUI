
#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_

#include <fstream>

#define FONT_CHARDICT_FILENAME	L"chardict.txt"
#define CUSTOM_FONT		1

#include "creator.h"
#include "fontshader.h"
#include "orthorenderer.h"
#include "XMWrapper.h"

#include "defcharlist.h"

using namespace std;
using namespace WrappedXM;


#if 0
class TextFontModel
{
	struct FONTMODEL_VERTEX
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	TextFontModel(FontHashTable* hash, TEXCOORD* coord, ID3D11Device* d, ID3D11DeviceContext* dc = nullptr)
	{
		_Ctor();
		m_pDevice = d;
		m_pDevContext = dc;
		m_pHashTable = hash;
		m_pTexCoordTable = coord;
		m_bModelCreated = false;
	}
	TextFontModel(const TextFontModel& ref)
	{
		_Ctor();
		m_pDevice = ref.m_pDevice;
		m_pDevContext = ref.m_pDevContext;
		m_VertexCount = ref.m_VertexCount;
		m_IndexCount = ref.m_IndexCount;
		m_ScreenWidth = ref.m_ScreenWidth;
		m_ScreenHeight = ref.m_ScreenHeight;
		m_RectWidth = ref.m_RectWidth;
		m_RectHeight = ref.m_RectHeight;
		m_PosX = ref.m_PosX;
		m_PosY = ref.m_PosY;

		SetupModel(/*!!!!!!!!!!!!JULIAN!!!!!!!!!!*/nullptr, m_ScreenWidth, m_ScreenHeight, m_RectWidth, m_RectHeight);
	}
	virtual ~TextFontModel() {}

	inline FONTMODEL_VERTEX InitFontVertex(float x, float y, float z, float u, float v)
	{
		FONTMODEL_VERTEX vtx = {};
		vtx.position = D3DXVECTOR3(x, y, z);
		vtx.texture = D3DXVECTOR2(u, v);
		return vtx;
	}

	bool SetupModel(PWSTR szText, int sw, int sh, int w/*=0*/, int h/*=0*/);

	bool UpdateModel(int newPosX = 0, int newPosY = 0, ID3D11DeviceContext* dc = nullptr);
	void Active(ID3D11DeviceContext* dc = nullptr);
	int IndexCount() { return m_IndexCount; }

private:
	void _Ctor()
	{
		m_pDevice = nullptr;
		m_pDevContext = nullptr;
		m_pVb = nullptr;
		m_pIb = nullptr;
		m_VertexCount = 0;
		m_IndexCount = 0;
		m_ScreenWidth = 0;
		m_ScreenHeight = 0;
		m_RectWidth = 0;
		m_RectHeight = 0;
		m_PosX = 0;
		m_PosY = 0;
	}

	FONTMODEL_VERTEX* BuildVertexData(PWSTR szText);

	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pDevContext;

//	FontHashTable *m_pHashTable;
//	TEXCOORD *m_pTexCoordTable;

	ID3D11Buffer *m_pVb;
	ID3D11Buffer *m_pIb;
	int m_VertexCount;
	int m_IndexCount;

	int m_ScreenWidth;
	int m_ScreenHeight;
	int m_RectWidth;
	int m_RectHeight;
	int m_PosX;
	int m_PosY;
public:
	bool m_bModelCreated;
};
#endif

#define D3DFONT_THIN		0x1
#define D3DFONT_LIGHT		0x2
#define D3DFONT_BOLD		0x4
#define D3DFONT_HEAVY		0x8
#define D3DFONT_ITALIC		0x10
#define D3DFONT_UNDERLINE	0x20

const WCHAR SpecialCharList[] = L" \\";
//const WCHAR DefaultCharList[] = L" abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890+-*/=,.;:'~!@#$%^&<>()[]{}\"";
const LPCWSTR DefaultCharList = g_szDefCharList;

struct TEXCOORD
{
	//UV uv1;
	//UV uv2;
	float u1;
	float v1;
	float u2;
	float v2;
	int w;
	int h;
};

class FontHashTable
{
	struct Node
	{
		WCHAR key; // input this char, and need to return the index of it in the texture. so this char works as the key.
		int index; // this is what we need to search for. this is the payload.
		struct Node *next;
	};
	typedef struct Node *PNode;
public:
	FontHashTable() : m_Count(0), m_pTable(nullptr) {}
	FontHashTable(int count)
	{
		m_PayloadFactor = 0.7f;
		m_Count = (int)((float)count / m_PayloadFactor);
		m_pTable = new PNode[m_Count];
		ZeroMemory(m_pTable, sizeof(PNode) * m_Count);
	}

	~FontHashTable()
	{
		for (int i = 0; i < m_Count; i++)
		{
			Node *curr = m_pTable[i];
			Node *next = nullptr;
			while (curr)
			{
				next = curr->next;
				SAFEDELETE(curr);
				curr = next;
			}
		}
		SAFEDELETEARRAY(m_pTable);
	}

	int Append(WCHAR c, int iindex);
	int Find(WCHAR c);
private:
	int Hash1(WCHAR c);
	int Hash2(WCHAR c);

	Node **m_pTable;
	float m_PayloadFactor;
	int m_Count;
};

class FontRenderer : public OrthoRenderer
{
public:
	FontRenderer(ID3D11Device* dev, int w, int h) : OrthoRenderer(dev, w, h),
		m_FontShader(nullptr), m_pHashTable(nullptr), m_pTexCoordTable(nullptr), m_DictLength(0) {}
	virtual ~FontRenderer()
	{
#if !CUSTOM_FONT
		SAFEDELETEARRAY(m_Font);
#endif
		SAFERELEASE(m_pFontTexSRV);
		SAFEDELETE(m_FontShader);
		SAFEDELETE(m_pHashTable);
		SAFEDELETEARRAY(m_pTexCoordTable);
	}

	virtual bool Create(int FontSize=0, DWORD FontFlag = 0, PWSTR fontname = nullptr, PCWSTR charfilename = nullptr);
//	void BuildVertexArray(void*, PWSTR, float, float);
	virtual TEXCOORD* GetCoord(WCHAR c)
	{
		int idx = m_pHashTable->Find(c);

		if ((idx < 0) || (idx >= m_DictLength))
		{
			return nullptr;
		}

		return &(m_pTexCoordTable[idx]);
	}
	virtual void Render(int indexCount, WXMVECTOR4* color);
#if !CUSTOM_FONT
	virtual int GetSentencePixelLength(char*);
	virtual int GetFontHeight();
private:
	struct FontType
	{
		float left, right;
		int size;
	};
	virtual bool LoadFontData(char*);
	FontType* m_Font;
	float m_fontHeight;
	int m_spaceSize;
#endif
	//virtual int TexWidth() { return m_TexWidth; }
	//virtual int TexHeight() { return m_TexHeight; }
private:
	virtual int LoadCharacters(PCWSTR charfname, PWSTR* pstr, int* pLen = nullptr);

	ID3D11ShaderResourceView *m_pFontTexSRV;
	//UINT m_TexWidth;
	//UINT m_TexHeight;
	int m_DictLength;
	FontShader *m_FontShader;
	FontHashTable *m_pHashTable;
	TEXCOORD *m_pTexCoordTable;
};

#endif