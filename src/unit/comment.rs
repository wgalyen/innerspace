use crate::proc::Processor;
use crate::err::InsRes;

pub fn process_comment<'d, 'p>(proc: &'p mut Processor<'d>) -> InsRes<()> {
    cascade_return!(proc.match_seq(b"<!--").expect().discard());

    // TODO Cannot use this pattern
    cascade_return!(proc.match_while_not_seq(b"-->").discard());

    cascade_return!(proc.match_seq(b"-->").require_with_reason("comment end")?.discard());

    Ok(())
}