# innerspace

A fast one-pass in-place HTML minifier written in Rust with advanced whitespace handling.

## Features

- Minification is done in one pass with no backtracking or DOM/AST building.
- No extra heap memory is allocated during processing, which increases performance.
- Context-aware whitespace handling allows maximum minification while retaining wanted spaces.

## Usage

```bash
innerspace --src /path/to/src.html --out /path/to/output.min.html
```

## Minification

### Whitespace

innerspace has advanced context-aware whitespace minification that does things such as:

- Leave whitespace untouched in `pre` and `code`, which are whitespace sensitive.
- Trim and collapse whitespace in content tags, as whitespace is collapsed anyway when rendered.
- Remove whitespace in layout tags, which allows the use of inline layouts while keeping formatted code.

#### Methods

There are three whitespace minification methods. When processing text content, innerspace chooses which ones to use depending on the containing element.

<details>
<summary><h5>Collapse whitespace</h5></summary>

> **Applies to:** text in root and any element except [whitespace sensitive](./src/spec/tag/wss.rs) elements.

Reduce a sequence of whitespace characters in text nodes to a single space (U+0020).

<table><thead><tr><th>Before<th>After<tbody><tr><td>

```html
<p>↵
··The·quick·brown·fox↵
··jumps·over·the·lazy↵
··dog.↵
</p>
```

<td>

```html
<p>·The·quick·brown·fox·jumps·over·the·lazy·dog.·</p>
```

</table>
</details>

<details>
<summary><h5>Destroy whole whitespace</h5></summary>

> **Applies to:** text in root and any element except [whitespace sensitive](./src/spec/tag/wss.rs), [content](./src/spec/tag/content.rs), [content-first](./src/spec/tag/contentfirst.rs), and [formatting](./src/spec/tag/formatting.rs) elements.

Remove any text nodes that only consist of whitespace characters.

<table><thead><tr><th>Before<th>After<tbody><tr><td>

```html
<ul>↵
··<li>A</li>↵
··<li>B</li>↵
··<li>C</li>↵
</ul>
```

<td>

```html
<ul>↵
··<li>A</li><li>B</li><li>C</li>↵
</ul>
```

</table>
</details>

<details>
<summary><h5>Trim whitespace</h5></summary>

> **Applies to:** text in root and any element except [whitespace sensitive](./src/spec/tag/wss.rs) and [formatting](./src/spec/tag/formatting.rs) elements.

Remove any leading/trailing whitespace from any leading/trailing text nodes of a tag.

<table><thead><tr><th>Before<th>After<tbody><tr><td>

```html
<p>↵
··Hey,·I·<em>just</em>·found↵
··out·about·this·<strong>cool</strong>·website!↵
··<sup>[1]</sup>↵
</p>
```

<td>

```html
<p>Hey,·I·<em>just</em>·found↵
··out·about·this·<strong>cool</strong>·website!↵
··<sup>[1]</sup></p>
```

</table>
</details>

#### Element types

innerspace groups elements based on how it assumes they are used. By making these assumptions, it can apply optimal whitespace minification strategies.

