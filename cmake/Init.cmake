macro(HxInit)
	#CMAKE构建时间戳
	string(TIMESTAMP HX_YEAR "%Y")
	string(TIMESTAMP HX_TIMESTAMP "%Y-%m-%d+%H:%M:%S")
	message(STATUS "HX_TIMESTAMP: ${HX_TIMESTAMP}")

	#如果未定义HX_SYSTEM则定义
	if(NOT DEFINED HX_SYSTEM)
		string(TOLOWER ${CMAKE_SYSTEM_NAME} HX_SYSTEM)
	endif()
	message(STATUS "HX_SYSTEM: ${HX_SYSTEM}")
	message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")

	#如果未定义HX_ARCHITECTURE则定义
	if(NOT DEFINED HX_ARCHITECTURE)
		string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} HX_ARCHITECTURE)
		if (HX_ARCHITECTURE MATCHES "amd64")
			if(CMAKE_SIZEOF_VOID_P EQUAL 8)
				set(HX_ARCHITECTURE "x64")
			else()
				set(HX_ARCHITECTURE "x86")
			endif()
		elseif(HX_ARCHITECTURE MATCHES "x86_64")
			set(HX_ARCHITECTURE "x64")
		elseif(HX_ARCHITECTURE MATCHES "i686")
			set(HX_ARCHITECTURE "x64")
		endif()
	endif()
	message(STATUS "HX_ARCHITECTURE: ${HX_ARCHITECTURE}")
	message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")

	#如果未定义CMAKE_BUILD_TYPE则定义
	if(CMAKE_BUILD_TYPE STREQUAL "")
		set(CMAKE_BUILD_TYPE "Release")
	endif()
	message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

	#隐藏符号
	set(CMAKE_C_VISIBILITY_PRESET hidden)
	set(CMAKE_CXX_VISIBILITY_PRESET hidden)
	set(CMAKE_VISIBILITY_INLINES_HIDDEN hidden)
	
	#启用C++17标准
	set(CMAKE_CXX_STANDARD 17)
	#确保所有目标类型都遵循可见生属性
	cmake_policy(SET CMP0063 NEW)
	#MSVC和非MSVC通用配置
	if(MSVC)
		#异常
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
		#并行编译
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /FS")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /FS")
		#禁止编译器进行不安全的代数变换
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:precise /fp:contract")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /fp:precise /fp:contract")
		#发布优化
		if (CMAKE_BUILD_TYPE MATCHES "Release")
			#消除未引用的函数和数据
			add_link_options(/OPT:REF)
			#启用COMDAT折叠
			add_link_options(/OPT:ICF)
		else()
			#用于“编辑并继续”的程序数据库
			#add_compile_options(/ZI)
			#启用增量连接
			add_link_options(/INCREMENTAL)
		endif()
		#生成调试信息
		add_link_options(/DEBUG)
		#添加相应宏
		add_definitions(-DNOMINMAX)
		add_definitions(-D_CRT_SECURE_NO_WARNINGS)
		add_definitions(-DCRT_NONSTDC_NO_DPRECATE)
		add_definitions(-DWIN32_LEAN_AND_MEAN)
		#VS2022 MUTEX BUG
		add_compile_definitions(_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR=1)
		#x64特殊配置
		if (HX_ARCHITECTURE MATCHES "x64")
			if (CMAKE_BUILD_TYPE MATCHES "Release")
				#开启AVX2加速
				set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
				set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:AVX2")
			endif()
		endif()
	else()
		#禁用快速数学优化
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-fast-math")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-fast-math")
		#RELEASE下生成调试信息并保留优化
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O2 -Wstrict-aliasing")
		#x64特殊配置
		if (HX_ARCHITECTURE MATCHES "x64")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpmath=sse")
			set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpmath=sse")
			if (CMAKE_BUILD_TYPE MATCHES "Release")
				#开启AVX2加速
				set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -mfma")
				set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mavx2 -mfma")
			endif()
		endif()
	endif()
	#尝试启用跨过程优化
	include(CheckIPOSupported)
	check_ipo_supported(RESULT CHECK_IPO_RESULT OUTPUT CHECK_IPO_OUTPUT)
	if (CHECK_IPO_RESULT)
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
	endif()
endmacro(HxInit)
