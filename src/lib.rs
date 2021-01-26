use crate::err::InsRes;
use crate::proc::Processor;
use crate::unit::content::process_content;

mod code;
pub mod err;
#[macro_use]
mod proc;
mod spec;
mod unit;

/**
 * Run innerspace on an input array and write to {@param output}. Output will be
 * null terminated if no error occurs. WARNING: Input must end with '\xFF' or
 * '\0', and {@param input_size} must not include the terminator. WARNING: Does
 * not check if {@param output} is large enough. It should at least match the
 * size of the input.
 *
 * @param input input array to process
 * @param output output array to write to
 * @param cfg configuration to use
 * @return result where to write any resulting error information
 */
pub fn innerspace<'d>(code: &'d mut [u8]) -> InsRes<usize> {
    let mut p = Processor::new(code);
    process_content(&mut p, None)?;
    Ok(p.written_len())
}
