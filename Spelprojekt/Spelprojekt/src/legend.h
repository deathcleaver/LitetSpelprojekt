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

	world_count, //leave this last
};

enum MiscID
{
	shrine,
	rune_range,
	rune_damage,
	rune_shield,

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

	enemy_count, //leave this last
};

enum EffectType
{
	NONE,
	torch,
	spark,
	shield,
};

#endif