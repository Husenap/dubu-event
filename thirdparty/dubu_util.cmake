message("-- External Project: dubu_util")
include(FetchContent)

FetchContent_Declare(
    dubu_util
    GIT_REPOSITORY  https://github.com/Husenap/dubu-util.git
    GIT_TAG         v1.0
)

set(dubu_util_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(dubu_util_FOLDER "thirdparty/dubu_util" CACHE STRING "" FORCE)

FetchContent_MakeAvailable(dubu_util)
