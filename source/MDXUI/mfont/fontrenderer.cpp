#include "inc.h"
#include "fontrenderer.h"

int IntegerSqrt(int x)
{
	if (x <= 0)
		return 0;

	if (x == 1)
		return 1;

	bool bNeed = false;
	if (x < 64)
	{
		x *= 64;
		bNeed = true;
	}

	int t = x >> 1;
	int t0 = x;

	while (t >= 1)
	{
		if ((t*t == x) || (t0 - 1 == t))
		{
			if (bNeed)
			{
				return t / 8;
			}
			return t;
		}
		else if (t*t > x)
		{
			t0 = t;
			t = t >> 1;
		}
		else
		{
			t += (t0 - t) >> 1;
		}
	}
	return 0;
}

int FontHashTable::Hash2(WCHAR c)
{
	WORD d = (WORD)c;
	//d = d * d;
	//int tmp = (d & 0xF) + ((d & 0xF0) >> 4) + /*((d & 0xF00) >> 8) + */((d & 0xF000) >> 12);
	//tmp = tmp*tmp;
	//tmp = (tmp & 0xFFFF0) >> 4;
	int tmp = d;
	return (WORD)tmp % m_Count;
}

int FontHashTable::Hash1(WCHAR c)
{
	WORD d = (WORD)c;
	d = d * d;
	int tmp = (d & 0xF) + ((d & 0xF0) >> 4) + /*((d & 0xF00) >> 8) + */((d & 0xF000) >> 12);
	tmp = tmp*tmp;
	//tmp = (tmp & 0xFFFF0) >> 4;
	//int tmp = d;
	return (WORD)tmp % m_Count;
}

int FontHashTable::Append(WCHAR c, int index)
{
	int idx = Hash1(c);
	Node *n = m_pTable[idx];
	Node *last = nullptr;

	while (n)
	{
		last = n;
		n = n->next;
	}

	n = new Node;
	n->next = nullptr;
	n->index = index;
	n->key = c;

	if (!last)
	{
		m_pTable[idx] = n;
	}
	else
	{
		last->next = n;
	}

	return 0;
}

int FontHashTable::Find(WCHAR c)
{
	int idx = Hash1(c);
	Node *n = m_pTable[idx];

	while (n)
	{
		if (n->key == c)
			return n->index;
		else
		{
			n = n->next;
		}
	}
	return -1; // return -1 means no such char, here we return 0 if failed, so that if will return 1st char in this case.
}

class GdiFontCreator
{
public:
	GdiFontCreator() : m_hBitmap(NULL), m_hBitmapOld(NULL) {}
	~GdiFontCreator();
	DWORD* Create(WCHAR* charlist, int len, FontHashTable* htable, TEXCOORD* texcoords,
		DWORD Height = 0, DWORD Flags = 0, PWSTR fontname = nullptr);

	UINT Width() { return m_Width; }
	UINT Height() { return m_Height; }
private:

	// Paint the printable characters of current GDI font to DC
	bool PaintCharacters(FontHashTable* htable, WCHAR* charlist, int len, TEXCOORD* texcoords);
	bool CalcDimension(WCHAR* charlist, int len);

	UINT m_Width;
	UINT m_Height;
	int m_Spacing;

	HDC m_hdc;
	HFONT m_hFont;
	HFONT m_hFontOld;
	HBITMAP m_hBitmap;
	HGDIOBJ m_hBitmapOld;
	DWORD *m_pBmpData; // no need to delete, for it will be released when delete m_hBitmap.
};

GdiFontCreator::~GdiFontCreator()
{
	SelectObject(m_hdc, m_hBitmapOld);
	DeleteObject(m_hBitmap);
	SelectObject(m_hdc, m_hFontOld);
	DeleteObject(m_hFont);
	DeleteDC(m_hdc);
}

