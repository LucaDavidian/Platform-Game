menu = {
	
	entities = {
	
		camera = {

			components = {
				
				transform = {
					position = {0.0, 0.0, 0.0},	
					rotation = 0.0,		
					scale = { 1.0, 1.0, 1.0 }
				},

				camera_ortho = {
					aspect_ratio = 3.0 / 2.0, 
					height = 2.0, 
					near_plane = -0.1, 
					far_plane = 1.0,
					offset = { 0.0, 0.0, 0.2 }
				},

			}

		}

	}, -- entities

	GUI = {

		panel = {
		
			type = 'panel',


			title = {
				type = 'label',

				text = 'YaPNC adventure',
				font = 'arial',
				size = 40,
				anchor_parent = 'center',
				anchor = 'center',
				rel_position_x = 0.0,
				rel_position_y = 0.3,
				width = 0.5,
				height = 0.2
			},

			editor_button = {
				type = 'button',

			},

			play_button = {
				type = 'button',

			}

		}

	} -- GUI

}  -- menu


