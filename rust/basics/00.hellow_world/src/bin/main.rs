extern crate hello_lib;

use hello_lib::greeter;

fn main() {
    let hello = greeter::Greeter::new("Hello");
    hello.greet("world");
}