bool GdiFontCreator::CalcDimension(WCHAR* charlist, int len)
{
	SIZE size;
	WCHAR teststr[] = L"W";//L"M";

	// calculate the spacing between characters based on line height
	if (!GetTextExtentPoint32(m_hdc, teststr, 1, &size))
		return false;

	m_Spacing = (int)ceil(size.cy * 0.2f);

	// set start point
	UINT x = 0;
	UINT y = 0;
#if 0
	// rough calc
	int single_w = (/*m_Spacing + */size.cx + m_Spacing); // width of a char and the spacing
	int single_h = (/*m_Spacing + */size.cy + m_Spacing); // width of a char and the spacing
	bool bNeedMore = true;
	while (bNeedMore)
	{
		x = 0;
		y = 0;
		int i = 0;
		for (i = 0; i<len; i++)
		{
			x += single_w;

			if (x > m_Width)
			{
				x = single_w;
				y += single_h;
			}
			if (y > m_Height)
			{
				m_Width *= 2;
				m_Height *= 2;
				break;
			}
		}
		if (i == len)
		{
			bNeedMore = false;
		}
	}
#else
	// rough calc
	int single_h = (/*m_Spacing + */size.cx + m_Spacing);
	int count_h = IntegerSqrt(len);
	while (count_h * single_h > (int)m_Height)
	{
		m_Width *= 2;
		m_Height *= 2;
	}
#endif
	// double check
	x = 0;
	y = 0;
	int spacing2 = m_Spacing * 1;
	for (int i = 0; i < len; i++)
	{
		teststr[0] = charlist[i];
		if (!GetTextExtentPoint32(m_hdc, teststr, 1, &size))
		{
			return false;
		}

		x += size.cx + spacing2;

		if (x > m_Width)
		{
			x = size.cx + spacing2;
			y += size.cy + spacing2;
		}
		if (y > m_Height)
		{
			m_Width *= 2;
			m_Height *= 2;
			break; // should be enough, it's better to check one more time, but we stop here.
		}
	}

	return true;
}

bool GdiFontCreator::PaintCharacters(FontHashTable* htable, WCHAR* charlist, int len, TEXCOORD* texcoords)
{
	SIZE size;
	WCHAR str[] = L" ";

	int x = 0; //m_Spacing;
	int y = 0; //m_Spacing;
	int spacing2 = m_Spacing * 1;

	for (int i = 0; i < len; i++)
	{
		str[0] = charlist[i];

		if (str[0] != L'\n')
		{
			if (!GetTextExtentPoint32(m_hdc, str, 1, &size))
				return false;

			if ((DWORD)(x + size.cx + 0/*m_Spacing*/) > m_Width)
			{
				x = 0;// m_Spacing;
				y += size.cy + spacing2;
			}

			// paint the character
			if (!ExtTextOut(m_hdc, x, y, ETO_OPAQUE | ETO_NUMERICSLATIN, NULL, str, 1, NULL))
				return false;

			//m_TexCoord[c - 32][0] = ((FLOAT)(x - m_Spacing)) / m_Width;
			//m_TexCoord[c - 32][1] = ((FLOAT)(y)) / m_Height;
			//m_TexCoord[c - 32][2] = ((FLOAT)(x + size.cx + m_Spacing)) / m_Width;
			//m_TexCoord[c - 32][3] = ((FLOAT)(y + size.cy)) / m_Height;

			texcoords[i].u1 = ((float)(x /*- m_Spacing*/)) / (float)m_Width;
			texcoords[i].v1 = ((float)(y /*- m_Spacing*/)) / (float)m_Height;
			texcoords[i].u2 = ((float)(x + size.cx/* + 0*/)) / (float)m_Width;
			texcoords[i].v2 = ((float)(y + size.cy/* + 0*/)) / (float)m_Height;

			texcoords[i].w = size.cx;
			texcoords[i].h = size.cy;

			x += size.cx + spacing2;
		}
		htable->Append(charlist[i], i);
	}
	return true;
}

