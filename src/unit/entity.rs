// The minimum length of any entity is 3, which is a character entity reference
// with a single character name. The longest UTF-8 representation of a Unicode
// code point is 4 bytes. Because there are no character entity references with
// a name of length 1, it's always better to decode entities for minification
// purposes.

// Based on the data sourced from https://www.w3.org/TR/html5/entities.json as
// of 2019-04-20T04:00:00.000Z:
// - Entity names can have [A-Za-z0-9] characters, and are case sensitive.
// - Some character entity references do not end with a semicolon, but
// spec says all must (https://html.spec.whatwg.org/multipage/syntax.html#character-references).
//   - All of these entities also have a corresponding entity with semicolon.
// - The longest name is "CounterClockwiseContourIntegral", with length 31
// (excluding leading ampersand and trailing semicolon).
// - All entity names are at least 2 characters long.

// Browser implementation behaviour to consider:
// - It is unclear what happens if an entity name does not match case
// sensitively but matches two or more case insensitively.
//   - For example, given "AlphA" or "aLpha", does the browser choose "alpha" or
//   "Alpha"?
// - Do browsers render valid entities without trailing semicolons?
//   - For example, how do browsers interpret "Chuck-&amp-Cheese", "1&amp1", and
//   "&ampe;"?

// innerspace implementation:
// - Entities must start with an ampersand and end with a semicolon.
// - Once an ampersand is encountered, it and the sequence of characters
// following must match the following ECMAScript regular expression to be
// considered a well formed entity:
//
//   /&(#(x[0-9a-f]{1-6}|[0-9]{1,7}))|[a-z0-9]{2,31};/i
//
// - If the sequence of characters following an ampersand do not combine to form
// a well formed entity, they are treated literally.

use crate::err::ProcessingResult;
use crate::proc::{Processor, ProcessorRange};
use crate::spec::codepoint::{is_digit, is_hex_digit, is_lower_hex_digit, is_upper_hex_digit};
use crate::spec::entity::{ENTITY_REFERENCES, is_valid_entity_reference_name_char};

#[derive(Clone, Copy)]
pub enum EntityType {
    Malformed(ProcessorRange),
    Ascii(u8),
    // If named or numeric reference refers to ASCII char, Type::Ascii is used instead.
    Named(&'static [u8]),
    Numeric(char),
}

impl EntityType {
    pub fn keep(self, proc: &mut Processor) -> () {
        match self {
            EntityType::Malformed(r) => proc.write_range(r),
            EntityType::Ascii(c) => proc.write(c),
            EntityType::Named(s) => proc.write_slice(s),
            EntityType::Numeric(c) => proc.write_utf8(c),
        };
    }
}

macro_rules! handle_decoded_code_point {
    ($code_point:ident) => {
        std::char::from_u32($code_point).map(|c| if c.is_ascii() {
            EntityType::Ascii(c as u8)
        } else {
            EntityType::Numeric(c)
        })
    };
}

fn parse_decimal(proc: &mut Processor) -> Option<EntityType> {
    let mut val = 0u32;
    // Parse at most seven characters to prevent parsing forever and overflowing.
    // TODO Require at least one digit.
    for _ in 0..7 {
        if let Some(c) = chain!(proc.match_pred(is_digit).discard().maybe_char()) {
            val = val * 10 + (c - b'0') as u32;
        } else {
            break;
        }
    }
    handle_decoded_code_point!(val)
}

fn parse_hexadecimal(proc: &mut Processor) -> Option<EntityType> {
    let mut val = 0u32;
    // Parse at most six characters to prevent parsing forever and overflowing.
    // TODO Require at least one digit.
    for _ in 0..6 {
        if let Some(c) = chain!(proc.match_pred(is_hex_digit).discard().maybe_char()) {
            let digit = if is_digit(c) {
                c - b'0'
            } else if is_upper_hex_digit(c) {
                c - b'A' + 10
            } else if is_lower_hex_digit(c) {
                c - b'a' + 10
            } else {
                unreachable!();
            };
            val = val * 16 + digit as u32;
        } else {
            break;
        }
    }
    handle_decoded_code_point!(val)
}

fn parse_name(proc: &mut Processor) -> Option<EntityType> {
    // TODO Limit match length.
    let data = chain!(proc.match_while_pred(is_valid_entity_reference_name_char).discard().slice());
    // In UTF-8, one-byte character encodings are always ASCII.
    ENTITY_REFERENCES.get(data).map(|s| if s.len() == 1 {
        EntityType::Ascii(s[0])
    } else {
        EntityType::Named(s)
    })
}

// This will parse and skip characters. Set a checkpoint to later write skipped, or to ignore results and reset to previous position.
pub fn parse_entity(proc: &mut Processor) -> ProcessingResult<EntityType> {
    let checkpoint = proc.checkpoint();
    chain!(proc.match_char(b'&').expect().discard());

    // The input can end at any time after initial ampersand.
    // Examples of valid complete source code: "&", "&a", "&#", "&#09",
    // "&amp".

    // There are three stages to this function:
    //
    // 1. Determine the type of entity, so we can know how to parse and
    // validate the following characters.
    //    - This can be done by simply looking at the first and second
    //    characters after the initial ampersand, e.g. "&#", "&#x", "&a".
    // 2. Parse the entity data, i.e. the characters between the ampersand
    // and semicolon.
    //    - TODO To avoid parsing forever on malformed entities without
    //    semicolons, there is an upper bound on the amount of possible
    //    characters, based on the type of entity detected from the first
    //    stage.
    // 3. Interpret and validate the data.
    //    - This simply checks if it refers to a valid Unicode code point or
    //    entity reference name.

    // TODO Could optimise.
    // These functions do not return EntityType::Malformed as it requires a checkpoint.
    // Instead, they return None if entity is malformed.
    let entity_type = if chain!(proc.match_seq(b"#x").discard().matched()) {
        parse_hexadecimal(proc)
    } else if chain!(proc.match_char(b'#').discard().matched()) {
        parse_decimal(proc)
    } else if chain!(proc.match_pred(is_valid_entity_reference_name_char).matched()) {
        parse_name(proc)
    } else {
        // At this point, only consumed ampersand.
        None
    };

    Ok(if entity_type.is_some() && chain!(proc.match_char(b';').discard().matched()) {
        entity_type.unwrap()
    } else {
        EntityType::Malformed(proc.consumed_range(checkpoint))
    })
}

pub fn process_entity(proc: &mut Processor) -> ProcessingResult<EntityType> {
    let entity = parse_entity(proc)?;
    entity.keep(proc);
    Ok(entity)
}
