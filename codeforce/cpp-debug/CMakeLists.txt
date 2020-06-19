file(READ "${CMAKE_CURRENT_LIST_DIR}/main.target.txt" tragetFile)
string(REPLACE "\\" "/" tragetFile ${tragetFile})
string(REPLACE "\n" ""  tragetFile ${tragetFile})
string(REPLACE "\r" ""  tragetFile ${tragetFile})
string(REPLACE "\"" ""  tragetFile ${tragetFile})
string(REPLACE  "'" ""  tragetFile ${tragetFile})

file(WRITE "${CMAKE_CURRENT_LIST_DIR}/main.include.cpp" "#include <${tragetFile}>\n")

GN_unit(codeforce-cpp-debug bFlat
    mode eApp
    extra_src "${tragetFile}"
)