DWORD* GdiFontCreator::Create(WCHAR* charlist, int len, FontHashTable* htable, TEXCOORD* texcoords, DWORD Height, DWORD Flags, PWSTR fontname)
{
	m_hdc = CreateCompatibleDC(NULL);
	SetMapMode(m_hdc, MM_TEXT);

	if (!fontname)
	{
		fontname = L"Microsoft YaHei Light";
		//fontname = L"Segoe UI";
	}

	if (Height == 0)
		Height = 11;

	int	realHeight = -MulDiv(Height, GetDeviceCaps(m_hdc, LOGPIXELSY), 72);

	int weight = FW_NORMAL;

	if (Flags & D3DFONT_THIN)
		weight = FW_THIN;
	else if (Flags & D3DFONT_LIGHT)
		weight = FW_LIGHT;
	else if (Flags & D3DFONT_BOLD)
		weight = FW_BOLD;
	else if (Flags & D3DFONT_HEAVY)
		weight = FW_HEAVY;

	// specifying ANTIALIASED_QUALITY to get antialiased font.
	m_hFont = CreateFont(realHeight, 0, 0, 0, weight, (Flags & D3DFONT_ITALIC), (Flags & D3DFONT_UNDERLINE), FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, fontname);

	if (!m_hFont)
	{
		DeleteDC(m_hdc);
		return nullptr;
	}

	m_hFontOld = (HFONT)SelectObject(m_hdc, m_hFont);

	// init the dimension for the smallest power-2-tex which can hold all the chars
	m_Width = 128;
	m_Height = 128;
	CalcDimension(charlist, len);

	// create a bitmap for the font
	DWORD* pData;
	BITMAPINFO bi = {};
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = (int)m_Width;
	bi.bmiHeader.biHeight = -(int)m_Height;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biBitCount = 32;

	m_hBitmap = CreateDIBSection(m_hdc, &bi, DIB_RGB_COLORS, (void**)&pData, NULL, 0);
	m_hBitmapOld = SelectObject(m_hdc, m_hBitmap);
	// Set text properties
	SetTextColor(m_hdc, RGB(255, 255, 255));
	SetBkColor(m_hdc, RGB(0, 0, 0));
	SetTextAlign(m_hdc, TA_TOP);

	PaintCharacters(htable, charlist, len, texcoords);

#if 0
	// write to file for test.
	if (0)
	{
		BITMAPFILEHEADER bfh = {};
		bfh.bfType = ('B') | ('M') << 8;
		bfh.bfSize = m_Width * m_Height * 4 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		FILE *file = nullptr;
		fopen_s(&file, "font.bmp", "wb");
		fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), file);
		fwrite(&bi, 1, sizeof(BITMAPINFOHEADER), file);
		fwrite(pData, 1, m_Width* m_Height * 4, file);

		fclose(file);
	}
#endif

	m_pBmpData = pData;
	return pData;
}

int FontRenderer::LoadCharacters(PCWSTR charfname, PWSTR* pstr, int* plen)
{
	FILE *file = nullptr;
	PWSTR string = nullptr;
	int len = 0;

	if (_wfopen_s(&file, charfname, L"rt,ccs=UTF-8"))
	{
		return 0;
	}

	int filesize = 0;
	if ((fseek(file, 0, SEEK_END) < 0) ||
		((filesize = ftell(file)) < 0) ||
		(fseek(file, 0, SEEK_SET) < 0))
	{
		fclose(file);
		return 0;
	}

	int len0 = /*filesize; */(filesize + 1) / sizeof(WCHAR) + 1;
	PWSTR string0 = new WCHAR[len0];
	WCHAR ch;
	while (!feof(file))
	{
		ch = fgetwc(file);
		if (ch != 0xFFFF && ch != 0xFEFF /*&& ch != L' '*/ && ch != L'\t' && ch != L'\n' && ch != L'\0' &&ch != L'\r')
		{
			string0[len] = ch;
			len++;
		}
	}
	fclose(file);

	//len--; skip EOF, which is 0xFFFF
	if (len != len0)
	{
		string = new WCHAR[len + 1];
		memcpy(string, string0, (len)* sizeof(WCHAR));
		string[len] = L'\0';
	}

	SAFEDELETEARRAY(string0);
	*pstr = string;
	if (plen)
	{
		*plen = len;
	}
	return len;
}

