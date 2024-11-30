#![allow(dead_code)]

const SUFFIX_SIZE: usize = std::mem::size_of::<*const u8>();

// NOTE: the sizes are intentionally avoiding the expected inlined length
const fn inlined_length<const N: usize>() -> usize {
    N + SUFFIX_SIZE - 1
}

// NOTE: the sizes are intentionally avoiding the expected inlined length
const fn allocated_length<const N: usize>() -> usize {
    N + SUFFIX_SIZE + 1
}

// TODO: add checks that the string cannot be instanciated using invalid prefix legths
// NOTE: #[should_panic] does not support compile-time assets

macro_rules! generate_main_tests {
    ($name:ident, $type:ident, $N:expr) => {
        #[allow(non_snake_case)] // NOTE: e.g. main_tests_BoxString_04
        mod $name {
            type MyString = concepts_umbra_strings::$type<$N>;

            #[test]
            fn test_size() {
                const EXPECTED: usize = 4 + $N + crate::SUFFIX_SIZE;
                std::assert_eq!(std::mem::size_of::<MyString>(), EXPECTED);
            }

            #[test]
            fn test_construct_prefix_only() {
                let expected = "1".repeat($N - 1);
                std::assert_eq!(
                    MyString::try_from(expected.as_str()).expect("").as_ref(),
                    expected
                );
            }

            #[test]
            fn test_construct_inlined() {
                let expected = "1".repeat(crate::inlined_length::<$N>());
                std::assert_eq!(
                    MyString::try_from(expected.as_str()).expect("").as_ref(),
                    expected
                );
            }

            #[test]
            fn test_construct_allocated() {
                let expected = "1".repeat(crate::allocated_length::<$N>());
                std::assert_eq!(
                    MyString::try_from(expected.as_str()).expect("").as_ref(),
                    expected
                );
            }
        }
    };
    ($type:ident) => {
        paste::paste! {
            generate_main_tests!([<main_tests_ $type _04>], $type, 4);
            generate_main_tests!([<main_tests_ $type _12>], $type, 12); // 4 + 8
            generate_main_tests!([<main_tests_ $type _20>], $type, 20); // 4 + 16
            generate_main_tests!([<main_tests_ $type _28>], $type, 28); // 4 + 24
        }
    };
}
generate_main_tests!(BoxString);

// =========================================================

macro_rules! generate_threaded_tests {
    ($name:ident, $type:ident, $N:expr) => {
        #[allow(non_snake_case)] // NOTE: e.g. main_tests_BoxString_04
        mod $name {
            type MyString = concepts_umbra_strings::$type<$N>;

            // TODO: impl theraded tests
            #[test]
            fn test_move_to_thread_inlined() {
                let expected = "1".repeat(crate::inlined_length::<$N>());

                let boxed = MyString::try_from(expected.as_str()).expect("");

                let handles: Vec<_> = (0..8)
                    .map(|_| {
                        let boxed_cloned = boxed.clone();
                        let expected_cloned = expected.clone();
                        std::thread::spawn(move || assert_eq!(boxed_cloned, expected_cloned))
                    })
                    .collect();

                for handle in handles {
                    handle.join().expect("Thread finishes successfully");
                }
            }

            // TODO: impl theraded tests
            #[test]
            fn test_move_to_thread_allocated() {
                let expected = "1".repeat(crate::allocated_length::<$N>());

                let boxed = MyString::try_from(expected.as_str()).expect("");

                let handles: Vec<_> = (0..8)
                    .map(|_| {
                        let boxed_cloned = boxed.clone();
                        let expected_cloned = expected.clone();
                        std::thread::spawn(move || assert_eq!(boxed_cloned, expected_cloned))
                    })
                    .collect();

                for handle in handles {
                    handle.join().expect("Thread finishes successfully");
                }
            }
        }
    };
    ($type:ident) => {
        paste::paste! {
            generate_threaded_tests!([<threaded_tests_ $type _04>], $type, 4);
            generate_threaded_tests!([<threaded_tests_ $type _12>], $type, 12); // 4 + 8
            generate_threaded_tests!([<threaded_tests_ $type _20>], $type, 20); // 4 + 16
            generate_threaded_tests!([<threaded_tests_ $type _28>], $type, 28); // 4 + 24
        }
    };
}
generate_threaded_tests!(BoxString);

// =========================================================

macro_rules! generate_cross_cmp_tests {
    (@format $name:ident, $TypeA:ident, $NA:expr, $TypeB:ident, $NB:expr) => {
        #[allow(non_snake_case)] // NOTE: e.g. main_tests_BoxString_04
        mod $name {
            type MyStringA = concepts_umbra_strings::$TypeA<$NA>;
            type MyStringB = concepts_umbra_strings::$TypeB<$NB>;

            #[test]
            fn test_eq() {
                let assert = |size: usize| {
                    let value = "1".repeat(size);
                    let str_a = MyStringA::try_from(&value).expect("");
                    let str_b = MyStringB::try_from(&value).expect("");
                    assert_eq!(str_a, str_b);
                };
                assert(crate::inlined_length::<$NA>());
                assert(crate::inlined_length::<$NB>());
                assert(crate::allocated_length::<$NA>());
                assert(crate::allocated_length::<$NB>());
            }

            #[test]
            fn test_ne_with_trailing_null() {
                let assert = |size: usize| {
                    let lhs = "1".repeat(size);

                    let mut rhs = "1".repeat(size);
                    rhs.push_str("\0");

                    let str_a = MyStringA::try_from(&lhs).expect("");
                    let str_b = MyStringB::try_from(&rhs).expect("");

                    // all tests in one place
                    assert_ne!(lhs, rhs);
                    assert_ne!(lhs, str_b);
                    assert_ne!(str_a, rhs);
                    assert_ne!(str_a, str_b);
                };
                assert($NA - 1);
                assert($NB - 1);
                assert(crate::inlined_length::<$NA>() - 1);
                assert(crate::inlined_length::<$NB>() - 1);
                assert(crate::allocated_length::<$NA>() - 1);
                assert(crate::allocated_length::<$NB>() - 1);
            }

            #[test]
            fn test_cmp() {
                //
            }
        }
    };
    (@format $TypeA:ident, $NA:expr, $TypeB:ident, $NB:expr) => {
        paste::paste! {
            generate_cross_cmp_tests!(
                @format [<cross_cmp_tests_ $TypeA _ $NA _ $TypeB _ $NB>],
                $TypeA, $NA, $TypeB, $NB
            );
        }
    };
    (@unpack $TypeA:ident, $NA:expr, $TypeB:ident, [$($NB:expr),+]) => {
        $(generate_cross_cmp_tests!(@format $TypeA, $NA, $TypeB, $NB);)*
    };
    (@unpack $TypeA:ident, $NA:expr, [$($TypeB:ident),+] x $NB:tt) => {
        $(generate_cross_cmp_tests!(@unpack $TypeA, $NA, $TypeB, $NB);)*
    };
    (@unpack $TypeA:ident, [$($NA:expr),+] x $TypeB:tt x $NB:tt) => {
        $(generate_cross_cmp_tests!(@unpack $TypeA, $NA, $TypeB x $NB);)*
    };
    (@unpack [$($TypeA:ident),+] x $NA:tt x $TypeB:tt x $NB:tt) => {
        $(generate_cross_cmp_tests!(@unpack $TypeA, $NA x $TypeB x $NB);)*
    };
    ($Type:tt x $N:tt) => {
        generate_cross_cmp_tests!(@unpack $Type x $N x $Type x $N);
    };
}
generate_cross_cmp_tests!([BoxString] x [4, 12]);
