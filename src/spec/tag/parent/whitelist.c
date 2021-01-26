#include <ins/collection.h>
#include <ins/rule.h>
#include <ins/rune.h>

static ins_map_tag_relations* ins_rule_tag_parent_whitelist_map;

void ins_rule_tag_parent_whitelist_map_add_entries(ins_map_tag_relations* map)
{
	// <caption>
	ins_set_tag_names* caption = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(caption, "table");
	ins_map_tag_relations_set_whole_literal(map, "caption", caption);

	// <col>
	ins_set_tag_names* col = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(col, "table");
	ins_set_tag_names_add_whole_literal(col, "colgroup");
	ins_map_tag_relations_set_whole_literal(map, "col", col);

	// <colgroup>
	ins_set_tag_names* colgroup = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(colgroup, "table");
	ins_map_tag_relations_set_whole_literal(map, "colgroup", colgroup);

	// <dd>
	ins_set_tag_names* dd = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(dd, "dl");
	ins_map_tag_relations_set_whole_literal(map, "dd", dd);

	// <dt>
	ins_set_tag_names* dt = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(dt, "dl");
	ins_map_tag_relations_set_whole_literal(map, "dt", dt);

	// <figcaption>
	ins_set_tag_names* figcaption = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(figcaption, "figure");
	ins_map_tag_relations_set_whole_literal(map, "figcaption", figcaption);

	// <legend>
	ins_set_tag_names* legend = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(legend, "fieldset");
	ins_map_tag_relations_set_whole_literal(map, "legend", legend);

	// <li>
	ins_set_tag_names* li = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(li, "ul");
	ins_set_tag_names_add_whole_literal(li, "ol");
	ins_set_tag_names_add_whole_literal(li, "menu");
	ins_map_tag_relations_set_whole_literal(map, "li", li);

	// <optgroup>
	ins_set_tag_names* optgroup = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(optgroup, "select");
	ins_map_tag_relations_set_whole_literal(map, "optgroup", optgroup);

	// <option>
	ins_set_tag_names* option = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(option, "select");
	ins_set_tag_names_add_whole_literal(option, "optgroup");
	ins_set_tag_names_add_whole_literal(option, "datalist");
	ins_map_tag_relations_set_whole_literal(map, "option", option);

	// <param>
	ins_set_tag_names* param = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(param, "object");
	ins_map_tag_relations_set_whole_literal(map, "param", param);

	// <rp>
	ins_set_tag_names* rp = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(rp, "ruby");
	ins_map_tag_relations_set_whole_literal(map, "rp", rp);

	// <rt>
	ins_set_tag_names* rt = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(rt, "ruby");
	ins_map_tag_relations_set_whole_literal(map, "rt", rt);

	// <rtc>
	ins_set_tag_names* rtc = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(rtc, "ruby");
	ins_map_tag_relations_set_whole_literal(map, "rtc", rtc);

	// <summary>
	ins_set_tag_names* summary = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(summary, "details");
	ins_map_tag_relations_set_whole_literal(map, "summary", summary);

	// <source>
	ins_set_tag_names* source = ins_set_tag_names_create();
	ins_rule_tag_media_add_elems(source);
	ins_set_tag_names_add_whole_literal(source, "picture");
	ins_map_tag_relations_set_whole_literal(map, "source", source);

	// <track>
	ins_set_tag_names* track = ins_set_tag_names_create();
	ins_rule_tag_media_add_elems(track);
	ins_map_tag_relations_set_whole_literal(map, "track", track);

	// <tbody>
	ins_set_tag_names* tbody = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(tbody, "table");
	ins_map_tag_relations_set_whole_literal(map, "tbody", tbody);

	// <td>
	ins_set_tag_names* td = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(td, "tr");
	ins_map_tag_relations_set_whole_literal(map, "td", td);

	// <tfoot>
	ins_set_tag_names* tfoot = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(tfoot, "table");
	ins_map_tag_relations_set_whole_literal(map, "tfoot", tfoot);

	// <th>
	ins_set_tag_names* th = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(th, "tr");
	ins_map_tag_relations_set_whole_literal(map, "th", th);

	// <thead>
	ins_set_tag_names* thead = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(thead, "table");
	ins_map_tag_relations_set_whole_literal(map, "thead", thead);

	// <tr>
	ins_set_tag_names* tr = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(tr, "table");
	ins_set_tag_names_add_whole_literal(tr, "thead");
	ins_set_tag_names_add_whole_literal(tr, "tbody");
	ins_set_tag_names_add_whole_literal(tr, "tfoot");
	ins_map_tag_relations_set_whole_literal(map, "tr", tr);

	// <template>
	// Should be <body>, <frameset>, <head>, <dl>, <colgroup>, but ignoring.
}

void ins_rule_tag_parent_whitelist_init(void)
{
	ins_rule_tag_parent_whitelist_map = ins_map_tag_relations_create();
	ins_rule_tag_parent_whitelist_map_add_entries(
		ins_rule_tag_parent_whitelist_map);
}

// Check if a child is allowed to have a specific parent.
bool ins_rule_tag_parent_whitelist_allowed(nh_view_str* child,
					  nh_view_str* parent)
{
	ins_set_tag_names* set = ins_map_tag_relations_get(
		ins_rule_tag_parent_whitelist_map, child);
	return set == NULL || ins_set_tag_names_has(set, parent);
}
