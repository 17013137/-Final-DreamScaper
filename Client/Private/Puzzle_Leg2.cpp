#include "stdafx.h"
#include "..\Public\Puzzle_Leg2.h"
#include "GameInstance.h"
#include "Puzzle_Leg1.h"
#include "Puzzle_Leg3.h"
#include "Puzzle_Leg4.h"
#include "Time_Manager.h"
#include "Puzzle_Water.h"
#include "Client_Level_Manager.h"
#include "Sound_Manager.h"

CPuzzle_Leg2::CPuzzle_Leg2(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
	: CGameObject(pDeviceOut, pDeviceContextOut)
{

}

CPuzzle_Leg2::CPuzzle_Leg2(const CPuzzle_Leg2 & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPuzzle_Leg2::NativeConstruct_Prototype()
{

	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPuzzle_Leg2::NativeConstruct(void * pArg)
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.SpeedPerSec = 5.0f;
	TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);
	
	CPuzzle_Manager::LEGINFO Desc;
	ZeroMemory(&Desc, sizeof(CPuzzle_Manager::LEGINFO));

	if (nullptr == pArg)
		return E_FAIL;

	Desc = *(CPuzzle_Manager::LEGINFO*)pArg;

	if (FAILED(__super::NativeConstruct(pArg, &TransformDesc)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;	

	m_vPos = Desc.vPos;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPos);

	m_bSwitch = false;
	m_iDegree = 0;
	m_bPipe = false;
	m_iStartDir = Desc.iStartDir;	
	m_bStartRing = Desc.bStartRing;
	m_iIndex = Desc.iIndex;
	m_Level = Desc.Level;

	switch (m_iStartDir)
	{
	case 0:
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f));
		break;
	case 1:
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
		break;
	case 2:
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
		break;
	case 3:
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(270.f));
		break;
	default:
		break;
	}

	switch (m_Level)
	{
	case 1:
		m_iPipeColor = 1;
		break;
	
	default:
		break;
	}

		m_iDir = 3;
	/*CStage_Manager::Get_Instance()->Add_Portal(this);*/
	return S_OK;
}

void CPuzzle_Leg2::Tick(_double TimeDelta)
{
	_double TimeRatio = CTime_Manager::Get_Instance()->Get_EctRatio();
	_double mTimeDelta = TimeDelta * TimeRatio;
	__super::Tick(mTimeDelta);
	if (false == CPuzzle_Manager::Get_Instance()->Get_AllTrue() && true == CPuzzle_Manager::Get_Instance()->Get_bPlayerPos())
	{
		if (true == m_bSwitch)
		{
			switch (m_iDir)
			{
			case 0:
				++m_iDegree;
				m_pTransformCom->Turn_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), 3.f);
				if (30 == m_iDegree)
				{
					m_bSwitch = false;
					m_iDegree = 0;
				}
				break;
			case 1:
				++m_iDegree;
				m_pTransformCom->Turn_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), 3.f);
				if (30 == m_iDegree)
				{
					m_bSwitch = false;
					m_iDegree = 0;
				}
				break;
			case 2:
				++m_iDegree;
				m_pTransformCom->Turn_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), 3.f);
				if (30 == m_iDegree)
				{
					m_bSwitch = false;
					m_iDegree = 0;
				}
				break;
			case 3:
				++m_iDegree;
				m_pTransformCom->Turn_Axis(XMVectorSet(0.f, 1.f, 0.f, 0.f), 3.f);
				if (30 == m_iDegree)
				{
					m_bSwitch = false;
					m_iDegree = 0;
				}
				break;
			default:
				break;
			}
		}


		/*if (28 == m_iDegree)
		{*/
		switch (m_iIndex)
		{
		case 1:
			CPuzzle_Manager::Get_Instance()->Check_Pipe01(m_Level);
			break;
		case 4:
			CPuzzle_Manager::Get_Instance()->Check_Pipe04(m_Level);
			break;
		case 7:
			CPuzzle_Manager::Get_Instance()->Check_Pipe07(m_Level);
			break;
		case 8:
			CPuzzle_Manager::Get_Instance()->Check_Pipe08(m_Level);
			break;
		case 11:
			CPuzzle_Manager::Get_Instance()->Check_Pipe11(m_Level);
			break;
		case 12:
			CPuzzle_Manager::Get_Instance()->Check_Pipe12(m_Level);
			break;
		case 21:
			CPuzzle_Manager::Get_Instance()->Check_Pipe21(m_Level);
			break;
		case 23:
			CPuzzle_Manager::Get_Instance()->Check_Pipe23(m_Level);
			break;
		case 25:
			CPuzzle_Manager::Get_Instance()->Check_Pipe25(m_Level);
			break;

		default:
			break;
		}

		//}




		m_pAABBCom[0]->Update(XMMatrixTranslation(0.f, 0.f, -2.f)*m_pTransformCom->Get_WorldMatrix());
		m_pAABBCom[1]->Update(XMMatrixTranslation(2.f, 0.f, 0.f)*m_pTransformCom->Get_WorldMatrix());
		m_pSphereCom->Update(m_pTransformCom->Get_WorldMatrix());
	}
}

