#include <iostream>
#include <klib.h>
#include <tests.h>


#define STRIFY(x) #x

#ifndef RELEASE


int main(int argc, char** argv) {

	/*kWinProc wp{ (kLPSTR)"cmd.exe /C wsl echo `ls -l`"};
	wp.setRedirectHandle(true);
	wp.start(true);
	wp.finish();

	std::cout << "ok !\n";*/

	//system("pause");

	//dmanager.createVariable(L"testVar", L"lol");

	/*
	kArgs args;
	args.addParameter("cat").setHelp("print file content").setGetInput(true).setHint("Files").getParent()
		.setGetInput(true).setHelpTitle("kLib args example").setMainHint("files")
		.addParameter("copy").setHelp("copy file content").setGetInput(true).setHint("Files");
	if (!args.parseArgv(argc, argv))
		return 1;

	kPassedParameter& param = args.getPassedParameter("cat");

	if (param == kPassedParameter::null) {
		std::cerr << "Error !!!!!\n";
	}



	std::cout << ">" << param.getContent() << "<\n";

	std::cout << "" << KVER_NUM(TESTVER) << "\n";*/



	//kSimpleContainer<int> kk(k, 3);

	//std::wcout << kk <<L"\n";

	system("pause");
	return 0;
	/*kString str("lolxddddd");
	kString::iterator it(str.begin()), end(str.end());
	for (; it != end; ++it) {
		if ((*it) == L'd') {
			std::wcout << str.itToIndex(it) << ":"_k << str[str.itToIndex(it)] << "\n";
		}
	
	}
	system("PAUSE");*/
	return 0;
}
#endif // RELEASE