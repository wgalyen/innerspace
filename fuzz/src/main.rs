use afl::fuzz;
use innerspace::innerspace;

fn main() {
    fuzz!(|data: &[u8]| {
        let mut mut_data: Vec<u8> = data.iter().map(|x| *x).collect();
        (&mut mut_data);
    });
}