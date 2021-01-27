use afl::fuzz;
use innerspace::{innerspace, ErrorType};

fn main() {
    fuzz!(|data: &[u8]| {
        let mut mut_data: Vec<u8> = data.iter().map(|x| *x).collect();
        innerspace(&mut mut_data);
    });
}