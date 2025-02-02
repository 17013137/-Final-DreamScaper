#include "stdafx.h"
#include "..\Public\TestBlur.h"
#include "GameInstance.h"

#include "Light.h"

CTestBlur::CTestBlur(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{
}

CTestBlur::CTestBlur(const CTestBlur & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTestBlur::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTestBlur::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	_vector Pos = *((_vector*)pArg);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Pos);

	_vector		vLook = XMVectorSet(0.f, 0.f, -1.f, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp));

	_vector		vRight = XMVector3Cross(m_pTransformCom->Get_State(CTransform::STATE_LOOK), m_pTransformCom->Get_State(CTransform::STATE_UP));
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	m_pTransformCom->Scaled(_float3(0.1f, 0.1f, 0.1f));

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	_float4 LightPos;
	XMStoreFloat4(&LightPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	//혹시 점광원 쓸 때 복붙하라고 안지웠음
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = LightPos;
	LightDesc.fRange = 3.f;
	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.08f, 0.f, 1.f);
	m_pLight = pGameInstance->Add_Lights_Point(m_pDevice, m_pDeviceContext, LightDesc);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CTestBlur::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	Time += TimeDelta;

	if (Time > 3.14f) {
		Rand = rand();
		Time = 0.f;
	}

	_float4 LightPos;
	XMStoreFloat4(&LightPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pLight->Set_LightPos(LightPos);
	//CTransform* pPlayerPos = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Player", L"Com_Transform");
	//_vector Pos = pPlayerPos->Get_State(CTransform::STATE_POSITION) + XMVectorSet(0.f, 1.f, 0.f, 0.f);
	//_float4 fPos;
	//XMStoreFloat4(&fPos, Pos);
	//if (m_pHeadLight != nullptr)
	//	m_pHeadLight->Set_LightPos(fPos);
	//if (pGameInstance->Key_Down(DIK_7)) {
	//	m_pHeadLight->Set_Dead();
	//}

	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), pow(-1, Rand % 2 + 1) * TimeDelta);
	m_pTransformCom->Go_Straight(Rand % 5 * 0.1f*TimeDelta);
	_vector Pos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Pos + XMVectorSet(0.f, 0.02f*cosf(_float(Time)), 0.f, 0.f));


}

void CTestBlur::LateTick(_double TimeDelta)
{
	__super::LateTick(TimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONLIGHTBLUR, this);
}

HRESULT CTestBlur::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);


	if (FAILED(m_pTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTestBlur::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTexBlur"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_TestBlur"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

CTestBlur * CTestBlur::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CTestBlur*	pInstance = new CTestBlur(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CTestBlur"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTestBlur::Clone(void * pArg)
{
	CTestBlur*	pInstance = new CTestBlur(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CTestBlur"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTestBlur::Free()
{
	__super::Free();
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);

}
