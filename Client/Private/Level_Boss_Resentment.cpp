#include "stdafx.h"
#include "Level_Boss_Resentment.h"

#include "GameInstance.h"
#include "Camera_Default.h"
#include "Client_Level_Manager.h"
#include "Stage_Manager.h"

#include "Equipment_Manager.h"
#include "Monster_Manager.h"
#include "Player_Manager.h"
#include "Map_Manager.h"

#include "Level_Loading.h"
#include "Sound_Manager.h"
#include "Scene_Manager.h"
#include "Scene_FadeInOut.h"
CLevel_Boss_Resentment::CLevel_Boss_Resentment(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: CLevel(pDevice, pDeviceContext)
{
}

HRESULT CLevel_Boss_Resentment::NativeConstruct()
{
	if (FAILED(__super::NativeConstruct())) {
		return E_FAIL;
	}

	if (FAILED(Ready_Lights())) {
		return E_FAIL;
	}

	CClient_Level_Manager::Get_Instance()->Set_Level((CClient_Level_Manager::LEVEL_BOSS_RESENTMENT));

	CClient_Level_Manager::Get_Instance()->Reset_ManagerList();

	if (FAILED(Load_MapObject(LEVEL_BOSS_RESENTMENT, L"../Data/Map/Boss_Resentment/MapData.dat")))
		return E_FAIL;


	CStage_Manager::Get_Instance()->Set_TileIndex(0);


	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player")))) 
		return E_FAIL;
	

	if (FAILED(Ready_Layer_Boss_Resentment(TEXT("Layer_Boss_Resentment")))) {
		return E_FAIL;
	}
	
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround")))) {
		return E_FAIL;
	}
	
	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI")))) {
		return E_FAIL;
	}
	if (FAILED(Ready_Layer_Boss_Effect(TEXT("Layer_Boss_Effect")))) {
		return E_FAIL;
	}
	//??
	CClient_Level_Manager::Get_Instance()->Set_Resentment_Write(false);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CGameObject*	Effect8 = pGameInstance->Add_GameObjToLayer(CClient_Level_Manager::Get_Instance()->Get_Level(), L"Layer_Lava_Back_Particle", L"Prototype_GameObject_ParticleEffect", L"Lava_Back_Particle");
	CTransform* trans8 = (CTransform*)Effect8->Get_Component(L"Com_Transform");
	trans8->Set_State(CTransform::STATE_POSITION, XMVectorSet(50.f, 1.0f, 50.f, 1.f));
	trans8->Scaled(_float3(1.0f, 1.0f, 1.0f));
	RELEASE_INSTANCE(CGameInstance);

	CEvent_Manager::Get_Instance()->Set_SceneNum(SCENE_RESENTINTRO);
	CScene_Manager::Get_Instance()->Reset_Setting();
	CScene_Manager::Get_Instance()->Set_OnAir(true);

	return S_OK;
}

