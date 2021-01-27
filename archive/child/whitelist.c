#include <ins/collection.h>
#include <ins/rule.h>
#include <ins/rune.h>

static ins_map_tag_relations* ins_rule_tag_child_whitelist_map;

void ins_rule_tag_child_whitelist_map_add_entries(ins_map_tag_relations* map)
{
	// <colgroup>
	ins_set_tag_names* colgroup = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(colgroup, "col");
	ins_map_tag_relations_set_whole_literal(map, "colgroup", colgroup);

	// <datalist>
	ins_set_tag_names* datalist = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(datalist, "option");
	ins_map_tag_relations_set_whole_literal(map, "datalist", datalist);

	// <dl>
	ins_set_tag_names* dl = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(dl, "dt");
	ins_set_tag_names_add_whole_literal(dl, "dd");
	ins_map_tag_relations_set_whole_literal(map, "dl", dl);

	// <hgroup>
	ins_set_tag_names* hgroup = ins_set_tag_names_create();
	ins_rule_tag_heading_add_elems(hgroup);
	ins_map_tag_relations_set_whole_literal(map, "hgroup", hgroup);

	// <ol>
	ins_set_tag_names* ol = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(ol, "li");
	ins_map_tag_relations_set_whole_literal(map, "ol", ol);

	// <optgroup>
	ins_set_tag_names* optgroup = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(optgroup, "option");
	ins_map_tag_relations_set_whole_literal(map, "optgroup", optgroup);

	// <picture>
	ins_set_tag_names* picture = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(picture, "source");
	ins_set_tag_names_add_whole_literal(picture, "img");
	ins_map_tag_relations_set_whole_literal(map, "picture", picture);

	// <select>
	ins_set_tag_names* select = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(select, "optgroup");
	ins_set_tag_names_add_whole_literal(select, "option");
	ins_map_tag_relations_set_whole_literal(map, "select", select);

	// <table>
	ins_set_tag_names* table = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(table, "caption");
	ins_set_tag_names_add_whole_literal(table, "colgroup");
	ins_set_tag_names_add_whole_literal(table, "col");
	ins_set_tag_names_add_whole_literal(table, "thead");
	ins_set_tag_names_add_whole_literal(table, "tbody");
	ins_set_tag_names_add_whole_literal(table, "tfoot");
	ins_set_tag_names_add_whole_literal(table, "tr");
	ins_map_tag_relations_set_whole_literal(map, "table", table);

	// <tbody>
	ins_set_tag_names* tbody = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(tbody, "tr");
	ins_map_tag_relations_set_whole_literal(map, "tbody", tbody);

	// <tfoot>
	ins_set_tag_names* tfoot = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(tfoot, "tr");
	ins_map_tag_relations_set_whole_literal(map, "tfoot", tfoot);

	// <thead>
	ins_set_tag_names* thead = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(thead, "tr");
	ins_map_tag_relations_set_whole_literal(map, "thead", thead);

	// <tr>
	ins_set_tag_names* tr = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(tr, "td");
	ins_set_tag_names_add_whole_literal(tr, "th");
	ins_set_tag_names_add_whole_literal(tr, "template");
	ins_set_tag_names_add_whole_literal(tr, "script");
	ins_map_tag_relations_set_whole_literal(map, "tr", tr);

	// <ul>
	ins_set_tag_names* ul = ins_set_tag_names_create();
	ins_set_tag_names_add_whole_literal(ul, "li");
	ins_map_tag_relations_set_whole_literal(map, "ul", ul);
}

void ins_rule_tag_child_whitelist_init(void)
{
	ins_rule_tag_child_whitelist_map = ins_map_tag_relations_create();
	ins_rule_tag_child_whitelist_map_add_entries(
		ins_rule_tag_child_whitelist_map);
}

// Check if a parent is allowed to have a specific child.
bool ins_rule_tag_child_whitelist_allowed(nh_view_str* parent,
					 nh_view_str* child)
{
	ins_set_tag_names* set = ins_map_tag_relations_get(
		ins_rule_tag_child_whitelist_map, parent);
	return set == NULL || ins_set_tag_names_has(set, child);
}
