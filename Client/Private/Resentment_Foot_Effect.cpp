#include "stdafx.h"
#include "..\Public\Resentment_Foot_Effect.h"
#include "GameInstance.h"
#include "Client_Level_Manager.h"

CResentment_Foot_Effect::CResentment_Foot_Effect(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CResentment_Foot_Effect::CResentment_Foot_Effect(const CResentment_Foot_Effect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CResentment_Foot_Effect::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CResentment_Foot_Effect::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	

	if (nullptr != pArg)
	{
		STORNBOOM BoomDesc = *(STORNBOOM*)pArg;
		m_pTargetTransform = BoomDesc.TargetTransform;
		m_iOption = BoomDesc.Option;
	}

	if (0 == m_iOption)
	{
		m_fPower = 0.15f;

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_pTargetTransform->Get_State(CTransform::STATE_RIGHT));
		m_pTransformCom->Set_State(CTransform::STATE_UP, m_pTargetTransform->Get_State(CTransform::STATE_UP));
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pTargetTransform->Get_State(CTransform::STATE_LOOK));


		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_RIGHT) * 3.0f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTargetTransform->Get_State(CTransform::STATE_POSITION) + m_pTargetTransform->Get_State(CTransform::STATE_LOOK) * 3.0f + m_pTargetTransform->Get_State(CTransform::STATE_RIGHT) * 2.0f + XMVectorSet(0.f,0.5f,0.f,0.f));

		m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f),XMConvertToRadians(_float(rand() % 360)));
		m_pTransformCom->Rotate_Radian(m_pTransformCom->Get_State(CTransform::STATE_LOOK), XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(_float(rand() % 360)));


		//

	}

	if (1 == m_iOption)
	{
		m_fPower = 0.15f;

		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_pTargetTransform->Get_State(CTransform::STATE_RIGHT));
		m_pTransformCom->Set_State(CTransform::STATE_UP, m_pTargetTransform->Get_State(CTransform::STATE_UP));
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pTargetTransform->Get_State(CTransform::STATE_LOOK));


		//m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_State(CTransform::STATE_POSITION) + m_pTransformCom->Get_State(CTransform::STATE_RIGHT) * 3.0f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTargetTransform->Get_State(CTransform::STATE_POSITION) + m_pTargetTransform->Get_State(CTransform::STATE_LOOK) * 3.0f + m_pTargetTransform->Get_State(CTransform::STATE_RIGHT) * -2.0f + XMVectorSet(0.f, 0.5f, 0.f, 0.f));


		m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(_float(rand() % 360)));
		m_pTransformCom->Rotate_Radian(m_pTransformCom->Get_State(CTransform::STATE_LOOK), XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(_float(rand() % 360)));
	
	}




	//float x,y,z;
	/*x = (float)(rand() % 30) / 10 + 0.3;
	y = (float)(rand() % 15) / 10 + 0.2;
	z = (float)(rand() % 20) / 10 + 1.5;*/


	m_pTransformCom->Scaled(_float3(_float(rand() % 5 + 1), _float(rand() % 5 + 1), _float( rand() % 5 + 1)));
	

	return S_OK;
}

void CResentment_Foot_Effect::Tick(_double TimeDelta)
{
	//_double TimeRatio = CTime_Manager::Get_Instance()->Get_PlayerRatio();
	//_double mTimeDelta = TimeDelta * TimeRatio;
	__super::Tick(TimeDelta);


	_float4		fPos;
	_float4		fLook;
	
	_vector		vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector		vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	XMStoreFloat4(&fPos, vPos);
//	XMVectorSetW(vPos, 0.f);
	XMStoreFloat4(&fLook, vLook);

	fPos.x += fLook.x * 0.04f;
	fPos.z += fLook.z * 0.04f;
	fPos.y += m_fPower - ((9.8f * (_float)m_ThorwTime * (_float)m_ThorwTime));

	if(m_ThorwTime < 0.2f) // 제일 위에올라가면 떨어질ㄸ ㅐ너무빨라서 제어함.
		m_ThorwTime += 0.5 * TimeDelta;
	else
		m_ThorwTime += 0.07 * TimeDelta;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&fPos));
	

	//m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, -1.f, 0.f), mTimeDelta);

	if (fPos.y < -2.5)
		m_eState = STATE_DEAD;

	m_pModelCom->Update(TimeDelta);
}

void CResentment_Foot_Effect::LateTick(_double TimeDelta)
{
	//_double TimeRatio = CTime_Manager::Get_Instance()->Get_PlayerRatio();
	//_double mTimeDelta = TimeDelta * TimeRatio;
	__super::LateTick(TimeDelta);

		if (nullptr != m_pRendererCom)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONLIGHT, this);

		}
}

HRESULT CResentment_Foot_Effect::Render()
{
	if (nullptr == m_pShaderCom || 
		nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;


	_uint		iNumMeshContainers = m_pModelCom->Get_NumMeshContainer();

	for (_uint i = 0; i < iNumMeshContainers; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material_OnShader(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;
 		if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i,1)))
			return E_FAIL;
	}	


	return S_OK;
}

HRESULT CResentment_Foot_Effect::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnimEms"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Ch_Recent_Foot_Effect"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	/* For.Com_Texture */


	return S_OK;
}

HRESULT CResentment_Foot_Effect::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;	

	//if (FAILED(m_pObjectTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_ObjectTexture", 0)))
	//	return E_FAIL;

	//if (FAILED(m_pObjectTextureCom->SetUp_ShaderResourceView(m_pShaderCom, "g_EffectTexture", 0)))
	//	return E_FAIL;

	//if (FAILED(m_pShaderCom->Set_RawValue("g_RenderTick", &m_fRenderTick, sizeof(_float))))
	//	return E_FAIL;
	//

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CResentment_Foot_Effect * CResentment_Foot_Effect::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CResentment_Foot_Effect*	pInstance = new CResentment_Foot_Effect(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CResentment_Foot_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CResentment_Foot_Effect::Clone(void * pArg)
{
	CResentment_Foot_Effect*	pInstance = new CResentment_Foot_Effect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CResentment_Foot_Effect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CResentment_Foot_Effect::Free()
{
	__super::Free();	
	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

	Safe_Release(m_pAABBCom);
	
}
