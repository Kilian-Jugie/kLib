#include <klib.h>

#include <cassert>

#define _KTEST_VERSION 1,2,3

bool testPreprocessor() {
	static_assert(KVER_MAJ(_KTEST_VERSION) == 1, "Error KVER_MAJ");
	static_assert(KVER_MIN(_KTEST_VERSION) == 2, "Error KVER_MIN");
	static_assert(KVER_EDIT(_KTEST_VERSION) == 3, "Error KVER_EDIT");
	static_assert(KVER_NUM(_KTEST_VERSION) == 123, "Error KVER_NUM");
	static_assert(KU_NOT(1) == 0&&KU_NOT(0)==1, "Error KU_NOT");
	KU_IF_ELSE(1)()(static_assert(false, "Error on KU_IF_ELSE"));
	static_assert(KU_FIRST(1, 2, 3) == 1,"Error on KU_FIRST");
	static_assert(KU_SECOND(1, 2, 3) == 2, "Error on KU_SECOND");
	static_assert(KU_THIRD(1, 2, 3) == 3, "Error on KU_THIRD");
	return true;
}


#include <combaseapi.h>
void wslExample() {
	kWinWslAPI wslAPI;
	if (wslAPI.isDistribRegistered(L"debian")) {
		ULONG version, uid, varsCount;
		WSL_DISTRIBUTION_FLAGS flags;
		PSTR* vars = (PSTR*)malloc(sizeof(PSTR) * 256);
		wslAPI.wslGetDistributionConfiguration(L"debian", &version, &uid, &flags, &vars, &varsCount);
		std::cout << "Debian: version " << version << " uid " << uid << " Variables : ";
		for (unsigned int i(0u); i < varsCount; ++i) {
			std::cout << vars[i] << ";";
		}

		std::cout << " (" << varsCount << " variables)\n";

		CoTaskMemFree(vars);
		DWORD exit;
		wslAPI.wslLaunchInteractive(L"debian", L"echo This distribution is installed and work !\n", FALSE, &exit);
		std::cout << "Exited with code : " << exit << "\n";

	}
}