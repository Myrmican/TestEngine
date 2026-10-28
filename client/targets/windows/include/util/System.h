#pragma once

namespace Client {
	namespace System {
		bool IsVirtualEnvironment() {
			int CPUInfo[4] = { 0 };
			__cpuid(CPUInfo, 1);
			return false; //(CPUInfo[2] & (1 << 31)) != 0;
		}
	}
}