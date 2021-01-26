use crate::proc::Processor;
use crate::err::InsRes;

pub fn process_bang<'d, 'p>(proc: &'p mut Processor<'d>) -> InsRes<()> {
    cascade_return!(proc.match_seq(b"<!").require()?.keep());

    cascade_return!(proc.match_while_not_char(b'>').keep());

    cascade_return!(proc.match_char(b'>').require()?.keep());

    Ok(())
}