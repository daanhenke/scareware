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
	console::WriteFormat("cham mats: %x, %x\n", m_cham_material, m_cham_hidden_material);
}

bool sw::hacks::chams::Render(void* ctx, void* state, iface::ModelRenderInfo& info, iface::matrix3x4* customBoneToWorld)
{
	auto entity = interfaces::IClientEntityList->GetClientEntity(info.entityIndex);

	if (entity && !entity->IsDormant() && entity->IsPlayer())
	{
		auto localPlayer = interfaces::GetLocalPlayer();
		if (!localPlayer) return false;

		m_cham_material->AlphaModulate(1.f);
		m_cham_hidden_material->AlphaModulate(1.f);

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
