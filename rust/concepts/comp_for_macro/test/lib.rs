#[test]
fn test_generator() {
    let expected = (0..10).map(|x| x * 2).collect::<Vec<_>>();
    let produced = {
        comp_for_macro::genexp![
            x * 2 for x in 0..10
        ]
        .collect::<Vec<_>>()
    };
    std::assert_eq!(expected, produced);
}

#[test]
fn test_generator_single_if() {
    let expected = (0..10).filter(|x| 2 < *x && *x < 6).map(|x| x * 2).collect::<Vec<_>>();

    let produced = {
        comp_for_macro::genexp![
            x * 2 for x in 0..10 if 2 < x && x < 6
        ]
        .collect::<Vec<_>>()
    };

    std::assert_eq!(expected, produced);
}

#[test]
fn test_generator_multiple_if() {
    let expected = (0..10).filter(|x| 2 < *x && *x < 6).map(|x| x * 2).collect::<Vec<_>>();

    let produced = {
        comp_for_macro::genexp![
            x * 2 for x in 0..10 if 2 < x if x < 6
        ]
        .collect::<Vec<_>>()
    };

    std::assert_eq!(expected, produced);
}

#[test]
fn test_generator_multiple_clauses() {
    let mut expected = vec![];
    for x in 0..10 {
        for y in 3..10 {
            if 2 < x && x < 6 && y % 3 == 0 && x * y % 2 == 0 {
                expected.push(x * y * 2);
            }
        }
    }

    let produced = {
        comp_for_macro::genexp![
            x * y * 2
            for x in 0..10
            if 2 < x
            if x < 6
            for y in 3..10
            if y % 3 == 0
            if x * y % 2 == 0
        ]
        .collect::<Vec<_>>()
    };
    std::assert_eq!(expected, produced);
}

#[test]
fn test_binding() {
    let expected = vec![4, 24, 60];

    let raw_data = vec![(1, 2), (3, 4), (5, 6)];
    let produced = {
        comp_for_macro::genexp!(
            x * y * 2 for (x, y) in raw_data
        )
        .collect::<Vec<_>>()
    };
    std::assert_eq!(expected, produced);
}
