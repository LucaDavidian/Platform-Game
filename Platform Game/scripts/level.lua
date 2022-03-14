level = {

	assets = {

		sounds = {

			land = 'land2.wav'

		}

	},  -- assets

	entities = {

		room = {
		
			components = {
		
				transform = {
					position = {0.0, 0.0, 0.0},	
					rotation = 0.0,		
					scale = { 3.0, 3.0, 1.0 }
				},
		
				sprite = {
					file_name = 'forest.jpg',
					num_rows = 1,
					num_cols = 1,
					--z_order = 0
				}
		
			}
		
		},

		room2 = {
		
			components = {
		
				transform = {
					position = {3.0, 0.0, 0.0},	
					rotation = 0.0,		
					scale = { 3.0, 3.0, 1.0 }
				},
		
				sprite = {
					file_name = 'forest.jpg',
					num_rows = 1,
					num_cols = 1,
					--z_order = 0
				}
		
			}
		
		},

		room3 = {
		
			components = {
		
				transform = {
					position = {-3.0, 0.0, 0.0},	
					rotation = 0.0,		
					scale = { 3.0, 3.0, 1.0 }
				},
		
				sprite = {
					file_name = 'forest.jpg',
					num_rows = 1,
					num_cols = 1,
					--z_order = 0
				}
		
			}
		
		},

		player = {
			
			components = {
		
				tag = { 'player' },

				transform = {
					position = {0.5, 0.5, 0.0},	
					rotation = 0.0,		
					scale = { 0.7, 0.4, 0.7 }
				},
			
				camera_ortho = {
					aspect_ratio = 3.0 / 2.0, 
					height = 2.0, 
					near_plane = -0.1, 
					far_plane = 1.0,
					offset = { 0.0, 0.0, 0.2 }
				},
				
				player_input = {
					
				},
				
				--sprite = {
				--	file_name = 'knight.png',
				--	num_rows = 7,
				--	num_cols = 22,
				--	z_order = 2
				--},

				sprite = {
					file_name = 'player.png',
					num_rows = 15,
					num_cols = 12,
					z_order = 2
				},
				
				--sprite_animation = {
				--	animations = {
				--		stand = { {0, 0.08}, {1, 0.08}, {2, 0.08}, {3, 0.08}, {4, 0.08}, {5, 0.08}, {6, 0.08}, {7, 0.08}, {8, 0.08}, {9, 0.08}, {10, 0.08}, {11, 0.08}, {12, 0.08}, {13, 0.08}, {14, 0.08} },
				--		run = { {22, 0.1}, {23, 0.1}, {24, 0.1}, {25, 0.1}, {26, 0.1}, {27, 0.1}, {28, 0.1}, {29, 0.1} },
				--		attack = { {66, 0.06}, {67, 0.06}, {68, 0.06}, {69, 0.06}, {70, 0.06}, {71, 0.06}, {72, 0.06}, {73, 0.06}, {74, 0.06}, {75, 0.06}, {76, 0.06}, {77, 0.06}, {78, 0.06}, {79, 0.06}, {80, 0.06},  {81, 0.06}, {82, 0.06}, {83, 0.06}, {84, 0.06}, {85, 0.06}, {86, 0.06}, {87, 0.06} },
				--		fall = { {89, 0.08} },
				--		jump = { {110, 0.08}, {111, 0.08}, {112, 0.08}, {113, 0.08}, {114, 0.08}, {115, 0.08}, {116, 0.08}, {117, 0.08}, {118, 0.08} },
				--		land = { {132, 0.08}, {133, 0.08}, {134, 0.08}, {135, 0.08}, {136, 0.08} },
				--		hit = { {66, 1.0 }, {66, 0.06}, {66, 0.06} }
				--	},
				--},

				sprite_animation = {
					animations = {
						idle = { {0, 0.1}, {1, 0.1}, {2, 0.1}, {3, 0.4} },
						idle_crouch = { {108, 0.1}, {109, 0.1}, {110, 0.1}, {111, 0.5} },
						move = { {72, 0.1}, {73, 0.1}, {74, 0.1}, {75, 0.1}, {76, 0.1}, {77, 0.1}, {78, 0.1}, {79, 0.1}, {80, 0.1}, {81, 0.1}, {82, 0.1}, {83, 0.1} },
						move_crouch = { {120, 0.1 }, {121, 0.1}, {122, 0.1}, {123, 0.1 }, {124, 0.1}, {125, 0.1}, {126, 0.1 }, {127, 0.1}, {128, 0.1}, {129, 0.1 }, {130, 0.1}, {131, 0.1} },
						jump = { {24, 0.08}, {25, 0.08}, {26, 0.08} },
						fall = { {36, 0.08}, {37, 0.08}, {38, 0.08} },
						land = { {48, 0.08}, {49, 0.08}, {50, 0.08} },
						attack1 = { {132, 0.1}, {133, 0.1}, {134, 0.1}, {135, 0.1}, {136, 0.1} },
						attack2 = { {144, 0.1}, {145, 0.1}, {146, 0.1}, {147, 0.1}, {148, 0.1} },
						attack3 = { {156, 0.1}, {157, 0.1}, {158, 0.1}, {159, 0.1}, {160, 0.1} },
					},
				},
				
				--player_fsm = {
				--	run_speed = 1.0,
				--	jump_speed = 1.0,
				--	jump_airborne_speed = 1.0,
				--},

				physics = {
					mass = 1.0,
				},

				collision = {
					offset = { 0.0, -0.01 },
					dimensions = { 0.2, 0.4 }
				},

				lives = { 3 }, 

				FSM = {
					states = { 'idle', 'move' },
					initial_state = 'idle'
				},
				
			}
		},

		bat = {

			components = {

				tag = { 'enemy' },

				transform = {
					position = {-0.3, 0.4, 0.0},	
					rotation = 0.0,		
					scale = { 0.3, 0.3, 1.0 }
				},
				
				sprite = {
					file_name = 'bat.png',
					num_rows = 3,
					num_cols = 10,
					z_order = 2
				},

				sprite_animation = {
					animations = {
						flight = { {20, 0.08}, {21, 0.08}, {22, 0.08}, {23, 0.08}, {24, 0.08}, {25, 0.08}, {26, 0.08}, {27, 0.08} },
						attack = { {0, 0.08}, {1, 0.08}, {2, 0.08}, {3, 0.08}, {4, 0.08}, {5, 0.08}, {6, 0.08}, {7, 0.08}, {8, 0.08}, {9, 0.08} },
						death = { {10, 0.08}, {11, 0.08}, {12, 0.08}, {13, 0.08}, {14, 0.08}, {15, 0.08}, {16, 0.08}, {17, 0.08}, {18, 0.08}, {19, 0.08} },
					},

					--initial_animation = 'flight'
				},

				collision = {
					offset = { -0.015, 0.0 },
					dimensions = { 0.55, 0.3 }
				},

				enemy_fsm = {}

			}

		},

		platform1 = {
		
			components = {
				
				tag = { 'platform'},
		
				transform = {
					position = {-0.8, -0.7, 0.0},	
					rotation = 0.0,		
					scale = { 1.0, 0.1, 1.0 }
				},
		
				sprite = {
					file_name = 'platform.png',
					num_rows = 1,
					num_cols = 1,
					z_order = 2
				},
		
				collision = {
		
				}
		
			}
		
		},

		platform2 = {
		
			components = {
				
				tag = { 'platform'},
		
				transform = {
					position = {0.0, -0.5, 0.0},	
					rotation = 0.0,		
					scale = { 1.5, 0.1, 1.0 }
				},
		
				sprite = {
					file_name = 'platform.png',
					num_rows = 1,
					num_cols = 1,
					z_order = 2
				},
		
				collision = {
		
				}
		
			}
		
		},

		moving_platform1 = {

			components = {

				tag = { 'platform'},

				transform = {
					position = { 0.5, 0.0, 0.0 },	
					rotation = 0.0,		
					scale = { 0.4, 0.1, 1.0 }
				},
	
				sprite = {
					file_name = 'platform.png',
					num_rows = 1,
					num_cols = 1,
					z_order = 2
				},
	
				collision = {
	
				},

				move_to = {
					{ 0.5, -0.2}, {0.5, 0.3},  { -0.2, 0.0 }
				},

				physics = {
					
				}

			}

		},

		moving_platform2 = {

			components = {

				tag = { 'platform'},

				transform = {
					position = { 1.2, -0.3, 0.0 },	
					rotation = 0.0,		
					scale = { 0.4, 0.1, 1.0 }
				},
	
				sprite = {
					file_name = 'platform.png',
					num_rows = 1,
					num_cols = 1,
					z_order = 2
				},
	
				collision = {
	
				},

				physics = {
					
				},

				move_to = {
					{ 1.1, 0.0 }, {1.6, 0.0} 
				},

			}

		},

		--moving_platform3 = {
		--
		--	components = {
		--
		--		tag = { 'platform'},
		--
		--		transform = {
		--			position = { -0.5, -0.4, 0.0 },	
		--			rotation = 0.0,		
		--			scale = { 0.3, 0.1, 1.0 }
		--		},
		--
		--		sprite = {
		--			file_name = 'platform.png',
		--			num_rows = 1,
		--			num_cols = 1,
		--			z_order = 2
		--		},
		--
		--		collision = {
		--
		--		},
		--
		--		physics = {
		--			
		--		},
		--
		--		move_to = {
		--			{ -0.7, -0.4 }, {0.0, -0.4} 
		--		},
		--
		--	}
		--
		--},

		moving_platform4 = {

			components = {

				tag = { 'platform'},

				transform = {
					position = { -1.3, -0.6, 0.0 },	
					rotation = 0.0,		
					scale = { 0.3, 0.1, 1.0 }
				},
	
				sprite = {
					file_name = 'platform.png',
					num_rows = 1,
					num_cols = 1,
					z_order = 2
				},
	
				collision = {
	
				},

				physics = {
					
				},

				move_to = {
					{ -1.3, -0.6 }, {-1.3, 0.2} 
				},

			}

		},

		wall1 = {

			components = {

				tag = { 'wall' },

				transform = {
					position = { 0.8, -0.1, 0.0 },	
					rotation = 90.0,		
					scale = { 1.0, 0.1, 1.0 }
				},
				
				sprite = {
					file_name = 'platform.png',
					num_rows = 1,
					num_cols = 1,
					z_order = 2
				},
	
				collision = {
	
				}

			}

		}

	},  -- entities

	GUI = {
	
		text = { type = 'text', 'lives', font = 'super legend boy', size = 25 },

		panel = { 
			
		}

	}  -- GUI

}  -- level