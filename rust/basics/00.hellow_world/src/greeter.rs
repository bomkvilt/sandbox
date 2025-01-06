// NOTE: the include is only needed to check that bazel build property works with crate universe
#[allow(unused_imports)]
use rand::Rng as _;

pub struct Greeter {
    greeting: String,
}

impl Greeter {
    #[must_use]
    pub fn new(greeting: &str) -> Greeter {
        Greeter {
            greeting: greeting.to_string(),
        }
    }

    pub fn from_resource_file() -> runfiles::Result<Greeter> {
        let resource = runfiles::Runfiles::create()?;
        Ok(Greeter {
            greeting: std::fs::read_to_string(
                runfiles::rlocation!(
                    resource,
                    "sandbox/rust/basics/00.hellow_world/data/greeting.txt"
                )
                .unwrap(),
            )
            .map_err(runfiles::RunfilesError::RunfileIoError)?,
        })
    }

    #[must_use]
    pub fn greeting(&self, thing: &str) -> String {
        format!("{} {}", &self.greeting, thing)
    }

    pub fn greet(&self, thing: &str) {
        println!("{} {}", &self.greeting, thing);
    }
}

#[cfg(test)]
mod test {
    use super::Greeter;

    #[test]
    fn test_greeting() {
        let hello = Greeter::new("Hi");
        assert_eq!("Hi Rust", hello.greeting("Rust"));
    }

    #[test]
    fn test_greeting_from_txt_file() {
        let welcome = Greeter::from_resource_file().unwrap();
        assert_eq!("Welcome Rust", welcome.greeting("Rust"));
    }
}
