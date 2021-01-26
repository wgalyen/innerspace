#include <is/collection.h>
#include <is/rule.h>
#include <is/rune.h>

static ins_map_tag_relations* ins_rule_tag_child_blacklist_map;

void ins_rule_tag_child_blacklist_map_add_entries(ins_map_tag_relations* map)
{
	// <address>
	ins_set_tag_names* address = ins_set_tag_names_create();
	ins_rule_tag_heading_add_elems(address);
	ins_rule_tag_sectioning_add_elems(address);
	ins_set_tag_names_add_whole_literal(address, "address");
	ins_set_tag_names_add_whole_literal(address, "header");
	ins_set_tag_names_add_whole_literal(address, "footer");
	ins_map_tag_relations_set_whole_literal(map, "address", address);

	// <audio>
	ins_set_tag_names* audio = ins_set_tag_names_create();
	ins_rule_tag_media_add_elems(audio);
	ins_map_tag_relations_set_whole_literal(map, "audio", audio);

	// <dfn>
	ins_set_tag_names* dfn = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(dfn, "dfn");
	ins_map_tag_relations_set_whole_literal(map, "dfn", dfn);

	// <dt>
	ins_set_tag_names* dt = ins_set_tag_names_create();
	ins_rule_tag_heading_add_elems(dt);
	ins_rule_tag_sectioning_add_elems(dt);
	ins_set_tag_names_add_whole_literal(dt, "header");
	ins_set_tag_names_add_whole_literal(dt, "footer");
	ins_map_tag_relations_set_whole_literal(map, "dt", dt);

	// <footer>
	ins_set_tag_names* footer = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(footer, "header");
	ins_set_tag_names_add_whole_literal(footer, "footer");
	ins_map_tag_relations_set_whole_literal(map, "footer", footer);

	// <form>
	ins_set_tag_names* form = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(form, "form");
	ins_map_tag_relations_set_whole_literal(map, "form", form);

	// <header>
	ins_set_tag_names* header = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(header, "header");
	ins_set_tag_names_add_whole_literal(header, "footer");
	ins_map_tag_relations_set_whole_literal(map, "header", header);

	// <label>
	ins_set_tag_names* label = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(label, "label");
	ins_map_tag_relations_set_whole_literal(map, "label", label);

	// <progress>
	ins_set_tag_names* progress = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(progress, "progress");
	ins_map_tag_relations_set_whole_literal(map, "progress", progress);

	// <th>
	ins_set_tag_names* th = ins_set_tag_names_create();
	ins_rule_tag_heading_add_elems(th);
	ins_rule_tag_sectioning_add_elems(th);
	ins_set_tag_names_add_whole_literal(th, "header");
	ins_set_tag_names_add_whole_literal(th, "footer");
	ins_map_tag_relations_set_whole_literal(map, "th", th);

	// <video>
	ins_set_tag_names* video = ins_set_tag_names_create();
	ins_rule_tag_media_add_elems(video);
	ins_map_tag_relations_set_whole_literal(map, "video", video);
}

void ins_rule_tag_child_blacklist_init(void)
{
	ins_rule_tag_child_blacklist_map = ins_map_tag_relations_create();
	ins_rule_tag_child_blacklist_map_add_entries(
		ins_rule_tag_child_blacklist_map);
}

// Check if a parent is allowed to have a specific child, based on the
// blacklist.
bool ins_rule_tag_child_blacklist_allowed(nh_view_str* parent,
					 nh_view_str* child)
{
	ins_set_tag_names* set = ins_map_tag_relations_get(
		ins_rule_tag_child_blacklist_map, parent);
	return set == NULL || !ins_set_tag_names_has(set, child);
}
