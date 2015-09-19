#pragma once

using namespace System;

namespace MyMemory {
	namespace Hooks {
		ref class HookBase;
	}
}

namespace MyMemory {
	namespace Hooks {

		public ref class CallbackBase abstract
		{

			// Members
		private:
			HookBase^ m_hook;

			// Methods
		public:
			CallbackBase(HookBase^ hook);
			~CallbackBase();

			// Methods
		protected:

			// Properties
		public:
			property HookBase^ Hook {
				HookBase^ get() { return m_hook; }
			}

			// Abstract properties
		public:
			virtual property IntPtr EntryPointPointer {
				IntPtr get() = 0;
			}

		};

	}
}