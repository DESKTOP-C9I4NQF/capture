use rustc_demangle::demangle;
fn main() {
	println!("{}", demangle("_ZN4testE").to_string());
}
