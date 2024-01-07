module;

// NOTE: I've failed to use header units 🤷
#include <string>

// NOTE: `module {module_name};` is required
// NOTE: the file does not define BMI so it must be putted to a raw PRIVATE source section (without FILE_SET)
// NOTE: all the code below will be mangled like `<...>@test_module`
module test_module;

/**
    NOTE: the implementation units still allows to avoid unwanted recompilation in other units and make.
    NOTE: ANY changes in INTERFACE module units invaludates all module object files (!!!)

    NOTE: studied BEST_PRACTICEs
    - module must be realised using interface and implementation units (like .hpp and .cpp before)
    - module must be relatively small to reduce bild cache invalidation effects (?)

    Changes in an implementation unit + (build all):

    # the opny one file has been changed
    - [build] [1/8  12% :: 0.180] Scanning /home/bomkvilt/algos/projects/c++20.modules/module.cpp for CXX dependencies

    # <building system magic>
      [build] [2/8  25% :: 0.239] Generating CXX dyndep file projects/c++20.modules/CMakeFiles/c++20.modules.dir/CXX.dd
      [build] [5/8  37% :: 0.312] Generating CXX dyndep file projects/c++20.modules/consumer/CMakeFiles/c++20.modules.consumer.dir/CXX.dd
      [build] [5/8  50% :: 0.325] Generating CXX dyndep file projects/c++20.modules/ut/CMakeFiles/c++20.modules.ut.dir/CXX.dd

      # building the updated .cpp file only
      [build] [5/8  62% :: 0.546] Building CXX object projects/c++20.modules/CMakeFiles/c++20.modules.dir/module.cpp.o

      # relinking the dependent targets
      [build] [6/8  75% :: 0.585] Linking CXX static library projects/c++20.modules/libc++20.modules.a
      [build] [8/8  87% :: 0.727] Linking CXX executable projects/c++20.modules/consumer/c++20.modules.consumer
      [build] [8/8 100% :: 2.042] Linking CXX executable projects/c++20.modules/ut/c++20.modules.ut

    ------------------------------------------------------------------------------------------------

    Changes in an inteface unit (`:private` segment) + (build all):

    # the opny one file has been changed
    + [build] [1/8  12% :: 0.198] Scanning /home/bomkvilt/algos/projects/c++20.modules/module.cppm for CXX dependencies

    # <building system magic>
      [build] [2/8  25% :: 0.255] Generating CXX dyndep file projects/c++20.modules/CMakeFiles/c++20.modules.dir/CXX.dd
      [build] [5/11  27% :: 0.329] Generating CXX dyndep file projects/c++20.modules/consumer/CMakeFiles/c++20.modules.consumer.dir/CXX.dd
      [build] [5/12  33% :: 0.342] Generating CXX dyndep file projects/c++20.modules/ut/CMakeFiles/c++20.modules.ut.dir/CXX.dd

    # NOTE: interface unit changes invalidate ALL module's TUs (new negative behaviour)
    + [build] [5/13   38% :: 0.830] Building CXX object projects/c++20.modules/CMakeFiles/c++20.modules.dir/module.cppm.o
    + [build] [10/13  46% :: 1.188] Building CXX object projects/c++20.modules/CMakeFiles/c++20.modules.dir/submodule/a.cpp.o
      [build] [10/13  53% :: 1.196] Building CXX object projects/c++20.modules/CMakeFiles/c++20.modules.dir/module.cpp.o

    # NOTE: interface unit changes invalidate all TUs where they were imported to (like header files)
    + [build] [11/13  69% :: 1.537] Building CXX object projects/c++20.modules/consumer/CMakeFiles/c++20.modules.consumer.dir/main.cpp.o
    + [build] [12/13  84% :: 1.961] Building CXX object projects/c++20.modules/ut/CMakeFiles/c++20.modules.ut.dir/private_tests.cpp.o
    + [build] [12/13  92% :: 2.914] Building CXX object projects/c++20.modules/ut/CMakeFiles/c++20.modules.ut.dir/public_tests.cpp.o

    # relinking the dependent targets
      [build] [11/13  61% :: 1.243] Linking CXX static library projects/c++20.modules/libc++20.modules.a
      [build] [12/13  76% :: 1.700] Linking CXX executable projects/c++20.modules/consumer/c++20.modules.consumer
      [build] [13/13 100% :: 4.472] Linking CXX executable projects/c++20.modules/ut/c++20.modules.ut
 */

namespace  NTestModule {
    std::string TPublicClass::Name() const {
        return "public_class";
    }

    std::string TPrivateClass::Name() const {
        return "private_class";
    }
}
