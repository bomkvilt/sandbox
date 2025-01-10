use syn::parse::{Parse, ParseStream};
use syn::{parse_macro_input, Expr, Pat, Token};

struct GenExpr {
    expression: Expression,
    if_clauses: Vec<ForIfClause>,
}

impl Parse for GenExpr {
    fn parse(input: ParseStream) -> syn::Result<Self> {
        Ok(Self { expression: input.parse()?, if_clauses: parse_zero_or_more(input) })
    }
}

impl quote::ToTokens for GenExpr {
    fn to_tokens(&self, tokens: &mut proc_macro2::TokenStream) {
        let for_ifs_inv = std::iter::IntoIterator::into_iter(&self.if_clauses);
        let mut for_ifs = for_ifs_inv.rev();

        let mut output = {
            let clause = for_ifs.next().expect("at least one for_if_clause is required");
            let ForIfClause { bindings, sequence, conditions } = clause;
            let Expression(expression) = &self.expression;

            quote::quote! {
                ::core::iter::IntoIterator::into_iter(#sequence).filter_map(move |#bindings| {
                    (true #(&& (#conditions))*).then(|| #expression)
                })
            }
        };

        output = for_ifs.fold(output, |current_output, next_layer| {
            let ForIfClause { bindings, sequence, conditions } = next_layer;
            quote::quote! {
                ::core::iter::IntoIterator::into_iter(#sequence).filter_map(move |#bindings| {
                    (true #(&& (#conditions))*).then(|| #current_output)
                })
                .flatten()
            }
        });

        tokens.extend(output);
    }
}

// =================================================================================================

struct ForIfClause {
    bindings: Pat,
    sequence: Expr,
    conditions: Vec<Condition>,
}

impl Parse for ForIfClause {
    fn parse(input: ParseStream) -> syn::Result<Self> {
        _ = input.parse::<Token![for]>()?;
        let bindings = Pat::parse_single(input)?;

        _ = input.parse::<Token![in]>()?;
        let sequence = input.parse()?;

        let conditions = parse_zero_or_more(input);
        Ok(Self { bindings, sequence, conditions })
    }
}

// =================================================================================================

struct Expression(Expr);

impl Parse for Expression {
    fn parse(input: ParseStream) -> syn::Result<Self> {
        input.parse().map(Self)
    }
}

// =================================================================================================

struct Condition(Expr);

impl Parse for Condition {
    fn parse(input: ParseStream) -> syn::Result<Self> {
        _ = input.parse::<Token![if]>()?;
        input.parse().map(Self)
    }
}

impl quote::ToTokens for Condition {
    fn to_tokens(&self, tokens: &mut proc_macro2::TokenStream) {
        self.0.to_tokens(tokens);
    }
}

// =================================================================================================

fn parse_zero_or_more<T: Parse>(input: ParseStream) -> Vec<T> {
    let mut result = Vec::new();
    while let Ok(item) = input.parse() {
        result.push(item);
    }
    result
}

// =================================================================================================

#[proc_macro]
pub fn genexp(input: proc_macro::TokenStream) -> proc_macro::TokenStream {
    let c = parse_macro_input!(input as GenExpr);
    quote::quote! { #c }.into()
}