void CLevel_Boss_Resentment::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);	
	CEquipment_Manager::Get_Instance()->ProjectileObject_Release(false);
	
	if (m_isResentmentScene)
	{
		m_isResentmentScene = false;
		CSound_Manager::GetInstance()->Set_VolumeZeroAll();
		CSound_Manager::GetInstance()->StopAll();
	}


	CSound_Manager::GetInstance()->SoundPlay(L"A_Boss_Resentment_TotalTest.ogg", CSound_Manager::BGM1, 0.3f);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (CStage_Manager::Get_Instance()->Get_PlayerDead() == true) {
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_REALITY)))) {
			MSG_BOX(L"보스레벨오브젝트에서 레벨 못열ㅇ므");
			return;
		}
		CStage_Manager::Get_Instance()->Set_PlayerDead(false);
		CPlayer_Manager::Get_Instance()->Set_DeadWakeup_Mode(true);
		CEquipment_Manager::Get_Instance()->TokenItem_Release();
		CEquipment_Manager::Get_Instance()->EquipToken_Release();
		CEquipment_Manager::Get_Instance()->IdeaItem_Release();
		CEndGame_Manager::Get_Instance()->Set_EndGameObjectsZero();
		CMap_Manager::Get_Instance()->Release_Minimap();
	}
	if (CMonster_Manager::Get_Instance()->Get_BossDead() == true)
		m_bCheck = true;

	if (m_bCheck == true)
		++m_iTIck;

	if (m_iTIck >= 120)
		m_bCheck2 = true;

	if (m_bCheck2 == true)
	{
		++iFadeTime;

		if (iFadeTime == 1)
		{
			_uint i = 1;
			pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, TEXT("Layer_Fade"), TEXT("Prototype_GameObject_FadeInOut"), &i);
		}

		if (iFadeTime == 100)
		{
			if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pDeviceContext, LEVEL_ENDING)))) {
				MSG_BOX(L"보스레벨오브젝트에서 레벨 못열ㅇ므");
				return;
			}
			CStage_Manager::Get_Instance()->Set_PlayerDead(false);
			CPlayer_Manager::Get_Instance()->Set_DeadWakeup_Mode(true);
			CEvent_Manager::Get_Instance()->Set_Rebirth(true);
			CClient_Level_Manager::Get_Instance()->Set_WakeUp(true);
		}
	}
	CMonster_Manager::Get_Instance()->Monster_Dead(CStage_Manager::Get_Instance()->Get_TileIndex());
	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CLevel_Boss_Resentment::Render()
{
	if (FAILED(__super::Render())) {
		MSG_BOX(L"Failed To CLevel_Boss_Resentment : Render, __super::Render");
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CLevel_Boss_Resentment::Ready_Lights()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Clear_Light();
	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -2.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 0.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.3f, 0.f, 0.f, 1.f);
	//0.2f, 0.2f, 0.5f, 1.f
	//0.3 0.3 0.7 도 ㄱㅊ음 
	LightDesc.vSpecular = _float4(0.2f, 0.2f, 0.2f, 1.f);
	if (FAILED(pGameInstance->Add_Lights(m_pDevice, m_pDeviceContext, LightDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Boss_Resentment::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 10.f, -15.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 3000.0f;
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Camera_Default"), &CameraDesc))) {
		MSG_BOX(L"Failed To CLevel_Boss_Resentment : Ready_Layer_Camera,pGameInstance->Add_GameObjectToLayer, Camera_Default");
		return E_FAIL;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Boss_Resentment::Ready_Layer_Boss_Resentment(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_RESENTMENT, pLayerTag, TEXT("Prototype_GameObject_Boss_Resentment"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Boss_Resentment::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CTransform*	pTrans = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Player", L"Com_Transform");
	pTrans->Set_State(CTransform::STATE_POSITION, XMVectorSet(50.f, 0.f, 50.f, 1.f));

	CGameObject*	pPlayer = pGameInstance->Get_GameObject(LEVEL_STATIC, TEXT("Layer_Player"), 0);//느낌표
	dynamic_cast<CPlayer*>(pPlayer)->Set_CurrentIndex(pTrans->Compute_CurrentIndex(XMVectorSet(35.f, 0.f, 50.f, 1.f)));

	CTransform* pChaserTrans = (CTransform*)pGameInstance->Get_Component(LEVEL_STATIC, L"Layer_Chaser", L"Com_Transform");
	pChaserTrans->Set_State(CTransform::STATE_POSITION, XMVectorSet(50.f, 0.f, 50.f, 1.f));

	CEvent_Manager::Get_Instance()->Set_CutScene(true);
	CEvent_Manager::Get_Instance()->Set_SceneNum(SCENE_RESENTINTRO);
	CScene_FadeInOut::FADE_DESC FadeDesc;
	FadeDesc.e_State = 0;
	FadeDesc.Speed = 0.3f;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_RESENTMENT, TEXT("FadeInOut"), TEXT("Prototype_GameObject_Scene_FadeInOut"), &FadeDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Boss_Resentment::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_RESENTMENT, pLayerTag, TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;
	/*if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_RESENTMENT, pLayerTag, TEXT("Prototype_GameObject_Boss_Regret_ClockHand"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_RESENTMENT, pLayerTag, TEXT("Prototype_GameObject_Boss_Regret_ClockMap"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_REGRET, pLayerTag, TEXT("Prototype_GameObject_Clock_Red"))))
		return E_FAIL;*/

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Boss_Resentment::Ready_Layer_Boss_Effect(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//_bool Option = true;
	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_REGRET, pLayerTag, TEXT("Prototype_GameObject_Regret_HandTrail1"), &Option))) {
	//	MSG_BOX(L"Failed To CLevel_City : Ready_Layer_Effect,pGameInstance->Add_GameObjectToLayer, Rect_Effect");
	//	return E_FAIL;
	//}

	//Option = false;
	//if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_BOSS_REGRET, pLayerTag, TEXT("Prototype_GameObject_Regret_HandTrail1"), &Option))) {
	//	MSG_BOX(L"Failed To CLevel_City : Ready_Layer_Effect,pGameInstance->Add_GameObjectToLayer, Rect_Effect");
	//	return E_FAIL;
	//}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Boss_Resentment::Ready_Layer_Effect(const _tchar * pLayerTag)
{

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_bool Option = true;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Trail_Effect"), &Option))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Effect,pGameInstance->Add_GameObjectToLayer, Rect_Effect");
		return E_FAIL;
	}

	Option = false;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_Trail_Effect"), &Option))) {
		MSG_BOX(L"Failed To CLevel_GamePlay : Ready_Layer_Effect,pGameInstance->Add_GameObjectToLayer, Rect_Effect");
		return E_FAIL;
	}
	Safe_Release(pGameInstance);

	return S_OK;
	
}

HRESULT CLevel_Boss_Resentment::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_BossHealthBar")))) {
		MSG_BOX(L"Failed To CLevel_Reality : Ready_Layer_UI,pGameInstance->Add_GameObjectToLayer, UI");
		return E_FAIL;
	}

	_uint i = 2;
	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_BossHealth_Name"), &i))) {
		MSG_BOX(L"Failed To CLevel_Reality : Ready_Layer_UI,pGameInstance->Add_GameObjectToLayer, UI");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_BossHealthBar_BG")))) {
		MSG_BOX(L"Failed To CLevel_Reality : Ready_Layer_UI,pGameInstance->Add_GameObjectToLayer, UI");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_GameObjectToLayer(LEVEL_STATIC, pLayerTag, TEXT("Prototype_GameObject_BossTips")))) {
		MSG_BOX(L"Failed To CLevel_Reality : Ready_Layer_UI,pGameInstance->Add_GameObjectToLayer, UI");
		return E_FAIL;
	}
	Safe_Release(pGameInstance);


	return S_OK;
}

CLevel_Boss_Resentment * CLevel_Boss_Resentment::Create(ID3D11Device* pDeviceOut, ID3D11DeviceContext* pDeviceContextOut)
{
	CLevel_Boss_Resentment*	pInstance = new CLevel_Boss_Resentment(pDeviceOut, pDeviceContextOut);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX(TEXT("Failed to Created CLevel_Boss_Resentment"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Boss_Resentment::Free()
{
	__super::Free();
}
