#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CPrimalsScream_Distotion_EffectRect final : public CGameObject
{
private:
	explicit CPrimalsScream_Distotion_EffectRect(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CPrimalsScream_Distotion_EffectRect(const CPrimalsScream_Distotion_EffectRect& rhs);
	virtual ~CPrimalsScream_Distotion_EffectRect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture*			m_pTextureCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;

private:
	_float				m_Frame = 1.0f;
	_float				m_fStart = 0.f;
	_float				m_fEnd = 0.f;


	_vector vLook;
	_vector vPos;
	_vector vRight;
	_vector vUp;

public:
	HRESULT SetUp_Components();

public:
	static CPrimalsScream_Distotion_EffectRect* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END