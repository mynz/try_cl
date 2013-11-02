solution "MySolution"
	configurations { "Debug", "Release" }

	project "try_cl"
		kind "ConsoleApp"
		language "C++"
		-- files { "**.cpp" }
		files { "try_cl.cpp" }
		linkoptions { "-framework OpenCL" }
