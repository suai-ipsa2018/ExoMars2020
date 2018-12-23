#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <Windows.h>

int main(int argc, char* argv[])
{
	const bool compile(true);
	if (compile)
	{
#ifdef _DEBUG
		system(R"cmd("C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild" ..\Network\Network.vcxproj /t:Build /p:DEFINES=NETWORK_PART=1;IntDir=custom_build_int\;OutDir=custom_build\;TargetName=mast_part;Configuration=Debug;Platform=x86)cmd");
		system(R"cmd("C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild" ..\Network\Network.vcxproj /t:Build /p:DEFINES=NETWORK_PART=2;IntDir=custom_build_int\;OutDir=custom_build\;TargetName=rover_part;Configuration=Debug;Platform=x86)cmd");
#else
		system(R"cmd("C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild" ..\Network\Network.vcxproj /t:Build /p:DEFINES=NETWORK_PART=1;IntDir=custom_build_int\;OutDir=custom_build\;TargetName=mast_part;Configuration=Release;Platform=x86)cmd");
		system(R"cmd("C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild" ..\Network\Network.vcxproj /t:Build /p:DEFINES=NETWORK_PART=2;IntDir=custom_build_int\;OutDir=custom_build\;TargetName=rover_part;Configuration=Release;Platform=x86)cmd");
#endif
	}
	std::string dir("../Network/custom_build/");
	std::string mast_path(dir + "mast_part.exe");
	std::string rover_path(dir + "rover_part.exe");
	std::string cla("-v -d -t=\"300 us\"");

	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	CreateProcess(mast_path.c_str(),
		const_cast<char*>((mast_path + ' ' + cla).c_str()), NULL, NULL, FALSE, 0, NULL, "../Network", &si, &pi);

	CreateProcess(rover_path.c_str(),
		const_cast<char*>((rover_path + ' ' + cla).c_str()), NULL, NULL, FALSE, 0, NULL, "../Network", &si, &pi);


	return 0;
}
