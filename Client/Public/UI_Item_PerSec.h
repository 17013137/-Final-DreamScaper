#pragma once
#include "Client_Defines.h"
#include "UI.h"
#include "UI_Item.h"
#include "Text_Number.h"
BEGIN(Client)

class CUI_Item_PerSec final : public CUI
{
private:
	explicit CUI_Item_PerSec(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	explicit CUI_Item_PerSec(const CUI_Item_PerSec& rhs);
	virtual ~CUI_Item_PerSec() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual void Tick(_double TimeDelta) override;
	virtual void LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT SetUp_Components();

private:
	_bool m_bButton = false;
	CUI_Item::ITEMUIINFO m_tItemUIInfo;
	_float3 m_Scale;
	_uint iDamage = 0;
public:
	static CUI_Item_PerSec* Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END