GN_vcpkg_install(boost)

set(Boost_USE_STATIC_RUNTIME off)
set(Boost_USE_STATIC_LIBS    on)
set(Boost_USE_MULTITHREADED  on)
find_package(Boost REQUIRED 
    filesystem 
    date_time 
    thread
    system
    regex)


GN_Unit(boost
    Libs            ${Boost_LIBRARIES}
    Public          ${Boost_INCLUDE_DIR}
    Definitions     -DBOOST_ALL_DYN_LINK -DBOOST_ALL_NO_LIB ${Boost_LIB_DIAGNOSTIC_DEFINITIONS}
    bFlat
)
