#include <ins/collection.h>
#include <ins/rule.h>
#include <ins/rune.h>

void ins_rule_tag_parent_blacklist_init(void)
{
	// Don't do anything. This rule is currently unused.
}

// Check if a child is allowed to have a specific parent, based on the
// blacklist.
bool ins_rule_tag_parent_blacklist_allowed(nh_view_str* child,
					  nh_view_str* parent)
{
	// Since this rule is currently not being used, directly allow without
	// any checks.
	(void) child;
	(void) parent;
	return true;
}
