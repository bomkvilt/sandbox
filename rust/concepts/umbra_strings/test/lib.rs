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

// use quickcheck;

#[derive(Debug, Clone)]
struct NullString {
    value: String,
}

impl quickcheck::Arbitrary for NullString {
    fn arbitrary(g: &mut quickcheck::Gen) -> NullString {
        Self {
            value: {
                if *g.choose::<bool>(&[true, false]).unwrap() {
                    String::arbitrary(g) + "\0"
                } else {
                    String::arbitrary(g)
                }
            },
        }
    }
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

            #[quickcheck_macros::quickcheck]
            fn test_debug(base: crate::NullString) {
                let base = base.value;
                let test = MyString::try_from(&base).unwrap();
                std::assert_eq!(std::format!("{test:?}"), std::format!("{base:?}"));
            }

            #[quickcheck_macros::quickcheck]
            fn test_display(base: crate::NullString) {
                let base = base.value;
                let test = MyString::try_from(&base).unwrap();
                std::assert_eq!(std::format!("{test}"), std::format!("{base}"));
            }

            #[quickcheck_macros::quickcheck]
            fn test_eq(base: crate::NullString) {
                let base = base.value;
                let test = MyString::try_from(&base).unwrap();

                // self
                std::assert!(test.eq(&test));

                // String
                std::assert!(test.eq(&base));
                std::assert!(base.eq(&test));

                // str
                std::assert!(test.eq(base.as_str()));
                std::assert!(base.as_str().eq(&test));
            }

            #[quickcheck_macros::quickcheck]
            fn test_cmp(base: crate::NullString) {
                let base = base.value;
                let test = MyString::try_from(&base).unwrap();

                // self
                std::assert_eq!(test.cmp(&test), std::cmp::Ordering::Equal);
                std::assert_eq!(test.partial_cmp(&test), Some(std::cmp::Ordering::Equal));

                // String
                std::assert_eq!(test.partial_cmp(&base), Some(std::cmp::Ordering::Equal));
                std::assert_eq!(base.partial_cmp(&test), Some(std::cmp::Ordering::Equal));

                // str
                std::assert_eq!(
                    test.partial_cmp(base.as_str()),
                    Some(std::cmp::Ordering::Equal),
                );
                std::assert_eq!(
                    base.as_str().partial_cmp(&test),
                    Some(std::cmp::Ordering::Equal),
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
generate_main_tests!(ArcString);

// // =========================================================

macro_rules! generate_threaded_tests {
    ($name:ident, $type:ident, $N:expr) => {
        #[allow(non_snake_case)] // NOTE: e.g. main_tests_BoxString_04
        mod $name {
            type MyString = concepts_umbra_strings::$type<$N>;

            #[quickcheck_macros::quickcheck]
            fn test_move_to_thread(base: crate::NullString) {
                let base = base.value;
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
generate_threaded_tests!(ArcString);

// // =========================================================

macro_rules! generate_cross_tests {
    (@format $name:ident, $TypeA:ident, $NA:expr, $TypeB:ident, $NB:expr) => {
        #[allow(non_snake_case)] // NOTE: e.g. main_tests_BoxString_04
        mod $name {
            type MyStringA = concepts_umbra_strings::$TypeA<$NA>;
            type MyStringB = concepts_umbra_strings::$TypeB<$NB>;

            #[quickcheck_macros::quickcheck]
            fn test_eq(lhs: crate::NullString, rhs: crate::NullString) {
                let str_a = MyStringA::try_from(&lhs.value).unwrap();
                let str_b = MyStringB::try_from(&rhs.value).unwrap();
                assert_eq!(lhs.value.eq(&rhs.value), str_a.eq(&str_b));
            }

            // NOTE: test_eq gives no guaranties that lhs == rhs in a specific launch
            #[quickcheck_macros::quickcheck]
            fn test_eq_same_input(base: crate::NullString) {
                let str_a = MyStringA::try_from(&base.value).unwrap();
                let str_b = MyStringB::try_from(&base.value).unwrap();
                assert_eq!(str_a, str_b);
            }

            #[quickcheck_macros::quickcheck]
            fn test_eq_with_extra_trailing_null(lhs: String) {
                let rhs = lhs.clone() + "\0";

                let str_a = MyStringA::try_from(&lhs).unwrap();
                let str_b = MyStringB::try_from(&rhs).unwrap();

                assert_ne!(lhs, rhs);
                assert_ne!(str_a, str_b);
            }

            #[quickcheck_macros::quickcheck]
            fn test_cmp(lhs: crate::NullString, rhs: crate::NullString) {
                let str_a = MyStringA::try_from(&lhs.value).unwrap();
                let str_b = MyStringB::try_from(&rhs.value).unwrap();

                assert_eq!(
                    PartialOrd::partial_cmp(&lhs.value, &rhs.value),
                    PartialOrd::partial_cmp(&str_a, &str_b)
                );
            }

            // NOTE: test_cmp gives no guaranties that lhs == rhs in a specific launch
            #[quickcheck_macros::quickcheck]
            fn test_cmp_same_input(base: crate::NullString) {
                let str_a = MyStringA::try_from(&base.value).unwrap();
                let str_b = MyStringB::try_from(&base.value).unwrap();

                assert_eq!(
                    PartialOrd::partial_cmp(&str_a, &str_b),
                    Some(std::cmp::Ordering::Equal),
                );
            }

            #[quickcheck_macros::quickcheck]
            fn test_cmp_with_extra_trailing_null(base: crate::NullString) {
                let lhs = base.value;
                let rhs = std::format!("{lhs}\0");

                let str_a = MyStringA::try_from(lhs).unwrap();
                let str_b = MyStringB::try_from(rhs).unwrap();

                assert_eq!(
                    PartialOrd::partial_cmp(&str_a, &str_b),
                    Some(std::cmp::Ordering::Less),
                );
                assert_eq!(
                    PartialOrd::partial_cmp(&str_b, &str_a),
                    Some(std::cmp::Ordering::Greater),
                );
            }
        }
    };
    (@format $TypeA:ident, $NA:expr, $TypeB:ident, $NB:expr) => {
        paste::paste! {
            generate_cross_tests!(
                @format [<cross_cmp_tests_ $TypeA _ $NA _ $TypeB _ $NB>],
                $TypeA, $NA, $TypeB, $NB
            );
        }
    };
    (@unpack $TypeA:ident, $NA:expr, $TypeB:ident, [$($NB:expr),+]) => {
        $(generate_cross_tests!(@format $TypeA, $NA, $TypeB, $NB);)*
    };
    (@unpack $TypeA:ident, $NA:expr, [$($TypeB:ident),+] x $NB:tt) => {
        $(generate_cross_tests!(@unpack $TypeA, $NA, $TypeB, $NB);)*
    };
    (@unpack $TypeA:ident, [$($NA:expr),+] x $TypeB:tt x $NB:tt) => {
        $(generate_cross_tests!(@unpack $TypeA, $NA, $TypeB x $NB);)*
    };
    (@unpack [$($TypeA:ident),+] x $NA:tt x $TypeB:tt x $NB:tt) => {
        $(generate_cross_tests!(@unpack $TypeA, $NA x $TypeB x $NB);)*
    };
    ($Type:tt x $N:tt) => {
        generate_cross_tests!(@unpack $Type x $N x $Type x $N);
    };
}
generate_cross_tests!([ArcString, BoxString] x [4, 12]);
