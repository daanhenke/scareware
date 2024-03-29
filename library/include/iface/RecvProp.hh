#pragma once

#include <type_traits>
#include <cstdint>
#include "iface/Vector.hh"

namespace sw::iface
{
	struct RecvProp;

	struct RecvProxyData
	{
		const RecvProp* prop;
		union {
			float _float;
			long _int;
			char* _string;
			void* _data;
			Vector vector;
			int64_t int64;
		};
	};

	using RecvProxy = std::add_pointer_t<void __cdecl(RecvProxyData&, void*, void*)>;

	struct RecvProp {
		char* name;
		int type;
		int flags;
		int stringBufferSize;
		int insideArray;
		const void* extraData;
		RecvProp* arrayProp;
		void* arrayLengthProxy;
		RecvProxy proxy;
		void* dataTableProxy;
		struct RecvTable* dataTable;
		int offset;
		int elementStride;
		int elementCount;
		const char* parentArrayPropName;
	};

	struct RecvTable {
		RecvProp* props;
		int propCount;
		void* decoder;
		char* netTableName;
		bool isInitialized;
		bool isInMainList;
	};
}