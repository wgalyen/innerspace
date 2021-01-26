#[derive(Debug)]
pub enum InsErr {
    ExpectedCharNotFound { expected: u8, got: u8 },
    ExpectedMatchNotFound(&'static [u8]),
    ExpectedNotFound(&'static str),
    NoSpaceBeforeAttr,
    UnclosedTag,
    UnexpectedCharFound(u8),
    UnexpectedEnd,
}

pub type InsRes<T> = Result<T, InsErr>;
