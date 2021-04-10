#pragma once

#include "iface/common.hh"

#include "iface/Vector.hh"
#include "iface/IClientEntity.hh"

namespace sw::iface
{
	struct Ray
	{
		Ray(Vector& start, Vector& end) : start(start), delta(end - start)
		{
			is_swept = delta.x || delta.y || delta.z;
		}

		Vector start{};
		float _pad0{};
		Vector delta{};
		char _pad1[40]{};
		bool is_ray{true};
		bool is_swept{};
	};

	struct Trace
	{
		Vector start;
		Vector end;
		char _pad0[20];
		float fraction;
		int contents;
		unsigned short display_flags;
		bool allSolid;
		bool startSolid;
		char _pad1[4];
		struct Surface {
			const char* name;
			short surface_props;
			unsigned short flags;
		} surface;

		int hitgroup;
		char _pad2[4];
		IClientEntity* entity;
		int hitbox;
	};

	struct TraceFilter
	{
		TraceFilter(IClientEntity* skip_me) : skip_me(skip_me)
		{}
		
		virtual bool ShouldHitEntity(IClientEntity* entity)
		{
			return entity != skip_me;
		}
		
		virtual int GetTraceType()
		{
			return 0;
		}

		IClientEntity * skip_me;
	};

	class IEngineTrace
	{
	public:
		LAZY_MEMBER(TraceRay, void, (Ray& ray, unsigned int mask, TraceFilter& filter, Trace& trace), 5, (this, std::cref(ray), mask, std::cref(filter), std::ref(trace)));
	};
}