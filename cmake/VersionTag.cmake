#===============================================================================
# Check current git repo is modified
# if modified, HX_GIT_MODIFIED will be defined and set to ON
function(HxCheckGitModified)
	execute_process(COMMAND "git" "status" "-s" "-uno"
					WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					OUTPUT_VARIABLE CONSOLE_OUTPUT
					RESULT_VARIABLE RETCODE)
	
	if(${RETCODE} EQUAL 0)
		string(STRIP "${CONSOLE_OUTPUT}" CONSOLE_OUTPUT)
		string(LENGTH "${CONSOLE_OUTPUT}" CONSOLE_OUTPUT_LEN)
		#message(STATUS "OUTPUT: ${CONSOLE_OUTPUT}: ${CONSOLE_OUTPUT_LEN}")
		if(${CONSOLE_OUTPUT_LEN} GREATER 0)
			message(STATUS "Working tree is modified")
			set(HX_GIT_MODIFIED ON PARENT_SCOPE)
		endif()
	endif()
endfunction()

#===============================================================================
# Get current git repo HEAD commit's date
function(HxGetGitRepoRevDate)
	execute_process(COMMAND "git" "log" "-1" "--pretty=format:%cd" "--date=format:%y%m%d"
					WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					OUTPUT_VARIABLE CONSOLE_OUTPUT
					RESULT_VARIABLE RETCODE)

	string(STRIP "${CONSOLE_OUTPUT}" CONSOLE_OUTPUT)
	if(${RETCODE} EQUAL 0)
		set(HX_GIT_REPO_REV_DATE "${CONSOLE_OUTPUT}" PARENT_SCOPE)
	endif()	
endfunction()

#===============================================================================
# Get current git repo HEAD commit's hash
function(HxGetGitRepoRevHash)
	execute_process(COMMAND "git" "rev-parse" "--short" "HEAD"
					WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
					OUTPUT_VARIABLE CONSOLE_OUTPUT
					RESULT_VARIABLE RETCODE)

	string(STRIP "${CONSOLE_OUTPUT}" CONSOLE_OUTPUT)
	if(${RETCODE} EQUAL 0)
		set(HX_GIT_REPO_REV_HASH "${CONSOLE_OUTPUT}" PARENT_SCOPE)
	endif()	
endfunction()

#===============================================================================
# Get version tag
# HX_VERSION_TAG: used to generate target's last version part
# 					1. if current repo is modified, use current date as version, eg: 260701
#					2. otherwise use current commit's hash plus date as version, eg: f234a45@260701
function(HxGetVersionTag)
	HxCheckGitModified()
	
	if(NOT HX_GIT_MODIFIED)
		HxGetGitRepoRevHash()
		HxGetGitRepoRevDate()
	endif()
	
	if(HX_GIT_REPO_REV_HASH)
		if (NOT DEFINED HX_GIT_REPO_REV_DATE)
			string(TIMESTAMP HX_GIT_REPO_REV_DATE "%y%m%d")
		endif()
		set(HX_VERSION_TAG "${HX_GIT_REPO_REV_HASH}@${HX_GIT_REPO_REV_DATE}" PARENT_SCOPE)
	else()
		string(TIMESTAMP TODAY "%y%m%d")
		set(HX_VERSION_TAG "${TODAY}" PARENT_SCOPE)
	endif()
endfunction()
