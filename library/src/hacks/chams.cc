#include "hacks/chams.hh"
#include "console.hh"
#include "iface/KeyValues.hh"
#include "hooks.hh"

sw::iface::IMaterial* m_cham_material;
sw::iface::IMaterial* m_cham_hidden_material;

void sw::hacks::chams::Initialize()
{
	m_cham_material = interfaces::IMaterialSystem->CreateMaterial("normal", iface::KeyValues::FromString("VertexLitGeneric", nullptr));
	m_cham_hidden_material = interfaces::IMaterialSystem->CreateMaterial("normal2", iface::KeyValues::FromString("VertexLitGeneric", nullptr));
}

bool sw::hacks::chams::Render(void* ctx, void* state, iface::ModelRenderInfo& info, iface::matrix3x4* customBoneToWorld)
{
	auto entity = interfaces::IClientEntityList->GetClientEntity(info.entityIndex);
	auto isWeapon = false;

	auto modelName = std::string(info.model->name);
	if (modelName.starts_with("models/weapons/"))
	{
		//console::WriteFormat("%s\n", info.model->name);
		isWeapon = true;
	}

	if (entity && !entity->IsDormant())
	{
		auto localPlayer = interfaces::GetLocalPlayer();
		if (!localPlayer) return false;

		m_cham_material->AlphaModulate(1.f);
		m_cham_hidden_material->AlphaModulate(1.f);

		if (entity->IsPlayer())
		{
			if (localPlayer->iTeamNum() == entity->iTeamNum())
			{
				m_cham_material->ColorModulate(.0f, .0f, 1.f);
				m_cham_hidden_material->ColorModulate(.0f, .5f, 1.f);
			}
			else
			{
				m_cham_material->ColorModulate(1.f, .0f, 0.f);
				m_cham_hidden_material->ColorModulate(1.f, .5f, 0.f);
			}
		}
		else if (entity->GetClientClass()->classId == iface::ClassId::PlantedC4)
		{
			m_cham_material->ColorModulate(1.f, 1.f, 0.f);
			m_cham_hidden_material->ColorModulate(.7f, .7f, 0.f);
		}
		else if (isWeapon)
		{
			/*m_cham_material->ColorModulate(0.f, 1.f, 0.f);
			m_cham_hidden_material->ColorModulate(0.f, .7f, 0.f);*/
			return false;
		}
		else
		{
			return false;
		}

		m_cham_hidden_material->SetMaterialVarFlag(iface::MaterialVarFlag::IGNOREZ, true);
		interfaces::IStudioRender->ForcedMaterialOverride(m_cham_hidden_material);
		hooks::oDrawModelExecute(interfaces::IVModelRender, ctx, state, info, customBoneToWorld);
		interfaces::IStudioRender->ForcedMaterialOverride(nullptr);
		m_cham_material->SetMaterialVarFlag(iface::MaterialVarFlag::IGNOREZ, false);
		interfaces::IStudioRender->ForcedMaterialOverride(m_cham_material);
		hooks::oDrawModelExecute(interfaces::IVModelRender, ctx, state, info, customBoneToWorld);
		interfaces::IStudioRender->ForcedMaterialOverride(nullptr);

		return true;
	}

	return false;
}
