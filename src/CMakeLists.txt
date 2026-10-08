add_library(ok_api_lib 
    api.cpp
    client.cpp
    content_types.cpp
    context.cpp
    convert.cpp
    file.cpp
    http_error.cpp
    http.cpp
    make_response.cpp
    request.cpp
    response.cpp
    router.cpp
    serializer.cpp
    server.cpp
    worker.cpp
)

include(FetchContent)

FetchContent_Declare(
    byte_talk
    GIT_REPOSITORY https://github.com/mikhailshevtsov/byte_talk.git
    GIT_TAG main
)
FetchContent_MakeAvailable(byte_talk)


FetchContent_Declare(
    mx_parser
    GIT_REPOSITORY https://github.com/mikhailshevtsov/mx_parser.git
    GIT_TAG main
)
FetchContent_MakeAvailable(mx_parser)


FetchContent_Declare(
    mx_router
    GIT_REPOSITORY https://github.com/mikhailshevtsov/mx_router.git
    GIT_TAG main
)
FetchContent_MakeAvailable(mx_router)


FetchContent_Declare(
    jarl
    GIT_REPOSITORY https://github.com/mikhailshevtsov/jarl.git
    GIT_TAG main
)
FetchContent_MakeAvailable(jarl)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(json)

target_link_libraries(ok_api_lib PUBLIC byte_talk mx_parser mx_router jarl nlohmann_json::nlohmann_json)
target_compile_features(ok_api_lib PUBLIC cxx_std_20)
