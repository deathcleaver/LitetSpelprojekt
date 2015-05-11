#ifndef LEGEND_H
#define LEGEND_H

enum OBJ
{
	BACK,
	WORLD,
	MISC,
	ENEMY,
};

enum BackID
{
	grid,
	rock_grey,
	rock_dirt,
	rock_boss,
	crypt_wall,

	back_count, //leave this last
};

enum WorldID
{
	box,
	mushroom,
	stalactite,
	Rock1x1,
	minecart,
	minetrack,
	minestruct_small,
	minestruct_large,
	lantern,
	platformRock1x1,
	platformRock1x3,
	platformRock1x5,
	platformRock3x3,
	platformRock5x5,
	Rock5x5,
	Rock5x5Crack,
	MushroomWall,
	Pickaxe,
	Board,
	CrateHD,
	Crate3D,
	platformMarble1x1,
	platformMarble1x3,
	platformMarble1x5,
	platformMarble3x3,
	platformMarble5x5,
	candle_big,
	candle_small,
	candelier,
	coffin_bottom,
	coffin_top,
	grave_circle,
	grave_classic,
	grave_cross,
	pillar_bottom_broken,
	pillar_top_broken,
	pillar,
	tunnel_broken,
	tunnel,
	redblock,
	marble_shrine,
	dead_tree,
	fence,
	gate,
	mirror,
	chair,
	table,
	bookshelf_empty,
	bookshef_full,
	book_red,
	book_blue,
	book_green,
	bookpile,
	brickwall,
	mansionwall,
	tornwall,
	mansionfloor,
	mansionflooredge,
	ghost_platform,
	world_count, //leave this last
};

enum MiscID
{
	shrine,
	rune_range,
	rune_damage,
	rune_shield,
	heart,

	misc_count, //leave this last
};

enum EnemyID
{
	spikes,
	bat,
	flame,
	cube,
	spider,
	ghost,
	web,
	spellbook,
	batboss,
	maexxna,
	demon,

	enemy_count, //leave this last
};

enum EffectType
{
	NONE,
	torch,
	spark,
	shield,
	lightning,
};

enum MusicID
{
	intro = 0, // witcher_dusk
	mine_area = 1, // witcher_cave
	boss_battle_bat = 2, // witcher_battle_bat
	mine_area_awesome = 3, // mine_track
	shroom_area = 4, // witcher_dike
	crypt_area = 5, // witcher_omnious
	boss_battle_spider = 6, // witcher_battle_spider
	ghost_area = 7, // witcher_dead_city
	boss_battle_ghost = 8 // witcher_battle_ghost
};

enum SoundID
{
	// shrine
	rune_recieved = 0,
	player_healed = 1,
	player_saved = 51,
	// player
	player_resurrected = 2,
	player_attack_miss = 3,
	player_attack_fire = 4,
	player_attack_ice = 5,
	player_shield_force = 6,
	player_landing = 7,
	player_hurt = 26,
	// interface
	interface_button = 8,
	interface_pause = 9,
	// bosses
	boss_clear = 10,
	boss_bat_attack = 11,
	boss_bat_hurt = 12,
	boss_bat_death = 13,
	boss_spider_spawn = 46,
	boss_spider_attack = 47,
	boss_spider_hurt = 48,
	boss_spider_death = 49,
	boss_spider_webshot = 50,
	boss_ghost_laugh = 52,
	boss_ghost_hurt = 53,
	boss_ghost_death = 54,
	// enemies
	enemy_slime_jump = 14,
	enemy_slime_hurt = 15,
	enemy_slime_death = 16,
	enemy_flame_hurt = 17,
	enemy_flame_death = 18,
	enemy_ghost_hurt = 20,
	enemy_ghost_death = 21,
	enemy_ghost_moan = 22,
	enemy_spider_jump = 23,
	enemy_spider_hurt = 24,
	enemy_spider_death = 25,
	enemy_tome_spellcast = 19,
	enemy_tome_hurt = 55,
	enemy_tome_death = 56,
	enemy_missile_death = 57,
	// items
	item_hearth_piece = 27,
	item_hearth_completed = 28,
	// ambient
	ambient_water_drop = 29,
	ambient_rain = 30,
	ambient_thunder2 = 31,
	ambient_crow = 32,
	ambient_everyone = 33,
	ambient_frostmournce_chant = 34,
	ambient_mining = 35,
	ambient_owl = 36,
	ambient_bats = 37,
	ambient_water_splash = 38
};

#endif