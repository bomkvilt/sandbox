#ifndef CODEFORCE_CPP__CF_TEST_HPP
#define CODEFORCE_CPP__CF_TEST_HPP

#include <gtest/gtest.h>
#include <unordered_map>
#include <array>


// it's hackyway to use a macro defintion's value as a function name
#define PROBLEM_FUNC(name) main##_##name
#define PROBLEM_FONC(name) PROBLEM_FUNC(name)
#define PROBLEM_MAIN int PROBLEM_FONC(PROBLEM_NAME)

#define PROBLEM_TEST_NAME(name) test_##name
#define PROBLEM_TEST_NOME(name) PROBLEM_TEST_NAME(name)
#define PROBLEM_TEST PROBLEM_TEST_NOME(PROBLEM_NAME)


#endif //!CODEFORCE_CPP__CF_TEST_HPP
