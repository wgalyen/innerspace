use ::phf::{phf_set, Set};

// Sourced from https://developer.mozilla.org/en-US/docs/Web/SVG/Element at 2018-08-04T03:50:00Z.
pub static SVG_TAGS: Set<&'static [u8]> = phf_set! {
	b"a",
	b"altGlyph",
	b"altGlyphDef",
	b"altGlyphItem",
	b"animate",
	b"animateColor",
	b"animateMotion",
	b"animateTransform",
	b"circle",
	b"clipPath",
	b"color-profile",
	b"cursor",
	b"defs",
	b"desc",
	b"discard",
	b"ellipse",
	b"feBlend",
	b"feColorMatrix",
	b"feComponentTransfer",
	b"feComposite",
	b"feConvolveMatrix",
	b"feDiffuseLighting",
	b"feDisplacementMap",
	b"feDistantLight",
	b"feDropShadow",
	b"feFlood",
	b"feFuncA",
	b"feFuncB",
	b"feFuncG",
	b"feFuncR",
	b"feGaussianBlur",
	b"feImage",
	b"feMerge",
	b"feMergeNode",
	b"feMorphology",
	b"feOffset",
	b"fePointLight",
	b"feSpecularLighting",
	b"feSpotLight",
	b"feTile",
	b"feTurbulence",
	b"filter",
	b"font-face-format",
	b"font-face-name",
	b"font-face-src",
	b"font-face-uri",
	b"font-face",
	b"font",
	b"foreignObject",
	b"g",
	b"glyph",
	b"glyphRef",
	b"hatch",
	b"hatchpath",
	b"hkern",
	b"image",
	b"line",
	b"linearGradient",
	b"marker",
	b"mask",
	b"mesh",
	b"meshgradient",
	b"meshpatch",
	b"meshrow",
	b"metadata",
	b"missing-glyph",
	b"mpath",
	b"path",
	b"pattern",
	b"polygon",
	b"polyline",
	b"radialGradient",
	b"rect",
	b"script",
	b"set",
	b"solidcolor",
	b"stop",
	b"style",
	b"svg",
	b"switch",
	b"symbol",
	b"text",
	b"textPath",
	b"title",
	b"tref",
	b"tspan",
	b"unknown",
	b"use",
	b"view",
	b"vkern",
};
