#![allow(dead_code)]

use quickcheck_macros::quickcheck;

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
                    MyString::try_from(expected.as_str()).unwrap().as_ref(),
                    expected
                );
            }

            #[test]
            fn test_construct_inlined() {
                let expected = "1".repeat(crate::inlined_length::<$N>());
                std::assert_eq!(
                    MyString::try_from(expected.as_str()).unwrap().as_ref(),
                    expected
                );
            }

            #[test]
            fn test_construct_allocated() {
                let expected = "1".repeat(crate::allocated_length::<$N>());
                std::assert_eq!(
                    MyString::try_from(expected.as_str()).unwrap().as_ref(),
                    expected
                );
            }

            #[crate::quickcheck]
            fn test_debug(base: String) {
                let test = MyString::try_from(&base).unwrap();
                std::assert_eq!(std::format!("{test:?}"), std::format!("{base:?}"));
            }

            #[crate::quickcheck]
            fn test_display(base: String) {
                let test = MyString::try_from(&base).unwrap();
                std::assert_eq!(std::format!("{test}"), std::format!("{base}"));
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

            #[crate::quickcheck]
            fn test_move_to_thread(base: String) {
                let test = MyString::try_from(base.as_str()).unwrap();

                let handles: Vec<_> = (0..8)
                    .map(|_| {
                        let test_cloned = test.clone();
                        let base_cloned = base.clone();
                        std::thread::spawn(move || assert_eq!(test_cloned, base_cloned))
                    })
                    .collect();

                for handle in handles {
                    handle.join().unwrap();
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

            fn list_sizes() -> &'static [usize] {
                static SIZES: [usize; 6] = [
                    $NA, crate::inlined_length::<$NA>(), crate::allocated_length::<$NA>(),
                    $NB, crate::inlined_length::<$NB>(), crate::allocated_length::<$NB>(),
                ];
                return &SIZES;
            }

            #[test]
            fn test_eq() {
                let assert_by_size = |size: usize| {
                    let value = "1".repeat(size);
                    let str_a = MyStringA::try_from(&value).unwrap();
                    let str_b = MyStringB::try_from(&value).unwrap();
                    assert_eq!(str_a, str_b);
                };
                for size in list_sizes() {
                    assert_by_size(*size);
                }
            }

            #[test]
            fn test_ne_with_trailing_null() {
                let assert_by_size = |size: usize| {
                    let lhs = "1".repeat(size - 1);

                    let mut rhs = "1".repeat(size - 1);
                    rhs.push_str("\0");

                    let str_a = MyStringA::try_from(&lhs).unwrap();
                    let str_b = MyStringB::try_from(&rhs).unwrap();

                    assert_ne!(lhs, rhs);
                    assert_ne!(str_a, str_b);
                };
                for size in list_sizes() {
                    assert_by_size(*size);
                }
            }

            #[test]
            fn test_cmp() {
                let assert = |lhs: &str, rhs: &str| {
                    let str_a = MyStringA::try_from(lhs).unwrap();
                    let str_b = MyStringB::try_from(rhs).unwrap();

                    assert_eq!(
                        PartialOrd::partial_cmp(lhs, rhs),
                        PartialOrd::partial_cmp(&str_a, &str_b)
                    );
                };
                let assert_by_size = |size: usize| {
                    // case: equal strings
                    let lhs = "1".repeat(size);
                    assert(&lhs, &lhs);

                    // case: null-terminated
                    let lhs = "1".repeat(size - 1);
                    let mut rhs = "1".repeat(size - 1);
                    rhs.push_str("\0");
                    assert(&lhs, &rhs);
                    assert(&rhs, &lhs);

                    // case: less
                    let lhs = "1".repeat(size);
                    for i in 0..size {
                        let mut rhs = "1".repeat(size);
                        rhs.replace_range(i..i+1, "2");
                        assert_eq!(lhs.len(), rhs.len());

                        assert(&lhs, &rhs);
                        assert(&rhs, &lhs);
                    }
                };
                for size in list_sizes() {
                    assert_by_size(*size);
                }
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
