#pragma once

using namespace System;
using namespace System::Linq;
using namespace System::Runtime::InteropServices;
using namespace System::Reflection;
using namespace System::Reflection::Emit;

namespace MyMemory {
	namespace Utils {

		public ref class Randoms
		{

		private:
			static Random s_randomString;

		public:
			static String^ GetRandomString(int minLength, int maxLength);
		};

	}
}