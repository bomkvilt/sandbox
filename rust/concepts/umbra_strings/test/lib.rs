const SUFFIX_SIZE: usize = std::mem::size_of::<*const u8>();

// TODO: add checks that the string cannot be instanciated using invalid prefix legths
// NOTE: #[should_panic] does not support compile-time assets

macro_rules! generate_main_tests {
    ($name:ident, $type:ident, $N:expr) => {
        // NOTE: e.g. main_tests_BoxString_04
        #[allow(non_snake_case)]
        mod $name {
            type BoxString = concepts_umbra_strings::$type<$N>;

            // NOTE: the sizes are intentionally avoiding the expected inlined length
            const INLINED_LENGTH: usize = $N + crate::SUFFIX_SIZE - 1;
            const ALLOCATED_LENGTH: usize = $N + crate::SUFFIX_SIZE + 1;

            #[test]
            fn test_size() {
                const EXPECTED: usize = 4 + $N + crate::SUFFIX_SIZE;
                std::assert_eq!(std::mem::size_of::<BoxString>(), EXPECTED);
                // assert_eq!(std::mem::size_of::<ArcString>(), EXPECTED);
                // assert_eq!(std::mem::size_of::<RcString>(), EXPECTED);
            }

            #[test]
            fn test_construct_prefix_only() {
                let expected = "1".repeat($N);
                std::assert_eq!(
                    BoxString::try_from(expected.as_str()).expect("").as_ref(),
                    expected
                );
            }

            #[test]
            fn test_construct_inlined() {
                let expected = "1".repeat(INLINED_LENGTH);
                std::assert_eq!(
                    BoxString::try_from(expected.as_str()).expect("").as_ref(),
                    expected
                );
            }

            #[test]
            fn test_construct_allocated() {
                let expected = "1".repeat(ALLOCATED_LENGTH);
                std::assert_eq!(
                    BoxString::try_from(expected.as_str()).expect("").as_ref(),
                    expected
                );
            }

            // TODO: impl theraded tests
            #[test]
            fn test_move_to_thread_inlined() {
                let expected = "1".repeat(INLINED_LENGTH);

                let boxed = BoxString::try_from(expected.as_str()).expect("");

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
                let expected = "1".repeat(ALLOCATED_LENGTH);

                let boxed = BoxString::try_from(expected.as_str()).expect("");

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
}
macro_rules! generate_main_tests_bunch {
    ($type:ident) => {
        paste::paste! {
            generate_main_tests!([<main_tests_ $type _04>], BoxString, 4);
            generate_main_tests!([<main_tests_ $type _12>], BoxString, 12); // 4 + 8
            generate_main_tests!([<main_tests_ $type _20>], BoxString, 20); // 4 + 16
            generate_main_tests!([<main_tests_ $type _28>], BoxString, 28); // 4 + 24
        }
    };
}
generate_main_tests_bunch!(BoxString);
