#pragma once

struct renderable_t;

struct scene_node_t {
	explicit scene_node_t( renderable_t* mesh )  : Content( mesh ),
								                                                Hashcode( 0 ) {
		Flags = {
			false,
		};
	}

	~scene_node_t() {
		Content->~renderable_t();
		RENDER_ALLOCATED_FREE( Content );

		for( scene_node_t* child : Children ) {
			SAFE_DELETE( child )
		}

		Children.clear();
		Hashcode = 0;
	}

	struct {
		bool IsAlwaysVisible; // Should be set for special entities (e.g. editor grid, trigger volumes, ...)
	} Flags;

	std::vector<scene_node_t*> Children;
	u32                        Hashcode;
	renderable_t*              Content;
};