bool FontRenderer::Create(int FontSize/*=0*/, DWORD FontFlag/* = 0*/, PWSTR fontname/* =nullptr*/, PCWSTR charfilename/* = nullptr*/)
{
	m_FontShader = new FontShader(m_pDevice);
//	m_FontShader->Create(L"data/font.vs", "FontVertexShader", L"data/font.ps", "FontPixelShader");
//	m_FontShader->Create(FontVSCode, "FontVSMain", FontPSCode, "FontPSMain");
	m_FontShader->Create();
//	InitMatrix(w, h);

#if CUSTOM_FONT
	WCHAR *string = nullptr;
	PCWSTR fname = charfilename;
	if (!fname)
		fname = FONT_CHARDICT_FILENAME;//L"font/character.txt";
	int len = LoadCharacters(fname, &string);

	if (len == 0)
	{
		string = _wcsdup(DefaultCharList);
		len = (int)wcslen(string);
		string = new WCHAR[len +1];
		wcscpy_s(string, len+1, DefaultCharList);
		string[len] = L'\0';
	}
	int len2 = len + (int)wcslen(SpecialCharList);
	WCHAR *string2 = new WCHAR[len2 + 1];
	//wcscpy_s(string2, len2+1, string);
	//wcscat_s(string2, len2+1, SpecialCharList);
	memcpy(string2, string, sizeof(WCHAR) * len);
	memcpy(((void *)((ULONG_PTR)string2 + 2*len)), SpecialCharList, sizeof(WCHAR) * (len2 - len));
	string2[len2] = L'\0';

	m_DictLength = len;
	m_pHashTable = new FontHashTable(len);
	m_pTexCoordTable = new TEXCOORD[len];

	GdiFontCreator fctr;
	DWORD *src = fctr.Create(string, len, m_pHashTable, m_pTexCoordTable, FontSize, FontFlag, fontname);
	SAFEDELETEARRAY(string);
	SAFEDELETEARRAY(string2);

	TextureCreator ctr(m_pDevice);
	UINT TexWidth = fctr.Width();
	UINT TexHeight = fctr.Height();
	
#if 0
	WORD *ptr = new WORD[m_TexWidth * m_TexHeight];
	ZeroMemory(ptr, sizeof(WORD)*m_TexWidth * m_TexHeight);
	WORD *curr = ptr;
	BYTE bAlpha;
	for (UINT y = 0; y < m_TexHeight; y++)
	{
		//curr = (WORD *)ptr0;
		for (UINT x = 0; x < m_TexWidth; x++)
		{
			bAlpha = (BYTE)((src[m_TexWidth * y + x] & 0xff) >> 4);
			if (bAlpha > 0)
			{
				*curr = (WORD)((bAlpha << 12) | 0x00fff);
			}
			else
			{
				*curr = 0x00000000;
			}
			curr++;
		}
		//curr += lr.Pitch;
	}

	m_pFontTexSRV = ctr.Create2D(nullptr, m_TexWidth, m_TexHeight, DXGI_FORMAT_B4G4R4A4_UNORM, 2, ptr);
	SAFEDELETEARRAY(ptr);
#else
#if 0
	DWORD *ptr = new DWORD[m_TexWidth * m_TexHeight];
	ZeroMemory(ptr, sizeof(DWORD)*m_TexWidth * m_TexHeight);
	DWORD *curr = ptr;
	BYTE bAlpha;
	for (UINT y = 0; y < m_TexHeight; y++)
	{
		//curr = (DWORD *)ptr0;
		for (UINT x = 0; x < m_TexWidth; x++)
		{
			bAlpha = (BYTE)((src[m_TexWidth * y + x] & 0xff) >> 4);
			if (bAlpha > 0)
			{
				*curr = (DWORD)((bAlpha << 24) | 0x00ffffff);
			}
			else
			{
				*curr = 0x00000000;
			}
			curr++;
}
		//curr += lr.Pitch;
	}
#endif
	DWORD *ptr = src;
	m_pFontTexSRV = ctr.Create2D(nullptr, TexWidth, TexHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 4, ptr);
//	SAFEDELETEARRAY(ptr);
#endif
#else
//	fontHeight = 64;
	m_fontHeight = fontHeight;
	m_spaceSize = spaceSize;

	// Load in the text file containing the font data.
	bool result = LoadFontData("data/font/font01.txt");
	if (!result)
	{
		return false;
	}

	TextureCreator tctr0(m_device);
	m_pFontTexSRV = tctr0.CreateFromFile(L"data/font/font01.tga", m_deviceContext);

#endif
	return true;
}

#if CUSTOM_FONT

