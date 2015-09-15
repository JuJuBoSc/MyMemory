#pragma once

using namespace System;
using namespace System::Linq;
using namespace System::Runtime::InteropServices;
using namespace System::Reflection;
using namespace System::Reflection::Emit;

namespace MyMemory {
	namespace Utils {

		generic<typename T> 
		public ref class MarshalCache
		{

		private:
			static MarshalCache()
			{
				Type^ t = T::typeid;
				TypeCode = t->GetTypeCode(t);
				if (t == bool::typeid)
				{
					Size = 1;
					RealType = t;
				}
				else if (t->IsEnum)
				{
					Type^ underlying = t->GetEnumUnderlyingType();
					Size = Marshal::SizeOf(underlying);
					RealType = underlying;
					TypeCode = RealType->GetTypeCode(RealType);
				}
				else
				{
					RealType = t;
					Size = Marshal::SizeOf(RealType);
				}
				IsIntPtr = RealType == IntPtr::typeid;
				SizeU = (unsigned int)Size;
				TypeRequireMarshal = IsTypeRequireMarshal(RealType);
			}
			static bool IsTypeRequireMarshal(Type^ t);

		public:
			static Type^ RealType;
			static int Size;
			static unsigned int SizeU;
			static TypeCode TypeCode;
			static bool TypeRequireMarshal;
			static bool IsIntPtr;

		};

	}
}