|Group|Elements|Expected children|
|---|---|---|
|[Formatting](#formatting-elements)|`a`, `strong`, [and others](./src/spec/tag/formatting.rs)|Formatting elements, text.|
|[Content](#content-elements)|`h1`, `p`, [and others](./src/spec/tag/content.rs)|Formatting elements, text.|
|[Layout](#layout-elements)|`div`, `ul`, [and others](./src/spec/tag/layout.rs)|Layout elements, content elements.|
|[Content-first](#content-first-elements)|`label`, `li`, [and others](./src/spec/tag/contentfirst.rs)|Like content element but could have exactly one of an layout element's expected content elements.|

<details>
<summary><h5>Formatting elements</h5></summary>

> Whitespace is collapsed.
Formatting elements are usually inline elements that wrap around part of some text in a content element, so its whitespace isn't trimmed as they're probably part of the content.

</details>

<details>
<summary><h5>Content elements</h5></summary>

> Whitespace is trimmed and collapsed.
Content elements usually represent a contiguous and complete unit of content such as a paragraph. As such, whitespace is significant but sequences of them are most likely due to formatting.

###### Before

```html
<p>↵
··Hey,·I·<em>just</em>·found↵
··out·about·this·<strong>cool</strong>·website!↵
··<sup>[1]</sup>↵
</p>
```

###### After

```html
<p>Hey,·I·<em>just</em>·found·out·about·this·<strong>cool</strong>·website!·<sup>[1]</sup></p>
```

</details>

<details>
<summary><h5>Layout elements</h5></summary>

> Whitespace is trimmed and collapsed. [Whole whitespace](#destroy-whole-whitespace) is removed.
These elements should only contain other elements and no text. This makes it possible to [remove whole whitespace](#destroy-whole-whitespace), which is useful when using `display: inline-block` so that whitespace between elements (e.g. indentation) does not alter layout and styling.

###### Before

```html
<ul>↵
··<li>A</li>↵
··<li>B</li>↵
··<li>C</li>↵
</ul>
```

###### After

```html
<ul><li>A</li><li>B</li><li>C</li></ul>
```

</details>

<details>
<summary><h5>Content-first elements</h5></summary>

> Whitespace is trimmed and collapsed.
These elements are usually like [content elements](#content-elements) but are occasionally used like a layout element with one child. Whole whitespace is not removed as it might contain content, but this is OK for using as layout as there is only one child and whitespace is trimmed.

###### Before

```html
<li>↵
··<article>↵
····<section></section>↵
····<section></section>↵
··</article>↵
</li>
```

###### After

```html
<li><article><section></section><section></section></article></li>
```

</details>

### Attributes

Any entities in attribute values are decoded, and then the most optimal representation is calculated and used:

- Double quoted, with any `"` encoded.
- Single quoted, with any `'` encoded.
- Unquoted, with `"`/`'` first char (if applicable) and `>` last char (if applicable), and any whitespace, encoded.

Some attributes have their whitespace (after decoding) trimmed and collapsed, such as `class`.

If the attribute value is empty after any processing, it is completely removed (i.e. no `=`).

Spaces are removed between attributes if possible.

### Other

- Comments are removed.
- Entities are decoded if valid (see relevant parsing section).

### Ignored

### Explicitly important

Empty elements and bangs are not removed as it is assumed there is a special reason for their declaration.

## Parsing

Only UTF-8/ASCII is supported.

innerspace is an HTML minifier and simply does HTML minification. innerspace almost does no syntax checking or standards enforcement for performance and code complexity reasons.

For example, this means that it's not an error to have self-closing tags, declare multiple `<body>` elements, use incorrect attribute names and values, or write something like `<br>alert('');</br>`

However, there are some syntax requirements for speed and sanity reasons.

### Tags

Tag names are case sensitive.

### Entities

Well-formed entities are decoded, including in attribute values. 
 
They are interpreted as characters representing their decoded value. This means that `&#9;` is considered a whitespace character and could be minified.

Malformed entities are interpreted literally as a sequence of characters.

If a named entity is an invalid reference as per the [spec](https://html.spec.whatwg.org/multipage/named-characters.html#named-character-references), it is considered malformed.

Numeric character references that do not reference a valid [Unicode Scalar Value](https://www.unicode.org/glossary/#unicode_scalar_value) are considered malformed.

### Attributes

Backticks (`` ` ``) are not valid quote marks and are not interpreted as such.
However, backticks are valid attribute value quotes in Internet Explorer.

It is an error to place whitespace between `=` and attribute names/values. It is also an error if there is no whitespace before an attribute. For example:

```html
<div class="a"name="1" id = "a"></div>
```

Special handling of some attributes require case sensitive names and values. For example, `class` and `type="text/javascript"`.

### Script and style

`script` and `style` tags must be closed with `</script>` and `</style>` respectively (case sensitive).

Note that the closing tag must not contain any whitespace (e.g. `</script  >`).

[innerspace can handle text script content.](./notes/Text%20script%20content.md)

## Development

### More minification options

- Removal of [optional tags](https://html.spec.whatwg.org/multipage/syntax.html#syntax-tag-omission).
- Removal of boolean attribute values.
- Removal of redundant attributes (empty or default value).
- Handling of conditional or special comments.