#if 0
void FontRenderer::BuildVertexArray(void* ptr, PWSTR szText, float drawX, float drawY)
{
	FONT_VERTEX* vertices = (FONT_VERTEX*)ptr;

	// Get the number of letters in the sentence.
	int len = (int)wcslen(szText);

	FLOAT u1, v1, u2, v2;
	FLOAT w=0.f, h=0.f;
	FLOAT m_Spacing = 6.1f;
	FLOAT xstart = (FLOAT)(drawX + m_Spacing);
	FLOAT sx = xstart;
	FLOAT sy = (FLOAT)drawY;

	// Draw each letter onto a quad.
	for (int i = 0; i<len; i++)
	{
		WCHAR c = *szText++;
		if (c == L'\n')
		{
			sx = xstart;
			//sy += (m_TexCoord[0][3] - m_TexCoord[0][1]) * m_TexHeight;
			sy -= (h + m_Spacing); // use height of last char.
			continue;
		}

		int idx = m_pHashTable->Find(c);

		if (idx < 0)
			continue;

		u1 = m_pTexCoordTable[idx].u1;
		v1 = m_pTexCoordTable[idx].v1;
		u2 = m_pTexCoordTable[idx].u2;
		v2 = m_pTexCoordTable[idx].v2;

		//w = (u2 - u1) * m_TexWidth; //512.f;
		//h = (v2 - v1) * m_TexHeight; //512.f;
		w = m_pTexCoordTable[idx].w;
		h = m_pTexCoordTable[idx].h;

		// First triangle in quad.
		*vertices++ = InitFontVertex(sx,     sy,     0.0f, u1, v1);// Bottom right.
		*vertices++ = InitFontVertex(sx + w, sy - h, 0.0f, u2, v2);// Bottom left.
		*vertices++ = InitFontVertex(sx,     sy - h, 0.0f, u1, v2);// Top left.

		// Second triangle in quad.
		*vertices++ = InitFontVertex(sx,     sy,     0.0f, u1, v1);// Bottom right.
		*vertices++ = InitFontVertex(sx + w, sy,     0.0f, u2, v1);// Top left.
		*vertices++ = InitFontVertex(sx + w, sy - h, 0.0f, u2, v2);// Top right.

		sx += w;// +(1 * m_Spacing);
	}
}
#endif

#else

bool FontRenderer::LoadFontData(char* filename)
{
	ifstream fin;
	int i;
	char temp;


	// Create the font spacing buffer.
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	// Read in the font size and spacing between chars.
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for (i = 0; i<95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}

void FontRenderer::BuildVertexArray(void* vertices, PWSTR text, float drawX, float drawY)
{
	// Coerce the input vertices into a VertexType structure.
	FONT_VERTEX* vertexPtr = (FONT_VERTEX*)vertices;

	// Get the number of letters in the sentence.
	int len = (int)wcslen(text);

	// Initialize the index to the vertex array.
	int index = 0;
	int letter = 0;

	// Draw each letter onto a quad.
	for(int i=0; i<len; i++)
	{
		letter = ((int)text[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if(letter == 0 || letter >= (127 - 32))
		{
			drawX = drawX + (float)m_spaceSize;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - m_fontHeight), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - m_fontHeight), 0.0f);  // Bottom left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - m_fontHeight), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}

int FontRenderer::GetSentencePixelLength(char* sentence)
{
	int pixelLength, numLetters, i, letter;


	pixelLength = 0;
	numLetters = (int)strlen(sentence);

	for (i = 0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then count it as three pixels.
		if (letter == 0)
		{
			pixelLength += m_spaceSize;
		}
		else
		{
			pixelLength += (m_Font[letter].size + 1);
		}
	}

	return pixelLength;
}


int FontRenderer::GetFontHeight()
{
	return (int)m_fontHeight;
}
#endif

void FontRenderer::Render(int indexCount, WXMVECTOR4* color)
{
#if USE_LOCAL_RASTER_STATE
	AutoBlendStateHolder bsh(m_pImmeDC);
	AutoDSStateHolder dssh(m_pImmeDC);
	AutoRasterStateHolder rsh(m_pImmeDC, m_pRasterSolidState);
	EnableAlphaBlendState();
	DisableDepthState();
#endif
	m_FontShader->Render(indexCount, &m_MatView, &m_MatOrtho, color, m_pFontTexSRV);
}