void CPuzzle_Leg2::LateTick(_double TimeDelta)
{
	_double TimeRatio = CTime_Manager::Get_Instance()->Get_EctRatio();
	_double mTimeDelta = TimeDelta * TimeRatio;
	__super::LateTick(mTimeDelta);
	if (false == CPuzzle_Manager::Get_Instance()->Get_AllTrue() && true == CPuzzle_Manager::Get_Instance()->Get_bPlayerPos())
	{
		CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
		CCollider*		pTargetSphere = (CCollider*)pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_SPHERE"));
		if (nullptr == pTargetSphere)
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
			
		if (m_pSphereCom->Collsion(pTargetSphere))
		{
			if (/*pGameInstance->Key_Down(DIK_F)*/(GetAsyncKeyState('F') & 0X8000) && false == m_bSwitch)
			{
				TCHAR   m_szFPS[128];
				_uint Rand = rand() % 4;

				swprintf_s(m_szFPS, L"Puzzle_Pipe(%d).ogg", Rand);
				CSound_Manager::GetInstance()->StopSound(CSound_Manager::ENV_EFFECT);
				CSound_Manager::GetInstance()->SoundPlay(m_szFPS, CSound_Manager::ENV_EFFECT, SOUND_MAX);


				CPuzzle_Water::PUZZLEWATERINFO	Desc;
				ZeroMemory(&Desc, sizeof(CPuzzle_Water::PUZZLEWATERINFO));
				Desc.vPos = m_vPos;
				if (FAILED(pGameInstance->Add_GameObjectToLayer(CClient_Level_Manager::Get_Instance()->Get_Level(), L"Layer_Effect", TEXT("Prototype_GameObject_Puzzle_Water"), &Desc))) {
					MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Camera,pGameInstance->Add_GameObjectToLayer, Puzzle_Water");
					return;
				}
				m_bSwitch = true;
				++m_iDir;
				if (3 < m_iDir)
					m_iDir = 0;
			}
		}

		CPuzzle_Manager::Get_Instance()->Check_True_Pipe();

		switch (m_iIndex)
		{
		case 1:
			m_bPipe = CPuzzle_Manager::Get_Instance()->Get_TruePipe(1);
			break;
		case 4:
			m_bPipe = CPuzzle_Manager::Get_Instance()->Get_TruePipe(4);
			break;
		case 7:
			m_bPipe = CPuzzle_Manager::Get_Instance()->Get_TruePipe(7);
			break;
		case 8:
			m_bPipe = CPuzzle_Manager::Get_Instance()->Get_TruePipe(8);
			break;
		case 11:
			m_bPipe = CPuzzle_Manager::Get_Instance()->Get_TruePipe(11);
			break;
		case 12:
			m_bPipe = CPuzzle_Manager::Get_Instance()->Get_TruePipe(12);
			break;
		case 21:
			m_bPipe = CPuzzle_Manager::Get_Instance()->Get_TruePipe(21);
			break;
		case 23:
			m_bPipe = CPuzzle_Manager::Get_Instance()->Get_TruePipe(23);
			break;
		case 25:
			m_bPipe = CPuzzle_Manager::Get_Instance()->Get_TruePipe(25);
			break;

		default:
			break;
		}

		RELEASE_INSTANCE(CGameInstance);
	}

		if (nullptr != m_pRendererCom)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::GROUP_NONBLEND, this);

#ifdef _DEBUG
			for (_uint i = 0; i < 2; ++i)
				m_pRendererCom->Add_DebugComponent(m_pAABBCom[i]);
			m_pRendererCom->Add_DebugComponent(m_pSphereCom);
#endif // _DEBUG
		}

}

HRESULT CPuzzle_Leg2::Render()
{
	if (true == CPuzzle_Manager::Get_Instance()->Get_bPlayerPos())
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

			if (FAILED(m_pModelCom->Render(m_pShaderCom, "g_BoneMatrices", i, 6)))
				return E_FAIL;
		}
	}
		return S_OK;

}

HRESULT CPuzzle_Leg2::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNonAnim"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(TEXT("Com_Model"), LEVEL_STATIC, TEXT("Prototype_Component_Model_Puzzle_Leg2"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

	/* For.Com_AABB */
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vSize = _float3(0.7f, 0.7f, 0.7f);

	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB0"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom[0], &ColliderDesc)))
		return E_FAIL;

	if (FAILED(__super::SetUp_Components(TEXT("Com_AABB1"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), (CComponent**)&m_pAABBCom[1], &ColliderDesc)))
		return E_FAIL;

	/* For.Com_Sphere */
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	ColliderDesc.fRadius = 0.5f;

	if (FAILED(__super::SetUp_Components(TEXT("Com_Sphere"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSphereCom, &ColliderDesc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CPuzzle_Leg2::SetUp_ConstantTable()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(m_pTransformCom->Bind_WorldMatrixOnShader(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pGameInstance->Get_TransformFloat4x4_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;	

	if (FAILED(m_pShaderCom->Set_RawValue("g_PipeColor", &m_iPipeColor, sizeof(_uint))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_bPipe", &m_bPipe, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_bStartRing", &m_bStartRing, sizeof(_bool))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CPuzzle_Leg2 * CPuzzle_Leg2::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CPuzzle_Leg2*	pInstance = new CPuzzle_Leg2(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created CPuzzle_Leg2"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPuzzle_Leg2::Clone(void * pArg)
{
	CPuzzle_Leg2*	pInstance = new CPuzzle_Leg2(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created CPuzzle_Leg2"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPuzzle_Leg2::Free()
{
	__super::Free();	
	for(_uint i = 0; i<2;++i)
		Safe_Release(m_pAABBCom[i]);

	Safe_Release(m_pSphereCom);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
