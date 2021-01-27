use crate::err::{ErrorType, ProcessingResult};
use crate::proc::Processor;

fn is_string_delimiter(c: u8) -> bool {
    c == b'"' || c == b'\''
}

fn parse_comment_single(proc: &mut Processor) -> ProcessingResult<()> {
    chain!(proc.match_seq(b"//").expect().keep());

    // Comment can end at closing </script>.
    // TODO WARNING: Closing tag must not contain whitespace.
    // TODO Optimize
    while !chain!(proc.match_line_terminator().keep().matched()) {
        if chain!(proc.match_seq(b"</script>").matched()) {
            break;
        }

        proc.accept()?;
    }

    Ok(())
}

fn parse_comment_multi(proc: &mut Processor) -> ProcessingResult<()> {
    chain!(proc.match_seq(b"/*").expect().keep());

    // Comment can end at closing </script>.
    // TODO WARNING: Closing tag must not contain whitespace.
    // TODO Optimise
    while !chain!(proc.match_seq(b"*/").keep().matched()) {
        if chain!(proc.match_seq(b"</script>").matched()) {
            break;
        }

        proc.accept()?;
    }

    Ok(())
}

fn parse_string(proc: &mut Processor) -> ProcessingResult<()> {
    let delim = chain!(proc.match_pred(is_string_delimiter).expect().keep().char());

    let mut escaping = false;

    loop {
        let c = proc.accept()?;

        if c == b'\\' {
            escaping = !escaping;
            continue;
        }

        if c == delim && !escaping {
            break;
        }

        if chain!(proc.match_line_terminator().keep().matched()) {
            if !escaping {
                return Err(ErrorType::UnterminatedJsString);
            }
        }

        escaping = false;
    }

    Ok(())
}

fn parse_template(proc: &mut Processor) -> ProcessingResult<()> {
    chain!(proc.match_char(b'`').expect().keep());

    let mut escaping = false;

    loop {
        let c = proc.accept()?;

        if c == b'\\' {
            escaping = !escaping;
            continue;
        }

        if c == b'`' && !escaping {
            break;
        }

        escaping = false;
    }

    Ok(())
}

pub fn process_js_script(proc: &mut Processor) -> ProcessingResult<()> {
    while !chain!(proc.match_seq(b"</").matched()) {
        if chain!(proc.match_seq(b"//").matched()) {
            parse_comment_single(proc)?;
        } else if chain!(proc.match_seq(b"/*").matched()) {
            parse_comment_multi(proc)?;
        } else if chain!(proc.match_pred(is_string_delimiter).matched()) {
            parse_string(proc)?;
        } else if chain!(proc.match_char(b'`').matched()) {
            parse_template(proc)?;
        } else {
            proc.accept()?;
        }
    }
    Ok(())
}
