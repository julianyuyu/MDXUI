#pragma once

#include "inc.h"
#include "creator.h"

class MUTTex
{
public:
	MUTTex(ID3D11Device* d, PWSTR texname, ID3D11DeviceContext *dc = nullptr, bool autogen = false) { m_pDevice = d; m_dc = dc; Initialize(texname, autogen); }
	MUTTex(ID3D11Device* d, void* data, int size, ID3D11DeviceContext *dc = nullptr, bool autogen = false) { m_pDevice = d; m_dc = dc; Initialize(data, size, autogen); }
	virtual ~MUTTex() { SAFERELEASE(m_pTexture); }
	virtual bool Initialize(PWSTR texname, bool autogen = false)
	{
		if (texname)
		{
			TextureCreator tctr(m_pDevice);
			m_pTexture = tctr.CreateFromFile(texname, /*m_dc, */autogen);
			RETURNONNULL(m_pTexture);
		}
		return true;
	}
	virtual bool Initialize(void* data, int size, bool autogen = false)
	{
		if (data && size)
		{
			TextureCreator tctr(m_pDevice);
			m_pTexture = tctr.CreateFromMemory(data, size, 0,0,0,autogen);
			RETURNONNULL(m_pTexture);
		}
		return true;
	}
	virtual ID3D11ShaderResourceView* SRV() { return m_pTexture; }
private:
	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_dc;
	ID3D11ShaderResourceView *m_pTexture;
};

class MUTTexList
{
public:
	MUTTexList(ID3D11Device* dev/*ID3D11DeviceContext* dc*/) : m_pDevice(dev), m_pImmeDC(nullptr/*dc*/), m_ppTexList(nullptr),
					m_ListBegin(0), m_ListCount(0), m_pTempSRV(nullptr)
	{
		//if (m_pImmeDC)
		//	m_pImmeDC->GetDevice(&m_pDevice);
		if (m_pDevice)
			m_pDevice->GetImmediateContext(&m_pImmeDC);
		m_TexList.clear();
	}
	virtual ~MUTTexList()
	{
		SAFEDELETEARRAY(m_ppTexList);
		std::vector<MUTTex *>::iterator it = m_TexList.begin();
		for (; it != m_TexList.end(); it++)
		{
			SAFEDELETE(*it);
		}
		m_TexList.clear();
		SAFERELEASE(m_pImmeDC);
	}
	virtual void Append(PWSTR texname, bool autogen = false)
	{
		MUTTex *t = new MUTTex(m_pDevice, texname, m_pImmeDC, autogen);
		m_TexList.push_back(t);
	}
	virtual void AppendFromMemory(void* data, int size, bool autogen = false)
	{
		MUTTex *t = new MUTTex(m_pDevice, data, size, m_pImmeDC, autogen);
		m_TexList.push_back(t);
	}

	virtual ID3D11ShaderResourceView* SRV(int index)
	{
		return m_TexList.at(index)->SRV();
	}

	virtual ID3D11ShaderResourceView** SRVAsArray(int index)
	{
		m_pTempSRV = SRV(index);
		return &m_pTempSRV;
	}

	virtual ID3D11ShaderResourceView** SRVArray(int begin = 0, int end = 0)
	{
		int count = end - begin;

		if (begin == 0 && end == 0)
			count = Size();

		if (!m_ppTexList || m_ListBegin != begin || m_ListCount != count)
		{
			SAFEDELETEARRAY(m_ppTexList);
			if (count)
			{
				m_ListCount = count;
				m_ListBegin = begin;
				m_ppTexList = new ID3D11ShaderResourceView*[count];
				for (int i = 0; i < count; i++)
				{
					m_ppTexList[i] = SRV(begin + i);
				}
			}
		}
		return m_ppTexList;
	}

	virtual int Size() { return (int)m_TexList.size(); }

protected:
	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pImmeDC;
	std::vector<MUTTex *> m_TexList;

	ID3D11ShaderResourceView** m_ppTexList;
	int m_ListBegin; // begin of m_ppTexList;
	int m_ListCount; // count of m_ppTexList;
	ID3D11ShaderResourceView *m_pTempSRV; // !!!no need to release, just a reference ptr!!!
};
