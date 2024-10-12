extern crate hello_lib;

use hello_lib::greeter;

fn main() {
    let hello = greeter::Greeter::New("Hello");
    hello.greet("world");
}
