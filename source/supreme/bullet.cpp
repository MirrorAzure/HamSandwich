#include "winpch.h"
#include "bullet.h"
#include "guy.h"
#include "player.h"
#include "scanner.h"
#include "editor.h"
#include "shop.h"
#include "config.h"

constexpr int SPR_FLAME     = 0;
constexpr int SPR_LASER     = 5;
constexpr int SPR_HAMMER    = 21;
constexpr int SPR_MISSILE   = 149;
constexpr int SPR_SMOKE     = 165;
constexpr int SPR_ACID      = 172;
constexpr int SPR_BOMB      = 228;
constexpr int SPR_ENERGY    = 236;
constexpr int SPR_BOOM      = 238;
constexpr int SPR_MEGABEAM  = 246;
constexpr int SPR_SPORE     = 254;
constexpr int SPR_SHROOM    = 258;
constexpr int SPR_GRENADE   = 266;
constexpr int SPR_YELBOOM   = 268;
constexpr int SPR_SHOCKWAVE = 273;
constexpr int SPR_LILBOOM   = 277;
constexpr int SPR_SNOWBALL  = 282;
constexpr int SPR_BIGSNOW   = 283;
constexpr int SPR_ICESPIKE  = 286;
constexpr int SPR_ROCK      = 290;
constexpr int SPR_SPINE     = 294;
constexpr int SPR_BIGAXE    = 310;
constexpr int SPR_SPEAR     = 318;
constexpr int SPR_SLASH     = 326;
constexpr int SPR_MINE      = 350;
constexpr int SPR_STINKY    = 355;
constexpr int SPR_GREEN     = 358;
constexpr int SPR_ORBITER   = 359;
constexpr int SPR_PAPER     = 367;
constexpr int SPR_BUBBLE    = 375;
constexpr int SPR_SCANSHOT  = 384;
constexpr int SPR_SCANLOCK  = 387;
constexpr int SPR_ICEBEAM 	= 388;
constexpr int SPR_SLIME 	= 393;
constexpr int SPR_WAVE 		= 405;
constexpr int SPR_POISONGAS = 413;
constexpr int SPR_FLAMEWALL = 419;
constexpr int SPR_EATHSPIKE = 423;
constexpr int SPR_MEGABOOM  = 431;
constexpr int SPR_CLAW 	  	= 441;
constexpr int SPR_SWAMPGAS  = 457;
constexpr int SPR_EVILFACE  = 461;
constexpr int SPR_FIREBALL  = 467;
constexpr int SPR_SKULL	  	= 507;
constexpr int SPR_COMET	  	= 523;
constexpr int SPR_COMETBOOM = 531;

static bullet_t *bullet;
static sprite_set_t *bulletSpr;
static byte reflect = 0;
static byte attackType;
static int activeBulDX, activeBulDY;

void GetBulletDeltas(int *bdx,int *bdy)
{
	*bdx=activeBulDX;
	*bdy=activeBulDY;
}

void InitBullets(void)
{
	bulletSpr=new sprite_set_t("graphics/bullets.jsp");

	bullet=(bullet_t *)malloc(sizeof(bullet_t)*config.numBullets);
	memset(bullet,0,config.numBullets*sizeof(bullet_t));
}

void ExitBullets(void)
{
	free(bullet);
	delete bulletSpr;
}

byte Bulletable(byte type,Map *map,int x,int y)
{
	mapTile_t *tile;

	tile=map->GetTile(x,y);
	if(tile->wall==65535)
		return 0;

	if(tile->wall ||
		!BulletHitItem(type,tile,x,y))
	{
		if(tile->wall)
		{
			if(curWorld.terrain[tile->wall].flags&TF_DESTRUCT)
				tile->wall=curWorld.terrain[tile->wall].next;
		}
		if(curWorld.terrain[tile->floor].flags&TF_DESTRUCT)
			tile->floor=curWorld.terrain[tile->floor].next;
		return 0;
	}

	return 1;
}

void OffScreenBulletDie(bullet_t *me,Map *map)
{
	if(me->x<0 || me->y<0 || me->x>=(map->width*TILE_WIDTH*FIXAMT) ||
		me->y>=(map->height*TILE_HEIGHT*FIXAMT))
		me->type=BLT_NONE;
}

byte BulletCanGo(byte type,int xx,int yy,Map *map,byte size,byte friendly)
{
	byte result;
	int mapx,mapy,mapx1,mapx2,mapy1,mapy2;

	if(xx<0 || yy<0)
		return 0;
	if(type==BLT_LIFEBLIP)
		return 1;
	xx>>=FIXSHIFT;
	yy>>=FIXSHIFT;

	mapx=xx/TILE_WIDTH;
	mapy=yy/TILE_HEIGHT;
	mapx1=(xx-size)/TILE_WIDTH;
	mapy1=(yy-size)/TILE_HEIGHT;
	mapx2=(xx+size)/TILE_WIDTH;
	mapy2=(yy+size)/TILE_HEIGHT;

	result=(mapx1>=0 && mapy1>=0 && mapx2<map->width && mapy2<map->height &&
		(Bulletable(type,map,mapx,mapy1)) &&
		(Bulletable(type,map,mapx,mapy2)) &&
		(Bulletable(type,map,mapx1,mapy)) &&
		(Bulletable(type,map,mapx2,mapy)) &&
		(Bulletable(type,map,mapx,mapy)) &&
		(Bulletable(type,map,mapx1,mapy1)) &&
		(Bulletable(type,map,mapx2,mapy1)) &&
		(Bulletable(type,map,mapx2,mapy2)) &&
		(Bulletable(type,map,mapx1,mapy2)));

	if(!result)
		EventOccur(EVT_SHOOT,friendly,mapx,mapy,NULL);

	return result;
}

void BulletHitWallX(bullet_t *me,Map *map,world_t *world)
{
	switch(me->type)
	{
		case BLT_FIREBALL:
			me->type=BLT_NONE;
			ExplodeParticles(PART_YELLOW,me->x,me->y,me->z,8);
			MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,1000);
			break;
		case BLT_SKULL:
			if(player.hammerFlags&HMR_REFLECT)
			{
				me->x-=me->dx;
				me->dx=-me->dx;
				me->facing=((byte)(4-me->facing))&7;
				MakeSound(SND_FBALLREFLECT,me->x,me->y,SND_CUTOFF,900);
			}
			else
			{
				me->type=BLT_NONE;
				ExplodeParticles(PART_YELLOW,me->x,me->y,me->z,8);
				MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,1000);
			}
			break;
		case BLT_COMET:
		case BLT_COMETBOOM:
			break;
		case BLT_EVILFACE:
			// can go through anything
			break;
		case BLT_LASER2:
			me->type=BLT_NONE;
			ExplodeParticlesColor(7,me->x,me->y,me->z,1,2);
			break;
		case BLT_CLAW:
			ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,7);
			me->type=0;
			break;
		case BLT_WIND:
		case BLT_ICESHARD:
			BulletRanOut(me,map,world);
			break;
		case BLT_MEGABOOM:
			break;
		case BLT_EARTHSPIKE:
			ExplodeParticles(PART_DIRT,me->x,me->y,me->z,8);
			me->type=0;
			break;
		case BLT_FLAMEWALL:
			ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,4);
			me->type=0;
			break;
		case BLT_GASBLAST:
			me->x-=me->dx;
			me->dx=0;
			if(me->timer>6)
				me->timer=6;
			break;
		case BLT_BADSITFLAME:
			me->dx=me->dy=0;
			break;
		case BLT_BIGYELLOW:
			me->x-=me->dx;
			me->dx=-me->dx;
			me->facing=((byte)(4-me->facing))&7;
			//MakeSound(SND_HAMMERREFLECT,me->x,me->y,SND_CUTOFF,900); // ching!
			break;
		case BLT_YELWAVE:
			me->type=BLT_NONE;
			ExplodeParticles(PART_YELLOW,me->x,me->y,me->z,2);
			break;
		case BLT_ICEBEAM2:
		case BLT_DEATHBEAM2:
		break;
		case BLT_ICEBEAM:
			me->timer=8;
			me->type=BLT_ICEBEAM2;
			break;
		case BLT_DEATHBEAM:
			me->timer=8;
			me->type=BLT_DEATHBEAM2;
			break;
		case BLT_HAMMER:
		case BLT_LUNA:
		case BLT_BIGSHELL:
			me->type=BLT_NONE;
			ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
			MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,1000);
			break;
		case BLT_ORBITER:
		case BLT_ORBITER2:
		case BLT_SWAP:
		case BLT_SCANSHOT:
			me->x-=me->dx;
			me->dx=-me->dx;
			break;
		case BLT_FIREBALL2:
			ExplodeParticles(PART_YELLOW,me->x,me->y,me->z,3);
			me->x-=me->dx;
			me->dx=-me->dx;
			me->facing=((byte)(4-me->facing))&7;
			MakeSound(SND_FBALLREFLECT,me->x,me->y,SND_CUTOFF,900);
			break;
		case BLT_HAMMER2:
		case BLT_LUNA2:
		case BLT_EVILHAMMER:	// reflects off walls
			me->x-=me->dx;
			me->dx=-me->dx;
			me->facing=((byte)(4-me->facing))&7;
			MakeSound(SND_HAMMERREFLECT,me->x,me->y,SND_CUTOFF,900);
			break;
		case BLT_CHEESEHAMMER:
			me->x-=me->dx;
			if(me->facing<128)
				me->facing=128-me->facing;
			else
				me->facing=128+(256-me->facing);
			me->dx=Cosine(me->facing)*14;
			me->dy=Sine(me->facing)*14;
			MakeSound(SND_HAMMERREFLECT,me->x,me->y,SND_CUTOFF,900);
			break;
		case BLT_BALLLIGHTNING:
			if(player.hammerFlags&HMR_REFLECT)
			{
				me->x-=me->dx;
				me->facing=84+Random(89);
				if(me->dx<0)
					me->facing=(me->facing+128)&255;
				me->dx=Cosine(me->facing)*11;
				me->dy=Sine(me->facing)*11;
			}
			else
				BulletRanOut(me,map,world);
			break;
		case BLT_GRENADE:
		case BLT_HOLESHOT:
		case BLT_BLACKHOLE:
			me->x-=me->dx;
			break;
		case BLT_LASER:	// reflects off walls
			MakeSound(SND_BULLETREFLECT,me->x,me->y,SND_CUTOFF,900);
			me->x-=me->dx;
			me->dx=-me->dx;
			me->dy+=-FIXAMT/4+Random(FIXAMT/2);
			me->facing=((byte)(8-me->facing))&15;
			break;
		case BLT_BOMB:
			MakeSound(SND_BOMBREFLECT,me->x,me->y,SND_CUTOFF,600);
			me->x-=me->dx;
			me->dx=-me->dx;
			me->dy+=-FIXAMT/4+Random(FIXAMT/2);
			me->facing=((byte)(8-me->facing))&15;
			break;
		case BLT_SLASH:
			MakeSound(SND_CLANG,me->x,me->y,SND_CUTOFF,950);
			// supposedly sparks
			ExplodeParticles2(PART_SNOW2,me->x,me->y,me->z,4,8);
			me->type=BLT_NONE;
			break;
		case BLT_IGNITE:
		case BLT_MISSILE:
		case BLT_SPINE:
		case BLT_BIGAXE:
		case BLT_SPEAR:
		case BLT_BADSPEAR:
		case BLT_MINDWIPE:
		case BLT_REFLECT:
		case BLT_HARPOON:
		case BLT_SCANNER:
		case BLT_TORPEDO:
		case BLT_PAPER:
		case BLT_BUBBLE:
		case BLT_FREEZE:
		case BLT_FREEZE2:
			BulletRanOut(me,map,world);
			break;
		case BLT_FLAME:
		case BLT_FLAME2:
		case BLT_FLAME3:
			me->x-=me->dx;
			me->dy=((3-Random(7))<<FIXSHIFT);
			me->dx=0;
			break;
		case BLT_ROCK:	// reflects off walls
			me->x-=me->dx;
			me->dx=-me->dx;
			me->facing=((byte)(4-me->facing))&7;
			MakeSound(SND_ROCKBOUNCE,me->x,me->y,SND_CUTOFF,600);
			break;
		case BLT_ACID:
			MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,950);
			ExplodeParticles(PART_SLIME,me->x,me->y,me->z,6);
			me->type=0;
			break;
		case BLT_SHARK:
			MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,950);
			ExplodeParticles(PART_WATER,me->x,me->y,me->z,6);
			me->type=0;
			break;
		case BLT_ENERGY:
		case BLT_BIGSHOT:
			MakeSound(SND_ENERGYBONK,me->x,me->y,SND_CUTOFF,950);
			ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,4);
			me->type=0;
			break;
		case BLT_GREEN:
			MakeSound(SND_ENERGYBONK,me->x,me->y,SND_CUTOFF,950);
			ExplodeParticles(PART_SLIME,me->x,me->y,me->z,4);
			me->type=0;
			break;
		case BLT_MEGABEAM:
		case BLT_MEGABEAM1:
			me->type=BLT_MEGABEAM2;
			me->dy=0;
			me->timer=2;
			break;
		case BLT_SPORE:
		case BLT_ICESPIKE:
		case BLT_DIRTSPIKE:
			me->type=BLT_NONE;
			break;
		case BLT_SHROOM:
			// detonate
			BulletRanOut(me,map,world);
			break;
		case BLT_SNOWBALL:
		case BLT_BIGSNOW:
			BulletRanOut(me,map,world);
			break;
	}
}

void BulletHitWallY(bullet_t *me,Map *map,world_t *world)
{
	switch(me->type)
	{
		case BLT_FIREBALL:
			me->type=BLT_NONE;
			ExplodeParticles(PART_YELLOW,me->x,me->y,me->z,8);
			MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,1000);
			break;
		case BLT_SKULL:
			if(player.hammerFlags&HMR_REFLECT)
			{
				me->y-=me->dy;
				me->dy=-me->dy;
				me->facing=(8-me->facing)&7;
				MakeSound(SND_FBALLREFLECT,me->x,me->y,SND_CUTOFF,900);
			}
			else
			{
				me->type=BLT_NONE;
				ExplodeParticles(PART_YELLOW,me->x,me->y,me->z,8);
				MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,1000);
			}
			break;
		case BLT_COMET:
		case BLT_COMETBOOM:
			break;
		case BLT_EVILFACE:
			// can go through anything
			break;
		case BLT_LASER2:
			me->type=BLT_NONE;
			ExplodeParticlesColor(7,me->x,me->y,me->z,1,2);
			break;
		case BLT_BADSITFLAME:
			me->dx=me->dy=0;
			break;
		case BLT_CLAW:
			ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,7);
			me->type=0;
			break;
		case BLT_WIND:
		case BLT_ICESHARD:
			BulletRanOut(me,map,world);
			break;
		case BLT_MEGABOOM:
			break;
		case BLT_EARTHSPIKE:
			ExplodeParticles(PART_DIRT,me->x,me->y,me->z,8);
			me->type=0;
			break;
		case BLT_FLAMEWALL:
			ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,4);
			me->type=0;
			break;
		case BLT_GASBLAST:
			me->y-=me->dy;
			me->dy=0;
			if(me->timer>6)
				me->timer=6;
			break;
		case BLT_YELWAVE:
			me->type=BLT_NONE;
			ExplodeParticles(PART_YELLOW,me->x,me->y,me->z,2);
			break;
		case BLT_BIGYELLOW:
			me->y-=me->dy;
			me->dy=-me->dy;
			me->facing=(8-me->facing)&7;
			//MakeSound(SND_HAMMERREFLECT,me->x,me->y,SND_CUTOFF,900); ching!
			break;
		case BLT_ICEBEAM2:
		case BLT_DEATHBEAM2:
		break;
		case BLT_ICEBEAM:
			me->timer=8;
			me->type=BLT_ICEBEAM2;
			break;
		case BLT_DEATHBEAM:
			me->timer=8;
			me->type=BLT_DEATHBEAM2;
			break;
		case BLT_HAMMER:
		case BLT_LUNA:
		case BLT_BIGSHELL:
			me->type=BLT_NONE;
			ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
			MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,1000);
			break;
		case BLT_ORBITER:
		case BLT_ORBITER2:
		case BLT_SWAP:
		case BLT_SCANSHOT:
			me->y-=me->dy;
			me->dy=-me->dy;
			break;
		case BLT_FIREBALL2:
			ExplodeParticles(PART_YELLOW,me->x,me->y,me->z,3);
			me->y-=me->dy;
			me->dy=-me->dy;
			me->facing=(8-me->facing)&7;
			MakeSound(SND_FBALLREFLECT,me->x,me->y,SND_CUTOFF,900);
			break;
		case BLT_HAMMER2:
		case BLT_LUNA2:
		case BLT_EVILHAMMER:	// reflects off walls
			me->y-=me->dy;
			me->dy=-me->dy;
			me->facing=(8-me->facing)&7;
			MakeSound(SND_HAMMERREFLECT,me->x,me->y,SND_CUTOFF,900);
			break;
		case BLT_CHEESEHAMMER:
			me->y-=me->dy;
			if(me->facing<128)
				me->facing=128+(128-me->facing);
			else
				me->facing=128-(me->facing-128);
			me->dx=Cosine(me->facing)*14;
			me->dy=Sine(me->facing)*14;
			MakeSound(SND_HAMMERREFLECT,me->x,me->y,SND_CUTOFF,900);
			break;
		case BLT_BALLLIGHTNING:
			if(player.hammerFlags&HMR_REFLECT)
			{
				me->y-=me->dy;
				me->facing=20+Random(89);
				if(me->dy>0)
					me->facing+=128;
				me->dx=Cosine(me->facing)*11;
				me->dy=Sine(me->facing)*11;
			}
			else
				BulletRanOut(me,map,world);
			break;
		case BLT_GRENADE:
			me->y-=me->dy;
			break;
		case BLT_LASER:
			MakeSound(SND_BULLETREFLECT,me->x,me->y,SND_CUTOFF,900);
			me->y-=me->dy;
			me->dy=-me->dy;
			me->dx+=-FIXAMT/4+Random(FIXAMT/2);
			me->facing=(16-me->facing)&15;
			break;
		case BLT_BOMB:
			MakeSound(SND_BOMBREFLECT,me->x,me->y,SND_CUTOFF,600);
			me->y-=me->dy;
			me->dy=-me->dy;
			me->dx+=-FIXAMT/4+Random(FIXAMT/2);
			me->facing=(16-me->facing)&15;
			break;
		case BLT_SLASH:
			MakeSound(SND_CLANG,me->x,me->y,SND_CUTOFF,950);
			// supposedly sparks
			ExplodeParticles2(PART_SNOW2,me->x,me->y,me->z,4,8);
			me->type=BLT_NONE;
			break;
		case BLT_IGNITE:
		case BLT_MISSILE:
		case BLT_SPINE:
		case BLT_BIGAXE:
		case BLT_SPEAR:
		case BLT_BADSPEAR:
		case BLT_MINDWIPE:
		case BLT_REFLECT:
		case BLT_HARPOON:
		case BLT_SCANNER:
		case BLT_TORPEDO:
		case BLT_PAPER:
		case BLT_BUBBLE:
		case BLT_FREEZE:
		case BLT_FREEZE2:
			BulletRanOut(me,map,world);
			break;
		case BLT_FLAME:
		case BLT_FLAME2:
			me->y-=me->dy;
			me->dx=((3-Random(7))<<FIXSHIFT);
			me->dy=0;
			break;
		case BLT_ROCK:	// reflects off walls
			me->y-=me->dy;
			me->dy=-me->dy;
			me->facing=(8-me->facing)&7;
			MakeSound(SND_ROCKBOUNCE,me->x,me->y,SND_CUTOFF,600);
			break;
		case BLT_ACID:
			MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,950);
			ExplodeParticles(PART_SLIME,me->x,me->y,me->z,6);
			me->type=0;
			break;
		case BLT_SHARK:
			MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,950);
			ExplodeParticles(PART_WATER,me->x,me->y,me->z,6);
			me->type=0;
			break;
		case BLT_ENERGY:
		case BLT_BIGSHOT:
			MakeSound(SND_ENERGYBONK,me->x,me->y,SND_CUTOFF,950);
			ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,4);
			me->type=0;
			break;
		case BLT_GREEN:
			MakeSound(SND_ENERGYBONK,me->x,me->y,SND_CUTOFF,950);
			ExplodeParticles(PART_SLIME,me->x,me->y,me->z,4);
			me->type=0;
			break;
		case BLT_MEGABEAM:
		case BLT_MEGABEAM1:
			me->type=BLT_MEGABEAM2;
			me->dy=0;
			me->timer=2;
			break;
		case BLT_SPORE:
		case BLT_ICESPIKE:
		case BLT_DIRTSPIKE:
			me->type=BLT_NONE;
			break;
		case BLT_SHROOM:
			// detonate
			BulletRanOut(me,map,world);
			break;
		case BLT_SNOWBALL:
		case BLT_BIGSNOW:
			BulletRanOut(me,map,world);
			break;
	}
}

void BulletHitFloor(bullet_t *me,Map *map,world_t *world)
{
	int x,y;

	switch(me->type)
	{
		case BLT_COMET:
			me->type=BLT_COMETBOOM;
			me->anim=0;
			me->timer=18;	// 9 frames of animation
			me->dz=0;
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,200);
			break;
		case BLT_COMETBOOM:
			break;
		case BLT_EVILFACE:
		case BLT_ICESHARD:
		case BLT_WIND:
		case BLT_FLAMEWALL:
		break;
		case BLT_BADSITFLAME:
			Dampen(&me->dx,FIXAMT/8);
			Dampen(&me->dx,FIXAMT/8);
			me->z=0;
			me->dz=0;
			break;
		case BLT_BIGSHELL:
			BulletRanOut(me,map,world);
			break;
		case BLT_FIREBALL:
		case BLT_FIREBALL2:
			MakeSound(SND_FBALLREFLECT,me->x,me->y,SND_CUTOFF,850);
			me->dz=-me->dz*3/4;
			me->z=0;
			x=(me->x>>FIXSHIFT)/TILE_WIDTH;
			y=(me->y>>FIXSHIFT)/TILE_HEIGHT;
			if(GetTerrain(world,map->GetTile(x,y)->floor)->flags&TF_WATER)
				ExplodeParticles(PART_WATER,me->x,me->y,0,8);
			else if(GetTerrain(world,map->GetTile(x,y)->floor)->flags&TF_LAVA)
				ExplodeParticles(PART_HAMMER,me->x,me->y,0,8);
			break;
		case BLT_HAMMER:
		case BLT_HAMMER2:
		case BLT_EVILHAMMER:
		case BLT_LUNA:
		case BLT_LUNA2:
			MakeSound(SND_HAMMERREFLECT,me->x,me->y,SND_CUTOFF,850);
			me->dz=-me->dz*3/4;
			me->z=0;
			x=(me->x>>FIXSHIFT)/TILE_WIDTH;
			y=(me->y>>FIXSHIFT)/TILE_HEIGHT;
			if(GetTerrain(world,map->GetTile(x,y)->floor)->flags&TF_WATER)
				ExplodeParticles(PART_WATER,me->x,me->y,0,8);
			else if(GetTerrain(world,map->GetTile(x,y)->floor)->flags&TF_LAVA)
				ExplodeParticles(PART_HAMMER,me->x,me->y,0,8);
			break;
		case BLT_BOMB:
		case BLT_MINDWIPE:
			if(me->dz<-FIXAMT)	// don't make it on small bounces, because it'd be annoying
				MakeSound(SND_BOMBREFLECT,me->x,me->y,SND_CUTOFF,600);
			me->dz=-me->dz*3/4;
			me->z=0;
			break;
		case BLT_BUBBLE:
			me->dz=-me->dz*3/4;
			me->z=0;
			break;
		case BLT_SKULL:
		case BLT_IGNITE:
		case BLT_LASER2:
		case BLT_CLAW:
		case BLT_MEGABOOM:
		case BLT_EARTHSPIKE:
		case BLT_CHEESEHAMMER:
		case BLT_MISSILE: // this really really should never happen
		case BLT_FLAME:
		case BLT_FLAME2:
		case BLT_FLAME3:
		case BLT_LASER:
		case BLT_MEGABEAM:
		case BLT_MEGABEAM1:
		case BLT_MEGABEAM2:
		case BLT_SPORE:
		case BLT_YELBOOM:
		case BLT_LILBOOM:
		case BLT_LILBOOM2:
		case BLT_SHOCKWAVE:
		case BLT_ICESPIKE:
		case BLT_DIRTSPIKE:
		case BLT_SPINE:
		case BLT_BIGAXE:
		case BLT_SLASH:
		case BLT_MINE:
		case BLT_SLIME:
		case BLT_ORBITER:
		case BLT_ORBITER2:
		case BLT_BALLLIGHTNING:
		case BLT_REFLECT:
		case BLT_SWAP:
		case BLT_SCANNER:
		case BLT_SCANSHOT:
		case BLT_TORPEDO:
		case BLT_PAPER:
		case BLT_FREEZE:
		case BLT_HOLESHOT:
		case BLT_BLACKHOLE:
		case BLT_GASBLAST:
			me->z=0;
			break;
		case BLT_SPEAR:
		case BLT_BADSPEAR:
		case BLT_HARPOON:
		case BLT_FREEZE2:
			BulletRanOut(me,map,world);
			break;
		case BLT_ROCK:
			if(me->dz<-FIXAMT)
				MakeSound(SND_ROCKBOUNCE,me->x,me->y,SND_CUTOFF,600);
			me->dz=-me->dz*3/4;
			me->z=0;
			break;
		case BLT_ENERGY:
		case BLT_BIGSHOT:
			MakeSound(SND_ENERGYBONK,me->x,me->y,SND_CUTOFF,950);
			ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,4);
			me->type=0;
			break;
		case BLT_GREEN:
			MakeSound(SND_ENERGYBONK,me->x,me->y,SND_CUTOFF,950);
			ExplodeParticles(PART_SLIME,me->x,me->y,me->z,4);
			me->type=0;
			break;
		case BLT_ACID:
			MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,850);
			ExplodeParticles(PART_SLIME,me->x,me->y,me->z,6);
			me->type=0;
			break;
		case BLT_SHARK:
			MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,850);
			ExplodeParticles(PART_WATER,me->x,me->y,me->z,6);
			me->type=0;
			break;
		case BLT_SHROOM:
			// detonate
			BulletRanOut(me,map,world);
			break;
		case BLT_GRENADE:
			me->type=BLT_YELBOOM;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			me->anim=0;
			me->timer=9;
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,950);
			break;
		case BLT_SNOWBALL:
		case BLT_BIGSNOW:
			BulletRanOut(me,map,world);
			break;
	}
}

void BulletRanOut(bullet_t *me,Map *map,world_t *world)
{
	int i;
	Guy *g;

	switch(me->type)
	{
		case BLT_FIREBALL:
		case BLT_FIREBALL2:
		case BLT_BIGYELLOW:
		case BLT_SKULL:
			ExplodeParticles2(PART_YELLOW,me->x,me->y,me->z,6,3);
			me->type=0;
			break;
		case BLT_IGNITE:
			me->type=0;
			ExplodeParticles2(PART_SNOW2,me->x,me->y,me->z,10,8);
			break;
		case BLT_WIND:
		case BLT_ICESHARD:
			ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,8);
			me->type=BLT_NONE;
			break;
		case BLT_BADSITFLAME:
			me->type=0;
			BlowSmoke(me->x,me->y,me->z,FIXAMT);
			break;
		case BLT_YELWAVE:
			me->type=BLT_NONE;
			ExplodeParticles(PART_YELLOW,me->x,me->y,me->z,2);
			break;
		case BLT_COMET:
		case BLT_COMETBOOM:
		case BLT_EVILFACE:
		case BLT_LASER2:
		case BLT_SWAMPGAS:
		case BLT_CLAW:
		case BLT_MEGABOOM:
		case BLT_EARTHSPIKE:
		case BLT_BIGSHOT:
		case BLT_FLAMEWALL:
		case BLT_GASBLAST:
		case BLT_SLIME:
		case BLT_ICEBEAM:
		case BLT_DEATHBEAM:
		case BLT_DEATHBEAM2:
		case BLT_ICEBEAM2:
		case BLT_LUNA:
		case BLT_LUNA2:
		case BLT_HAMMER:
		case BLT_HAMMER2:
		case BLT_LILBOOM:
		case BLT_LILBOOM2:
		case BLT_LASER:
		case BLT_BOOM:
		case BLT_ENERGY:
		case BLT_MEGABEAM:
		case BLT_MEGABEAM1:
		case BLT_MEGABEAM2:
		case BLT_SPORE:
		case BLT_GRENADE:
		case BLT_YELBOOM:
		case BLT_SHOCKWAVE:
		case BLT_ICESPIKE:
		case BLT_DIRTSPIKE:
		case BLT_ROCK:
		case BLT_SPINE:
		case BLT_EVILHAMMER:
		case BLT_SLASH:
		case BLT_GREEN:
		case BLT_REFLECT:
		case BLT_BUBBLEPOP:
		case BLT_SCANLOCK:
		case BLT_BLACKHOLE:
		case BLT_LIFEBLIP:
			me->type=0;
			break;
		case BLT_HOLESHOT:
			me->type=BLT_BLACKHOLE;
			me->dx=0;
			me->dy=0;
			me->timer=30*2;//+player.weaponLvl[WPN_PORTAL-1]*10;
			me->anim=0;
			break;
		case BLT_SPEAR:
		case BLT_BADSPEAR:
		case BLT_MINDWIPE:
		case BLT_HARPOON:
			ExplodeParticles(PART_HAMMER,me->x,me->y,0,8);
			me->type=0;
			break;
		case BLT_BIGSNOW:
			// poof into snowballs
			MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,850);
			me->type=0;
			for(i=0;i<8;i++)
				FireBullet(me->x,me->y,(byte)i*32,BLT_SNOWBALL,me->friendly);
			break;
		case BLT_SNOWBALL:
		case BLT_FREEZE:
		case BLT_FREEZE2:
			MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,850);
			me->type=0;
			ExplodeParticles2(PART_SNOW2,me->x,me->y,me->z,6,12);
			break;
		case BLT_PAPER:
			me->type=0;
			ExplodeParticles2(PART_SNOW2,me->x,me->y,me->z,6,12);
			break;
		case BLT_BIGAXE:
			me->type=0;
			ExplodeParticles2(PART_SNOW2,me->x,me->y,me->z,10,8);
			break;
		case BLT_MISSILE:
		case BLT_CHEESEHAMMER:
			MakeSound(SND_MISSILEBOOM,me->x,me->y,SND_CUTOFF,1500);
			me->type=BLT_LILBOOM;
			me->timer=9;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			break;
		case BLT_TORPEDO:
			MakeSound(SND_MISSILEBOOM,me->x,me->y,SND_CUTOFF,1500);
			me->type=BLT_LILBOOM2;
			me->timer=9;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			break;
		case BLT_ACID:
		case BLT_SWAP:
			MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,850);
			ExplodeParticles(PART_SLIME,me->x,me->y,me->z,6);
			me->type=0;
			break;
		case BLT_SCANNER:
		case BLT_SCANSHOT:
			ExplodeParticles(PART_SLIME,me->x,me->y,me->z,6);
			me->type=0;
			break;
		case BLT_SHARK:
			MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,850);
			ExplodeParticles(PART_WATER,me->x,me->y,me->z,6);
			me->type=0;
			break;
		case BLT_BUBBLE:
			ExplodeParticles(PART_WATER,me->x,me->y,me->z,6);
			me->type=BLT_BUBBLEPOP;
			me->anim=0;
			me->timer=10;
			me->dx=0;
			me->dy=0;
			MakeSound(SND_BUBBLEPOP,me->x,me->y,SND_CUTOFF,1);
			break;
		case BLT_FLAME:
		case BLT_FLAME2:
		case BLT_FLAME3:
			me->type=0;
			BlowSmoke(me->x,me->y,me->z,FIXAMT);
			break;
		case BLT_BOMB:
		case BLT_MINE:
		case BLT_ORBITER:
		case BLT_ORBITER2:
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,2000);
			me->dx=0;
			me->dy=0;
			me->dz=0;
			me->type=BLT_BOOM;
			me->timer=7;
			me->anim=0;
			break;
		case BLT_SHROOM:
			for(i=0;i<256;i+=8)
			{
				FireExactBullet(me->x,me->y,me->z,Cosine(i)*12,Sine(i)*12,0,0,16,i,BLT_SPORE,me->friendly);
				FireExactBullet(me->x,me->y,me->z,Cosine(i)*6,Sine(i)*6,0,0,16,i,BLT_SPORE,me->friendly);
			}
			g=AddGuy(me->x-me->dx*2,me->y-me->dy*2,me->z,MONS_SHROOM,me->friendly);	// become a living shroom
			if(g && (!g->CanWalk(g->x,g->y,map,world)))
				RemoveGuy(g);

			me->type=BLT_NONE;	// all gone
			MakeSound(SND_MISSILEBOOM,me->x,me->y,SND_CUTOFF,1000);
			break;
		case BLT_BIGSHELL:
			me->type=BLT_NONE;
			ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
			MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,1000);
			break;
		case BLT_BALLLIGHTNING:
			ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,8);
			me->type=BLT_NONE;
			break;
	}
}

void HitBadguys(bullet_t *me,Map *map,world_t *world)
{
	int i;

	attackType=me->type;
	switch(me->type)
	{
		case BLT_COMETBOOM:
			if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,64,(8-MGL_random(17))<<FIXSHIFT,
				(8-MGL_random(16))<<FIXSHIFT,10,map,world,me->friendly))
			{
				// nothing much to do here, the victim will scream quite enough
			}
			break;
		case BLT_FIREBALL:
		case BLT_FIREBALL2:
		case BLT_SKULL:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,5,map,world,me->friendly))
				{
					if(!reflect)
					{
						me->type=BLT_NONE;
						ExplodeParticles(PART_YELLOW,me->x,me->y,me->z,8);
						MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,900);
					}
					else
					{
						me->friendly=1-me->friendly;
						me->facing=(me->facing+4)&7;
						me->dx=Cosine(me->facing*32)*12;
						me->dy=Sine(me->facing*32)*12;
						me->timer+=10;
					}
				}
			break;
		case BLT_LIFEBLIP:
			if(goodguy)
			{
				if(abs(me->x-goodguy->x)<64*FIXAMT && abs(me->y-goodguy->y)<48*FIXAMT)
				{
					if(me->type==BLT_LIFEBLIP)
					{
						if(goodguy->hp<goodguy->maxHP)
						{
							HealGoodguy(2);
							me->type=BLT_NONE;
						}
					}
				}
			}
			break;
		case BLT_IGNITE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,0,0,0,map,world,me->friendly))
			{
				if(!reflect)
				{
					GetLastGuyHit()->ignited=5*30+30*2;
					me->type=BLT_NONE;
					ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
					MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,900);
				}
				else
				{
					me->friendly=1-me->friendly;
					me->target=65535;
					me->dx=-me->dx;
					me->dy=-me->dy;
				}
			}
			break;
		case BLT_LASER2:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,0,0,80,map,world,me->friendly))
			{
				ExplodeParticlesColor(7,me->x,me->y,me->z,2,5);
				MakeSound(SND_LIGHTNING,me->x,me->y,SND_CUTOFF,900);
			}
			if(me->timer<30*10-2 && FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,0,0,80,map,world,1-me->friendly))
			{
				ExplodeParticlesColor(7,me->x,me->y,me->z,2,5);
				MakeSound(SND_LIGHTNING,me->x,me->y,SND_CUTOFF,900);
			}
			break;
		case BLT_ICESHARD:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,4,me->dx/2,me->dy/2,0,map,world,me->friendly))
			{
				//player.invinc=0;	// do not blink
				//player.stone+=15;
				goodguy->dx+=me->dx/4;
				goodguy->dy+=me->dy/4;
				if(FreezeGuy(GetLastGuyHit()))
				{
					MakeSound(SND_FREEZE,me->x,me->y,SND_CUTOFF,1400);
				}
			}
			break;
		case BLT_SWAMPGAS:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,10,me->dx,me->dy,0,map,world,me->friendly))
			{
				// 1 hearts/seconds of poison please
				PoisonVictim(GetLastGuyHit(),32);
			}
			break;
		case BLT_CLAW:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,0,0,3,map,world,me->friendly))
			{
				ExplodeParticles(PART_SNOW2,me->x,me->y,0,10);
				me->type=BLT_NONE;
			}
			break;
		case BLT_WIND:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,me->dx,me->dy,3,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);
			}
			break;
		case BLT_MEGABOOM:
			if(FindVictims2(me->x>>FIXSHIFT,me->y>>FIXSHIFT,100,0,0,30,map,world,me->friendly))
			{
				// ouch
			}
			break;
		case BLT_EARTHSPIKE:
			if(FindVictims2(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,me->dx,me->dy,3,map,world,me->friendly))
			{
			}
			break;
		case BLT_BIGSHOT:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,3,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,12);
			}
			break;
		case BLT_FLAMEWALL:
			if(FindVictims2(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,me->dx,me->dy,1,map,world,me->friendly))
			{
				// make a noise
			}
			break;
		case BLT_GASBLAST:
			if(FindVictims2(me->x>>FIXSHIFT,me->y>>FIXSHIFT,24,me->dx,me->dy,0,map,world,me->friendly))
			{
				PoisonVictim(GetLastGuyHit(),30);
			}
			break;
		case BLT_BLACKHOLE:
			if(FindVictims2(me->x>>FIXSHIFT,me->y>>FIXSHIFT,32,0,0,1,map,world,me->friendly))
			{

			}
			break;
		case BLT_BADSITFLAME:
			//BurnHay(me->x,me->y);
			if(FindVictims2(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,1,map,world,me->friendly))
			{
				BlowSmoke(me->x,me->y,me->z,FIXAMT);
				// no noise, just let them scream
			}
			break;
		case BLT_YELWAVE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,8,map,world,me->friendly))
			{
				// hit noise
				BulletRanOut(me,map,world);
			}
			break;
		case BLT_BIGYELLOW:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,me->dx,me->dy,20,map,world,me->friendly))
			{
				// hit noise
				BulletRanOut(me,map,world);
			}
			break;
		case BLT_SLIME:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,0,0,2,map,world,me->friendly))
			{
				ExplodeParticles(PART_WATER,me->x,me->y,me->z,4);
			}
			break;
		case BLT_SCANNER:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,0,0,0,map,world,me->friendly))
			{
				if(GetLastGuyHit()->aiType!=MONS_BOUAPHA)
				{
					ScanGuy(GetLastGuyHit());
					me->type=BLT_SCANLOCK;
					me->timer=60;
					me->target=GetLastGuyHit()->ID;
				}
			}
			else if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,0,0,0,map,world,1-me->friendly))
			{
				if(GetLastGuyHit()->aiType!=MONS_BOUAPHA)
				{
					ScanGuy(GetLastGuyHit());
					me->type=BLT_SCANLOCK;
					me->timer=60;
					me->target=GetLastGuyHit()->ID;
				}
			}
			break;
		case BLT_LASER:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,1,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
				MakeSound(SND_BULLETHIT,me->x,me->y,SND_CUTOFF,900);
			}
			break;
		case BLT_GREEN:
		case BLT_SCANSHOT:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,3,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				ExplodeParticles(PART_SLIME,me->x,me->y,me->z,8);
				MakeSound(SND_BULLETHIT,me->x,me->y,SND_CUTOFF,900);
			}
			break;
		case BLT_BALLLIGHTNING:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,5,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,8);
				MakeSound(SND_LIGHTNING,me->x,me->y,SND_CUTOFF,900);
				for(i=0;i<player.hammers-1;i++)
					FireBullet(me->x,me->y,0,BLT_LIGHTNING2,me->friendly);
				me->type=BLT_NONE;
			}
			break;
		case BLT_HAMMER:
		case BLT_HAMMER2:
		case BLT_LUNA:
		case BLT_LUNA2:
		case BLT_BIGSHELL:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,5,map,world,me->friendly))
			{
				if(!reflect)
				{
					me->type=BLT_NONE;
					ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
					MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,900);
				}
				else
				{
					me->friendly=1-me->friendly;
					me->type=BLT_EVILHAMMER;
					me->facing=(me->facing+4)&7;
					me->dx=Cosine(me->facing*32)*12;
					me->dy=Sine(me->facing*32)*12;
					me->timer+=10;
				}
			}
			break;
		case BLT_CHEESEHAMMER:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,me->dx,me->dy,5,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);	// detonate
			}
			break;
		case BLT_PAPER:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,5,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);
			}
			break;
		case BLT_SPEAR:
		case BLT_HARPOON:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,5,me->dx,me->dy,5,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
				MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,900);
			}
			break;
		case BLT_BADSPEAR:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,3,me->dx,me->dy,5,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,8);
				MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,900);
			}
			break;
		case BLT_MISSILE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,0,0,8,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);	// detonate
			}
			break;
		case BLT_TORPEDO:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,0,0,2,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);	// detonate
			}
			break;
		case BLT_LILBOOM:
			if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,(8-Random(17))<<FIXSHIFT,
				(8-Random(16))<<FIXSHIFT,2,map,world,me->friendly))
			{
				// nothing much to do here, the victim will scream quite enough
			}
			break;
		case BLT_SLASH:
			if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,Cosine(me->facing*32)*4,
				Sine(me->facing*32)*4,6,map,world,me->friendly))
			{
				// nothing needs to be done
			}
			break;
		case BLT_FLAME:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,1,map,world,me->friendly))
			{
				// no noise, just let them scream
			}
			break;
		case BLT_FLAME2:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,1,map,world,me->friendly))
			{
				// no noise, just let him scream
			}
			break;
		case BLT_SPORE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,4,me->dx/2,me->dy/2,1,map,world,me->friendly))
			{

				PoisonVictim(GetLastGuyHit(),30);
				me->type=BLT_NONE;	// go away
			}
			break;
		case BLT_SPINE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,4,me->dx/4,me->dy/4,4,map,world,me->friendly))
			{
				me->type=BLT_NONE;
			}
			break;
		case BLT_SHROOM:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,10,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);	// detonate, not to mention the 10 damage you already did
			}
			break;
		case BLT_ICESPIKE:
		case BLT_DIRTSPIKE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,Cosine(me->facing*32)*8,Sine(me->facing*32)*8,
				6,map,world,me->friendly))
			{
				// nothing special happens
			}
			break;
		case BLT_BOMB:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,0,0,0,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);	// detonate
			}
			break;
		case BLT_MINE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,16,0,0,0,map,world,me->friendly))
			{
				BulletRanOut(me,map,world);	// detonate
			}
			break;
		case BLT_BOOM:
			if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,64,(8-Random(17))<<FIXSHIFT,
				(8-Random(16))<<FIXSHIFT,4,map,world,me->friendly))
			{
				// nothing much to do here, the victim will scream quite enough
			}
			break;
		case BLT_BIGAXE:
			if(FindVictims2(me->x>>FIXSHIFT,me->y>>FIXSHIFT,32,(4-Random(9))<<FIXSHIFT,
				(4-Random(9))<<FIXSHIFT,5,map,world,me->friendly))
			{
				ExplodeParticles2(PART_SNOW2,me->x,me->y,me->z,10,8);
			}
			break;
		case BLT_YELBOOM:
			i=20*(5-(me->timer/2));	// size expands as boom expands
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,i,(8-Random(17))<<FIXSHIFT,
				(8-Random(16))<<FIXSHIFT,2,map,world,me->friendly))
			{
				// don't disappear because Bouapha needs to get multipounded
			}
			break;
		case BLT_SHOCKWAVE:
			i=30*(3-(me->timer/2))+30;	// size expands as wave expands
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,i,0,0,4,map,world,me->friendly))
			{
				// don't disappear because Bouapha needs to get multipounded
			}
			break;
		case BLT_ACID:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,5,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,1000);
				ExplodeParticles(PART_SLIME,me->x,me->y,me->z,6);
			}
			break;
		case BLT_SHARK:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,5,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				MakeSound(SND_ACIDSPLAT,me->x,me->y,SND_CUTOFF,1000);
				ExplodeParticles(PART_WATER,me->x,me->y,me->z,6);
			}
			break;
		case BLT_BUBBLE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,5,map,world,me->friendly))
			{
				me->type=BLT_BUBBLEPOP;
				me->anim=0;
				me->timer=10;
				me->dx=0;
				me->dy=0;
				MakeSound(SND_BUBBLEPOP,me->x,me->y,SND_CUTOFF,1000);
				ExplodeParticles(PART_WATER,me->x,me->y,me->z,6);
			}
			break;
		case BLT_ICEBEAM:
			if(FindVictims(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,0,0,5,map,world,me->friendly))
			{
				ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,4);
				if(FreezeGuy(GetLastGuyHit()))
				{
					MakeSound(SND_FREEZE,me->x,me->y,SND_CUTOFF,1400);
				}
			}
			break;
		case BLT_DEATHBEAM:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,0,0,40,map,world,me->friendly))
			{
				ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,4);
				MakeSound(SND_MEGABEAMHIT,me->x,me->y,SND_CUTOFF,900);
				me->type=BLT_DEATHBEAM2;
				me->timer=8;
			}
			break;
		case BLT_MEGABEAM1:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,40,0,12*FIXAMT,5,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				MakeSound(SND_MEGABEAMHIT,me->x,me->y,SND_CUTOFF,1000);
				me->type=BLT_MEGABEAM2;
				me->dy=0;
				me->timer=2;
			}
			break;
		case BLT_ENERGY:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,3,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				MakeSound(SND_ENERGYBONK,me->x,me->y,SND_CUTOFF,950);
				ExplodeParticles(PART_HAMMER,me->x,me->y,me->z,6);
			}
			break;
		case BLT_EVILHAMMER:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,10,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				MakeSound(SND_HAMMERBONK,me->x,me->y,SND_CUTOFF,950);
				ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,8);
			}
			break;
		case BLT_ROCK:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,8,map,world,me->friendly))
			{
				me->type=BLT_NONE;
				MakeSound(SND_ROCKBOUNCE,me->x,me->y,SND_CUTOFF,600);
				ExplodeParticles2(PART_DIRT,me->x,me->y,me->z,12,6);
			}
			break;
		case BLT_SNOWBALL:
		case BLT_BIGSNOW:
			if(me->z<FIXAMT*40)
			{
				if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,12,me->dx,me->dy,6,map,world,me->friendly))
				{
					BulletRanOut(me,map,world);
				}
			}
			break;
		case BLT_FREEZE:
		case BLT_FREEZE2:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,0,map,world,me->friendly))
			{
				if(FreezeGuy(GetLastGuyHit()))
				{
					MakeSound(SND_FREEZE,me->x,me->y,SND_CUTOFF,1400);
				}
				BulletRanOut(me,map,world);
			}
			break;
		case BLT_MINDWIPE:
			if(FindVictim(me->x>>FIXSHIFT,me->y>>FIXSHIFT,8,me->dx,me->dy,0,map,world,me->friendly))
			{
				if(ControlMind(GetLastGuyHit()))
				{
					MakeSound(SND_ROBOBOUAPHAON,me->x,me->y,SND_CUTOFF,1400);
				}
				BulletRanOut(me,map,world);
			}
			break;
		case BLT_REFLECT:
			ReflectBullets(me->x,me->y,128,me->friendly);
			break;
		case BLT_SWAP:
			if(SwapMe(me->x,me->y,10,map))
				BulletRanOut(me,map,world);
			break;
	}
	attackType=BLT_NONE;
}

inline void BulletFaceGuy(bullet_t *me,Guy *goodguy)
{
	int desired;
	int diff,dir;

	if(goodguy==NULL)
		return;

	if(goodguy->x<me->x-FIXAMT*32)
	{
		if(goodguy->y<me->y-FIXAMT*32)
			desired=5;
		else if(goodguy->y>me->y+FIXAMT*32)
			desired=3;
		else
			desired=4;
	}
	else if(goodguy->x>me->x+FIXAMT*32)
	{
		if(goodguy->y<me->y-FIXAMT*32)
			desired=7;
		else if(goodguy->y>me->y+FIXAMT*32)
			desired=1;
		else
			desired=0;
	}
	else
	{
		if(goodguy->y<me->y-FIXAMT*32)
			desired=6;
		else
			desired=2;
	}

	if(desired==me->facing)
		return;

	if(desired>me->facing)
	{
		diff=desired-me->facing;
		if(diff>4)
		{
			dir=-1;
			diff=8-diff;
		}
		else
			dir=1;
	}
	else
	{
		diff=me->facing-desired;
		if(diff>4)
		{
			dir=1;
			diff=8-diff;
		}
		else
			dir=-1;
	}
	me->facing=(me->facing+dir)&7;
}

void UpdateBullet(bullet_t *me,Map *map,world_t *world)
{
	int mapx,mapy;
	int x,y,x2,y2,v,v2;
	byte b;
	word w;

	OffScreenBulletDie(me,map);
	if(!me->type)
		return;

	reflect=0;

	mapx=(me->x/TILE_WIDTH)>>FIXSHIFT;
	mapy=(me->y/TILE_HEIGHT)>>FIXSHIFT;
	me->bright=map->GetTile(mapx,mapy)->templight;

	activeBulDX=me->dx;
	activeBulDY=me->dy;
	b=0;

	if(map->flags&MAP_UNDERWATER)
	{
		me->x+=me->dx*3/4;
		if(!BulletCanGo(me->type,me->x,me->y,map,8,me->friendly))
			BulletHitWallX(me,map,world);
		else
		{
			me->y+=me->dy*3/4;
			if(!BulletCanGo(me->type,me->x,me->y,map,8,me->friendly))
				BulletHitWallY(me,map,world);
		}
		me->z+=me->dz*3/4;
	}
	else
	{
		me->x+=me->dx;
		if(!BulletCanGo(me->type,me->x,me->y,map,8,me->friendly))
			BulletHitWallX(me,map,world);
		else
		{
			me->y+=me->dy;
			if(!BulletCanGo(me->type,me->x,me->y,map,8,me->friendly))
				BulletHitWallY(me,map,world);
		}
		me->z+=me->dz;
	}

	if(me->z<0)
		BulletHitFloor(me,map,world);

	// all gravity-affected bullets, get gravitized
	if(me->type==BLT_HAMMER || me->type==BLT_HAMMER2 || me->type==BLT_BOMB || me->type==BLT_GRENADE
		|| me->type==BLT_ROCK || me->type==BLT_EVILHAMMER || me->type==BLT_SPEAR || me->type==BLT_BADSPEAR
		|| me->type==BLT_BUBBLE || me->type==BLT_BADSITFLAME)
		me->dz-=FIXAMT;

	me->timer--;
	if(!me->timer)
		BulletRanOut(me,map,world);

	if(!me->type)
		return;

	// special things like animation
	switch(me->type)
	{
		case BLT_SKULL:
			me->anim++;
			if(me->timer==50)
				me->target=LockOnEvil(me->x>>FIXSHIFT,me->y>>FIXSHIFT);
			map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,(me->y/TILE_HEIGHT)>>FIXSHIFT,8,4);
			HitBadguys(me,map,world);
			if(!GetGuyPos(me->target,&mapx,&mapy))
				me->target=65535;
			else
			{
				if(me->x>mapx)
					me->dx-=FIXAMT;
				else
					me->dx+=FIXAMT;
				if(me->y>mapy)
					me->dy-=FIXAMT;
				else
					me->dy+=FIXAMT;
				Clamp(&me->dx,8<<FIXSHIFT);
				Clamp(&me->dy,8<<FIXSHIFT);

				if(me->dx>0)
				{
					if(me->facing>8)
						me->facing++;
					else
						me->facing--;
				}
				if(me->dx<0)
				{
					if(me->facing>8)
						me->facing--;
					else
						me->facing++;
				}
				if(me->dy>0)
				{
					if(me->facing>11 || me->facing<4)
						me->facing++;
					else
						me->facing--;
				}
				if(me->dy<0)
				{
					if(me->facing>11 || me->facing<4)
						me->facing--;
					else
						me->facing++;
				}

				if(me->facing>200)
					me->facing+=16;
				if(me->facing>15)
					me->facing-=16;
			}
			break;
		case BLT_FIREBALL:
		case BLT_FIREBALL2:
			me->dz=0;
			me->anim++;
			if(me->anim>4)
			{
				me->anim=0;
				AddParticle(me->x,me->y,me->z,-FIXAMT/2+MGL_randoml(FIXAMT),-FIXAMT/2+MGL_randoml(FIXAMT),0,
							10,PART_SHORTYELLOW,191);
			}
			HitBadguys(me,map,world);
			break;
		case BLT_COMET:
			me->anim++;
			if(me->anim==8)
				me->anim=0;
			break;
		case BLT_COMETBOOM:
			me->anim++;
			HitBadguys(me,map,world);
			break;
		case BLT_LIFEBLIP:
			if(Random(2)==0)
			{
				if(me->type==BLT_LIFEBLIP)
					ColorDrop(1,me->x,me->y,me->z);
			}
			me->dx+=Random(FIXAMT/4+1)-FIXAMT/8;
			me->dy+=Random(FIXAMT/4+1)-FIXAMT/8;
			if(abs(me->x-goodguy->x)<96*FIXAMT && abs(me->y-goodguy->y)<72*FIXAMT)
			{
				if(me->x>goodguy->x)
					me->dx-=FIXAMT/4;
				else
					me->dx+=FIXAMT/4;
				if(me->y>goodguy->y)
					me->dy-=FIXAMT/4;
				else
					me->dy+=FIXAMT/4;
			}
			Clamp(&me->dx,FIXAMT*6);
			Clamp(&me->dy,FIXAMT*4);
			me->dz=0;
			me->z=FIXAMT*20;
			HitBadguys(me,map,world);
			break;
		case BLT_EVILFACE:
			me->anim+=(byte)Random(3);
			if(me->anim>=6*16)
				me->anim=6*16-1;
			me->dx+=-FIXAMT/16+Random(FIXAMT/8);
			me->dy-=Random(FIXAMT/16);
			break;
		case BLT_IGNITE:
			HitBadguys(me,map,world);
			BlowSmoke(me->x,me->y,me->z,FIXAMT);
			break;
		case BLT_LASER2:
			HitBadguys(me,map,world);
			break;
		case BLT_SWAMPGAS:
			me->anim++;
			HitBadguys(me,map,world);
			//return;	// don't let the timer reset!
			break;
		case BLT_CLAW:
		case BLT_ICESHARD:
			HitBadguys(me,map,world);
			break;
		case BLT_WIND:
			if(me->timer==40)
			{
				if(me->friendly)
					me->target=LockOnEvil(me->x>>FIXSHIFT,me->y>>FIXSHIFT);
				else
					me->target=LockOnGood(me->x>>FIXSHIFT,me->y>>FIXSHIFT);
			}
			if(!GetGuyPos(me->target,&mapx,&mapy))
			{
				me->target=65535;
			}
			else
			{
				if(me->timer<40)
				{
					//BulletFaceGuy(me,goodguy);
					me->facing=AngleFrom(me->x,me->y,mapx,mapy);
				}
			}
			me->dx+=Cosine(me->facing);
			me->dy+=Sine(me->facing);
			Dampen(&me->dx,FIXAMT/2);
			Dampen(&me->dy,FIXAMT/2);
			Clamp(&me->dx,FIXAMT*6);
			Clamp(&me->dy,FIXAMT*6);
			HitBadguys(me,map,world);
			BlowSmoke(me->x,me->y-FIXAMT*20,0,FIXAMT);
			//FireBullet(me->x,me->y-FIXAMT*20,0,BLT_MISLSMOKE,me->friendly);
			break;
		case BLT_MEGABOOM:
			map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
						(me->y/TILE_HEIGHT)>>FIXSHIFT,18,64);
			me->anim++;
			if(me->anim<2)
				HitBadguys(me,map,world);
			break;
		case BLT_EARTHSPIKE:
			if(me->timer>22 || me->timer<10)
			{
				me->anim++;
				if(me->anim==15)
					me->type=BLT_NONE;
			}
			if(me->anim==4)
			{
				FireBullet(me->x+Cosine(me->facing)*TILE_WIDTH,me->y+Sine(me->facing)*TILE_HEIGHT,
						((me->facing+(256-16))+(byte)Random(33))&255,BLT_EARTHSPIKE,me->friendly);
			}
			if(me->anim>4 && me->anim<10)
				HitBadguys(me,map,world);
			break;
		case BLT_BIGSHOT:
			me->anim=1-me->anim;	// flip-flop animation
			me->facing+=16;			// spin
			HitBadguys(me,map,world);
			break;
		case BLT_FLAMEWALL:
			map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,8,4);
			me->anim=me->timer%5;
			if(me->anim>3)
				HitBadguys(me,map,world);
			break;
		case BLT_GASBLAST:
			if(me->timer>=10 && me->timer<=50)
			{
				me->anim++;
				if(me->anim>5)
					me->anim=3;
				Dampen(&me->dx,FIXAMT/16);
				Dampen(&me->dy,FIXAMT/16);
				HitBadguys(me,map,world);
			}
			else if(me->timer<6)
			{
				if(me->anim>0)
					me->anim--;
			}
			break;
		case BLT_HOLESHOT:
			me->anim=1-me->anim;
			me->dx=(me->dx*19)/20;
			me->dy=(me->dy*19)/20;
			break;
		case BLT_BLACKHOLE:
			SuckParticle(me->x,me->y,FIXAMT*20);
			map->BrightTorch(mapx,mapy,-10,2);
			SuckIn(me->x,me->y,me->friendly);
			if(Random(2)==0)
				HitBadguys(me,map,world);
			break;
		case BLT_BADSITFLAME:
			if(me->timer&1)	// every other frame
			{
				HitBadguys(me,map,world);
				map->DimTorch((me->x/TILE_WIDTH)>>FIXSHIFT,(me->y/TILE_HEIGHT)>>FIXSHIFT,10);
			}
			map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,8,4);
			me->dz+=FIXAMT/2;		//anti gravity
			Dampen(&me->dx,FIXAMT/8);
			Dampen(&me->dy,FIXAMT/8);
			Clamp(&me->dx,FIXAMT*10);
			Clamp(&me->dy,FIXAMT*10);
			me->anim=Random(5);
			break;
		case BLT_YELWAVE:
			HitBadguys(me,map,world);
			break;
		case BLT_BIGYELLOW:
			me->anim=1-me->anim;
			HitBadguys(me,map,world);
			break;
		case BLT_SLIME:	// frame 259-270
			me->anim++;
			if(me->anim&1)
				HitBadguys(me,map,world);
			break;
		case BLT_ICEBEAM2:
		case BLT_DEATHBEAM2:
			break;
			case BLT_DEATHBEAM:
			if(!BulletCanGo(me->type,me->x,me->y,map,8,me->friendly))
			{
				me->type=BLT_DEATHBEAM2;
				me->timer=8;
				break;
			}
			map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,8,4);
			HitBadguys(me,map,world);
			if(me->timer==4)
				//FireBulletAfter(me->x+Cosine(me->facing)*16,me->y+Sine(me->facing)*16,me->facing,BLT_DEATHBEAM,me,me->friendly);
				FireBulletAfter(me->x,me->y,me->facing,BLT_DEATHBEAM,me,me->friendly);
			me->anim=1-me->anim;
			break;
		case BLT_ICEBEAM:
			if(!BulletCanGo(me->type,me->x,me->y,map,8,me->friendly))
			{
				me->type=BLT_ICEBEAM2;
				me->timer=8;
				break;
			}
			HitBadguys(me,map,world);
			if(me->timer==4)
				//FireBulletAfter(me->x+Cosine(me->facing)*16,me->y+Sine(me->facing)*16,me->facing,BLT_ICEBEAM,me,me->friendly);
				FireBulletAfter(me->x,me->y,me->facing,BLT_ICEBEAM,me,me->friendly);
			me->anim=1-me->anim;
			if(me->anim==0)
				BlowSmoke(me->x,me->y,me->z-MGL_randoml(FIXAMT*4),0);
			break;
		case BLT_LUNA:
		case BLT_LUNA2:
			me->dz=0;
			//ExplodeParticlesColor(4,me->x,me->y,me->z,1,1);
			ColorDrop(4,me->x,me->y,me->z);

			HitBadguys(me,map,world);
			break;
		case BLT_HAMMER:
		case BLT_HAMMER2:
		case BLT_EVILHAMMER:
			me->anim++;
			if(me->anim>15)
				me->anim=0;
			HitBadguys(me,map,world);
			break;
		case BLT_CHEESEHAMMER:
			me->anim++;
			if(me->anim>7)
				me->anim=0;
			me->facing+=6-Random(13);
			me->dx=Cosine(me->facing)*16;
			me->dy=Sine(me->facing)*16;
			HitBadguys(me,map,world);
			break;
		case BLT_SPEAR:
		case BLT_BADSPEAR:
			me->dz+=FIXAMT/2;
			HitBadguys(me,map,world);
			break;
		case BLT_HARPOON:
			me->dz-=FIXAMT/8;
			HitBadguys(me,map,world);
			break;
		case BLT_BUBBLE:
			me->dz+=FIXAMT/2;	// less gravity
			HitBadguys(me,map,world);
			me->anim++;
			if(me->anim>=7*4)
				me->anim=7*4-1;

			if(me->timer==1)
			{
				BulletRanOut(me,map,world);
			}
			break;
		case BLT_BUBBLEPOP:
			me->dz=0;
			me->anim++;
			if(me->anim==4)
				me->type=BLT_NONE;
			break;
		case BLT_PAPER:
			me->anim++;
			if(me->anim==16)
				me->anim=0;
			HitBadguys(me,map,world);
			Dampen(&me->dx,FIXAMT/8);
			Dampen(&me->dy,FIXAMT/8);
			break;
		case BLT_SLASH:
			me->anim++;
			if(me->anim==4)
				BulletRanOut(me,map,world);
			else if(me->anim==2)
			{
				HitBadguys(me,map,world);
				if(GetItem(map->GetTile((me->x>>FIXSHIFT)/TILE_WIDTH,((me->y>>FIXSHIFT)/TILE_HEIGHT))->item)->trigger&ITR_CHOP)
				{
					if(!editing && !player.cheated && verified)
						profile.progress.grassChopped++;
					TriggerItem(NULL,map->GetTile((me->x>>FIXSHIFT)/TILE_WIDTH,((me->y>>FIXSHIFT)/TILE_HEIGHT)),(me->x>>FIXSHIFT)/TILE_WIDTH,(me->y>>FIXSHIFT)/TILE_HEIGHT);
				}
			}
			break;
		case BLT_BIGAXE:
			me->anim++;
			if(me->anim>7)
				me->anim=0;
			HitBadguys(me,map,world);
			break;
		case BLT_BIGSHELL:
		case BLT_SPINE:
			HitBadguys(me,map,world);
			break;
		case BLT_ICESPIKE:
		case BLT_DIRTSPIKE:
			if(me->timer>4)
				me->anim=(8-me->timer);
			else
				me->anim=me->timer-1;
			if(me->timer==5)
				HitBadguys(me,map,world);
			else if(me->timer==4)
			{
				b=(me->facing*32-32+Random(65))&255;
				mapx=(me->x+Cosine(b)*32);
				mapy=(me->y+Sine(b)*32);
				FireBullet(mapx,mapy,me->facing,me->type,me->friendly);
			}
			break;
		case BLT_ROCK:
			me->anim++;
			if(me->anim>3)
				me->anim=0;
			HitBadguys(me,map,world);
			break;
		case BLT_ENERGY:
			me->anim=1-me->anim;	// flip-flop animation
			HitBadguys(me,map,world);
			break;
		case BLT_GREEN:
			HitBadguys(me,map,world);
			break;
		case BLT_LILBOOM:
			HitBadguys(me,map,world);
			break;
		case BLT_LILBOOM2:
			break;
		case BLT_ACID:
		case BLT_SHARK:
		case BLT_FREEZE2:
			me->dz-=FIXAMT/2;	// less gravity than normal things
			me->anim++;
			if(me->anim>6)
				me->anim=0;
			HitBadguys(me,map,world);
			break;
		case BLT_FREEZE:
			me->anim++;
			if(me->anim>6)
				me->anim=0;
			HitBadguys(me,map,world);
			break;
		case BLT_SPORE:
			me->anim++;
			if(me->anim>3*4+3)
				me->anim=3*4+3;
			Dampen(&me->dx,FIXAMT/4);
			Dampen(&me->dy,FIXAMT/4);
			HitBadguys(me,map,world);
			break;
		case BLT_SNOWBALL:
			me->dz-=FIXAMT/2;	// less gravity than normal things
			HitBadguys(me,map,world);
			break;
		case BLT_BIGSNOW:
			me->anim++;
			if(me->anim>5)
				me->anim=0;
			me->dz-=FIXAMT/2;	// less gravity than normal things
			if(me->z<64*FIXAMT)
				HitBadguys(me,map,world);
			break;
		case BLT_GRENADE:
			me->anim=1-me->anim;
			break;
		case BLT_SHOCKWAVE:
			me->anim++;
			HitBadguys(me,map,world);
			break;
		case BLT_YELBOOM:
			me->anim++;
			map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,18,8);
			HitBadguys(me,map,world);
			break;
		case BLT_SHROOM:
			HitBadguys(me,map,world);
			break;
		case BLT_MEGABEAM:
			if(me->anim<(4*5))
				me->anim++;
			else	// FIRE!
				FireBulletAfter(me->x,me->y,0,BLT_MEGABEAM1,me,me->friendly);

			// keep this beam locked onto its launcher
			if(GetGuyPos(me->target,&me->x,&me->y))
			{
				if(GetGuy(me->target) && GetGuy(me->target)->aiType==MONS_DOZER)
					me->y+=FIXAMT*120;
				else
					me->y+=FIXAMT*8;
			}

			break;
		case BLT_MEGABEAM1:
			HitBadguys(me,map,world);
			break;
		case BLT_MEGABEAM2:
			// nothin
			break;
		case BLT_FLAME:
		case BLT_FLAME2:
		case BLT_FLAME3:
			if(me->timer&1)	// every other frame
				HitBadguys(me,map,world);
			map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,8,4);
			me->dz+=Random(FIXAMT/8);		//anti gravity
			me->dx+=Random(FIXAMT)-FIXAMT/2;
			me->dy+=Random(FIXAMT)-FIXAMT/2;
			Dampen(&me->dx,FIXAMT/4);
			Dampen(&me->dy,FIXAMT/4);
			Clamp(&me->dx,FIXAMT*10);
			Clamp(&me->dy,FIXAMT*10);
			me->anim=((32-me->timer)/8)+1;
			if(me->anim>4)
				me->anim=4;
			break;
		case BLT_LASER:
			HitBadguys(me,map,world);
			break;
		case BLT_BOMB:
			me->anim++;
			if(me->anim>15)
				me->anim=0;
			HitBadguys(me,map,world);
			break;
		case BLT_MINE:
			HitBadguys(me,map,world);
			me->anim+=me->facing-1;
			me->timer++;
			if(me->anim==0)
				me->facing=2;
			else if(me->anim==5*4-1)
			{
				me->timer--;
				me->facing=0;
			}
			if(map->flags&MAP_UNDERWATER)	// floating mines
			{
				if(me->z<FIXAMT*20)
					me->dz+=FIXAMT/4;
				else
					me->dz-=FIXAMT/4;
			}
			break;
		case BLT_BOOM:
			map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,18,8);
			HitBadguys(me,map,world);
			break;
		case BLT_SCANSHOT:
			HitBadguys(me,map,world);
			me->anim++;
			if(me->anim==3)
				me->anim=0;
			if(!GetGuyPos(me->target,&mapx,&mapy))
				me->target=65535;
			else
			{
				if(me->x>mapx)
					me->dx-=FIXAMT;
				else
					me->dx+=FIXAMT;
				if(me->y>mapy)
					me->dy-=FIXAMT;
				else
					me->dy+=FIXAMT;

				Clamp(&me->dx,FIXAMT*12);
				Clamp(&me->dy,FIXAMT*12);
			}
			break;
		case BLT_MISSILE:
			if(me->timer==40)
			{
				if(me->friendly)
					me->target=LockOnEvil(me->x>>FIXSHIFT,me->y>>FIXSHIFT);
				else
					me->target=LockOnGood(me->x>>FIXSHIFT,me->y>>FIXSHIFT);
			}

			me->anim++;
			if(((me->anim>0) && (me->target!=65535)) ||
				((me->anim>2) && (me->target==65535)))
			{
				if(map->flags&MAP_UNDERWATER)
					BlowBubble(me->x-me->dx,me->y-me->dy,me->z,FIXAMT/16);
				else
					BlowSmoke(me->x-me->dx,me->y-me->dy,me->z,FIXAMT/16);
				me->anim=0;
			}
			map->BrightTorch((me->x/TILE_WIDTH)>>FIXSHIFT,
							 (me->y/TILE_HEIGHT)>>FIXSHIFT,8,4);

			HitBadguys(me,map,world);
			if(!GetGuyPos(me->target,&mapx,&mapy))
				me->target=65535;
			else
			{
				if(me->x>mapx)
					me->dx-=FIXAMT;
				else
					me->dx+=FIXAMT;
				if(me->y>mapy)
					me->dy-=FIXAMT;
				else
					me->dy+=FIXAMT;

				Clamp(&me->dx,FIXAMT*12);
				Clamp(&me->dy,FIXAMT*12);

				if(me->dx>0)
				{
					if(me->facing>8)
						me->facing++;
					else
						me->facing--;
				}
				if(me->dx<0)
				{
					if(me->facing>8)
						me->facing--;
					else
						me->facing++;
				}
				if(me->dy>0)
				{
					if(me->facing>11 || me->facing<4)
						me->facing++;
					else
						me->facing--;
				}
				if(me->dy<0)
				{
					if(me->facing>11 || me->facing<4)
						me->facing--;
					else
						me->facing++;
				}

				if(me->facing>200)
					me->facing+=16;
				if(me->facing>15)
					me->facing-=16;
			}
			break;
		case BLT_TORPEDO:
			me->anim++;
			if(me->anim==9)
			{
				BlowBubble(me->x-me->dx,me->y-me->dy,me->z,FIXAMT/16);
				me->anim=0;
			}
			me->z+=me->dz;	// don't actually move by dz, it is just used to tell it which way to veer
			HitBadguys(me,map,world);
			me->dx+=Cosine(me->target/16)/2;
			me->dy+=Sine(me->target/16)/2;
			me->target+=me->dz;
			if(me->target>65536-256)
				me->target+=256*16;
			if(me->target>=256*16)
				me->target-=256*16;
			me->facing=me->target/256;
			Clamp(&me->dx,FIXAMT*16);
			Clamp(&me->dy,FIXAMT*16);
			break;
		case BLT_LIGHTNING:
			w=LockOnEvil(me->x>>FIXSHIFT,me->y>>FIXSHIFT);
			if(w!=65535)
			{
				GetGuy(w)->GetShot(0,0,2,map,world);
				LightningBolt(goodguy->x,goodguy->y-FIXAMT*20,GetGuy(w)->x,
					GetGuy(w)->y-FIXAMT*10-GetGuy(w)->z);
				MakeSound(SND_ZAP,GetGuy(w)->x,GetGuy(w)->y,SND_CUTOFF,1000);
			}
			else
			{
				// make sizzle around player if there was no target
				LightningBolt(
					goodguy->x-FIXAMT*32+Random(FIXAMT*64),
					goodguy->y-FIXAMT*52+Random(FIXAMT*64),
					goodguy->x-FIXAMT*32+Random(FIXAMT*64),
					goodguy->y-FIXAMT*52+Random(FIXAMT*64));
			}
			me->type=BLT_NONE;	// begone immediately
			break;
		case BLT_ORBITER:
			me->timer++;
			if(--me->anim==0)
			{
				me->timer--;
				me->anim=15;
				me->target=LockOnEvil(me->x>>FIXSHIFT,me->y>>FIXSHIFT);
				if(me->target!=65535)
				{
					FireBullet(me->x,me->y,me->facing*32,BLT_GREEN,me->friendly);
				}
			}
			if(me->target!=65535)
			{
				BulletFaceGuy(me,GetGuy(me->target));
			}
			if(me->x<goodguy->x)
				me->dx+=FIXAMT;
			if(me->y<goodguy->y)
				me->dy+=FIXAMT;
			if(me->x>goodguy->x)
				me->dx-=FIXAMT;
			if(me->y>goodguy->y)
				me->dy-=FIXAMT;
			Clamp(&me->dx,FIXAMT*9);
			Clamp(&me->dy,FIXAMT*9);
			break;
		case BLT_ORBITER2:
			me->timer++;
			if(--me->anim==0)
			{
				me->timer--;
				me->anim=45;
				me->target=LockOnEvil(me->x>>FIXSHIFT,me->y>>FIXSHIFT);
				if(me->target!=65535)
				{
					FireBullet(me->x,me->y,me->facing*32,BLT_BOMB,me->friendly);
				}
			}
			if(me->target!=65535)
			{
				BulletFaceGuy(me,GetGuy(me->target));
			}
			if(me->x<goodguy->x)
				me->dx+=FIXAMT;
			if(me->y<goodguy->y)
				me->dy+=FIXAMT;
			if(me->x>goodguy->x)
				me->dx-=FIXAMT;
			if(me->y>goodguy->y)
				me->dy-=FIXAMT;
			Clamp(&me->dx,FIXAMT*9);
			Clamp(&me->dy,FIXAMT*9);
			break;
		case BLT_BALLLIGHTNING:
			x=me->x;
			y=me->y-me->z;
			v=Random(256);
			v2=Random(16)+2;
			x2=(x+Cosine(v)*v2);
			y2=(y+Sine(v)*v2);
			v=Random(256);
			v2=Random(16)+2;
			x=(x+Cosine(v)*v2);
			y=(y+Sine(v)*v2);
			LightningBolt(x,y,x2,y2);
			ExplodeParticles(PART_SNOW2,me->x,me->y,me->z,1);
			HitBadguys(me,map,world);
			break;
		case BLT_LIGHTNING2:
			w=LockOnEvil2(me->x>>FIXSHIFT,me->y>>FIXSHIFT);
			if(w!=65535)
			{
				GetGuy(w)->GetShot(0,0,5,map,world);
				LightningBolt(me->x,me->y-FIXAMT*20,GetGuy(w)->x,
					GetGuy(w)->y-FIXAMT*10-GetGuy(w)->z);
				MakeSound(SND_ZAP,GetGuy(w)->x,GetGuy(w)->y,SND_CUTOFF,1000);
			}
			else
			{
				// zap to nowhere
				LightningBolt(
					me->x,
					me->y,
					me->x-FIXAMT*64+Random(FIXAMT*128),
					me->y-FIXAMT*64+Random(FIXAMT*128));
			}
			me->type=BLT_NONE;	// begone immediately
			break;
		case BLT_MINDWIPE:
			me->anim++;
			if(me->anim>2)
				me->anim=0;

			HitBadguys(me,map,world);
			me->bright=(char)Random(16);
			break;
		case BLT_REFLECT:
			me->anim++;
			//map->BrightTorch(me->x/(TILE_WIDTH*FIXAMT),me->y/(TILE_HEIGHT*FIXAMT),me->timer,4);
			if(me->anim<3)
				HitBadguys(me,map,world);
			break;
		case BLT_SWAP:
			me->anim+=8;
			HitBadguys(me,map,world);
			break;
		case BLT_SCANNER:
			me->anim++;
			if(me->anim==3)
				me->anim=0;
			HitBadguys(me,map,world);
			break;
		case BLT_SCANLOCK:
			if(me->target!=65535 && GetGuy(me->target)->type!=MONS_NONE)
			{
				me->x=GetGuy(me->target)->x;
				me->y=GetGuy(me->target)->y;
				me->z=GetGuy(me->target)->z;
				if(me->z==0)
					me->z=FIXAMT*20;
			}
			else
			{
				me->target=65535;
				if(me->timer>3)
					me->timer=3;	// go away if not on a target
			}
			me->dx=me->dy=me->dz=0;
			break;
	}
}

void RenderSmoke(int x,int y,int z,char bright,byte frm)
{
	SprDraw(x,y,z,255,bright-64,bulletSpr->GetSprite(SPR_SMOKE+frm),DISPLAY_DRAWME|DISPLAY_GHOST);
}

void RenderBubble(int x,int y,int z,char bright,byte frm)
{
	SprDraw(x,y,z,255,bright-64,bulletSpr->GetSprite(SPR_BUBBLE+frm),DISPLAY_DRAWME|DISPLAY_GLOW);
}

void RenderMindControl(int x,int y,int z,char bright,byte frm)
{
	SprDraw(x,y,z,255,bright-64,bulletSpr->GetSprite(SPR_SCANSHOT+frm),DISPLAY_DRAWME|DISPLAY_GLOW);
}

void RenderBoom(int x,int y,int z,char bright,byte frm)
{
	SprDraw(x,y,z,255,bright-64,bulletSpr->GetSprite(SPR_BOOM+frm),DISPLAY_DRAWME|DISPLAY_GLOW);
}

void RenderStinky(int x,int y,int z,char bright,byte frm)
{
	SprDraw(x,y,z,255,bright-64,bulletSpr->GetSprite(SPR_STINKY+frm),DISPLAY_DRAWME|DISPLAY_GHOST);
}

void RenderBullet(bullet_t *me)
{
	int v;
	sprite_t *curSpr;

	switch(me->type)
	{
		case BLT_SKULL:
			curSpr=bulletSpr->GetSprite(SPR_SKULL+me->facing);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright-2,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_FIREBALL:
		case BLT_FIREBALL2:
			v=me->facing*5+(me->anim)+SPR_FIREBALL;
			curSpr=bulletSpr->GetSprite(v);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_COMET:
			curSpr=bulletSpr->GetSprite(SPR_COMET+me->anim);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,0,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_COMETBOOM:
			curSpr=bulletSpr->GetSprite(SPR_COMETBOOM+me->anim/2);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,0,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_EVILFACE:
			curSpr=bulletSpr->GetSprite(me->anim/16+SPR_EVILFACE);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GHOST);
			break;
		case BLT_IGNITE:
			// shown only by its smoke
			break;
		case BLT_LASER2:
			curSpr=bulletSpr->GetSprite(((me->facing+8)/16)+SPR_LASER);
			for(v=0;v<3;v++)
			{
				// z=me->z;//+Random(FIXAMT*8);
				// x=me->x-FIXAMT*10+Random(FIXAMT*20+1);
				// y=me->y-FIXAMT*10+Random(FIXAMT*20+1);

				SprDraw((me->x-FIXAMT*10+Random(FIXAMT*20+1))>>FIXSHIFT,
				(me->y-FIXAMT*10+Random(FIXAMT*20+1))>>FIXSHIFT,
				(me->z)>>FIXSHIFT,255,me->bright,curSpr,
						DISPLAY_DRAWME|DISPLAY_GLOW);
			}
			break;
		case BLT_ICESHARD:
			curSpr=bulletSpr->GetSprite((me->facing/16)+SPR_SPINE);
			SprDrawOff(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,5,0,me->bright+6,curSpr,
					DISPLAY_DRAWME|DISPLAY_OFFCOLOR);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_SWAMPGAS:
			curSpr=bulletSpr->GetSprite(SPR_SWAMPGAS+me->anim/4);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			// else don't draw it at all
			break;
		case BLT_CLAW:
			curSpr=bulletSpr->GetSprite((me->facing/16)+SPR_CLAW);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_WIND:
			// no image
			break;
		case BLT_MEGABOOM:
			curSpr=bulletSpr->GetSprite(SPR_MEGABOOM+me->anim);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_EARTHSPIKE:
			curSpr=bulletSpr->GetSprite(SPR_EATHSPIKE+me->anim/2);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_BIGSHOT:
			curSpr=bulletSpr->GetSprite(me->anim+SPR_ENERGY);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			// extra deals
			SprDraw((me->x+Cosine(me->facing)*10)>>FIXSHIFT,
				(me->y+Sine(me->facing)*10)>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw((me->x+Cosine(me->facing)*10)>>FIXSHIFT,
				(me->y+Sine(me->facing)*10)>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			SprDraw((me->x+Cosine(me->facing+86)*10)>>FIXSHIFT,
				(me->y+Sine(me->facing+86)*10)>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw((me->x+Cosine(me->facing+86)*10)>>FIXSHIFT,
				(me->y+Sine(me->facing+86)*10)>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			SprDraw((me->x+Cosine(me->facing+172)*10)>>FIXSHIFT,
				(me->y+Sine(me->facing+172)*10)>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw((me->x+Cosine(me->facing+172)*10)>>FIXSHIFT,
				(me->y+Sine(me->facing+172)*10)>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_FLAMEWALL:
			curSpr=bulletSpr->GetSprite(SPR_FLAMEWALL + me->anim/2);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_GASBLAST:
			curSpr=bulletSpr->GetSprite(SPR_POISONGAS+me->anim);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_YELWAVE:
			curSpr=bulletSpr->GetSprite(SPR_WAVE+(me->facing));
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_BIGYELLOW:
			curSpr=bulletSpr->GetSprite(me->anim+SPR_GRENADE);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_SLIME:
			curSpr=bulletSpr->GetSprite(SPR_SLIME+(me->anim/16));
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright-4,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_ICEBEAM:
			curSpr=bulletSpr->GetSprite(((me->facing)&3)+SPR_ICEBEAM+1);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_ICEBEAM2:
			curSpr=bulletSpr->GetSprite(SPR_ICEBEAM);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_DEATHBEAM:
			curSpr=bulletSpr->GetSprite(((me->facing)&3)+SPR_ICEBEAM+1);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,4,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
			case BLT_DEATHBEAM2:
			curSpr=bulletSpr->GetSprite(SPR_ICEBEAM);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,4,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_HAMMER:
		case BLT_HAMMER2:
			v=me->facing*16+(me->anim)+SPR_HAMMER;
			curSpr=bulletSpr->GetSprite(v);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_CHEESEHAMMER:
			v=(((me->facing/32)+me->anim)&7)*16+SPR_HAMMER;
			curSpr=bulletSpr->GetSprite(v);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			SprDrawOff(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,4,5,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_EVILHAMMER:
			v=me->facing*16+(me->anim)+SPR_HAMMER;
			curSpr=bulletSpr->GetSprite(v);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,0,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_ACID:
			v=me->facing*7+me->anim+SPR_ACID;
			curSpr=bulletSpr->GetSprite(v);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_SHARK:
			v=me->facing*7+me->anim+SPR_ACID;
			curSpr=bulletSpr->GetSprite(v);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			SprDrawOff(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,1,3,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_FREEZE:
		case BLT_FREEZE2:
			v=me->facing*7+me->anim+SPR_ACID;
			curSpr=bulletSpr->GetSprite(v);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			SprDrawOff(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,1,7,me->bright+6,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_MISSILE:
			v=me->facing+SPR_MISSILE;
			curSpr=bulletSpr->GetSprite(v);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_TORPEDO:
			v=(me->facing)+SPR_MISSILE;
			curSpr=bulletSpr->GetSprite(v);
			SprDrawOff(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,0,4,me->bright+3,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_FLAME:
		case BLT_FLAME2:
		case BLT_FLAME3:
		case BLT_BADSITFLAME:
			curSpr=bulletSpr->GetSprite(me->anim+SPR_FLAME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,0,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_LASER:
			curSpr=bulletSpr->GetSprite(me->facing+SPR_LASER);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_PAPER:
			curSpr=bulletSpr->GetSprite(me->anim/2+SPR_PAPER);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_BUBBLE:
			curSpr=bulletSpr->GetSprite(me->anim/4+SPR_BUBBLE);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_BUBBLEPOP:
			curSpr=bulletSpr->GetSprite(me->anim+SPR_BUBBLE+5);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_BOMB:
			curSpr=bulletSpr->GetSprite(me->anim/2+SPR_BOMB);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_BOOM:
			curSpr=bulletSpr->GetSprite(7-me->timer+SPR_BOOM);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_ENERGY:
			curSpr=bulletSpr->GetSprite(me->anim+SPR_ENERGY);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_MEGABEAM:
			curSpr=bulletSpr->GetSprite((me->anim>>2)+SPR_MEGABEAM);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright-8,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_MEGABEAM1:
			curSpr=bulletSpr->GetSprite(6+SPR_MEGABEAM);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright-8,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_MEGABEAM2:
			curSpr=bulletSpr->GetSprite(7+SPR_MEGABEAM);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright-8,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_SPORE:
			curSpr=bulletSpr->GetSprite(me->anim/4+SPR_SPORE);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_SHROOM:
			curSpr=bulletSpr->GetSprite(me->facing+SPR_SHROOM);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_HOLESHOT:
		case BLT_GRENADE:
			curSpr=bulletSpr->GetSprite(me->anim+SPR_GRENADE);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_YELBOOM:
			curSpr=bulletSpr->GetSprite(me->anim/2+SPR_YELBOOM);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_SHOCKWAVE:
			curSpr=bulletSpr->GetSprite(me->anim/2+SPR_SHOCKWAVE);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_LILBOOM:
		case BLT_LILBOOM2:
			curSpr=bulletSpr->GetSprite(4-(me->timer/2)+SPR_LILBOOM);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_SNOWBALL:
			curSpr=bulletSpr->GetSprite(SPR_SNOWBALL);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_BIGSNOW:
			curSpr=bulletSpr->GetSprite(SPR_BIGSNOW+(me->anim/2));
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_ICESPIKE:
			curSpr=bulletSpr->GetSprite(SPR_ICESPIKE+(me->anim));
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_DIRTSPIKE:
			curSpr=bulletSpr->GetSprite(SPR_ICESPIKE+(me->anim));
			SprDrawOff(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,0,2,me->bright-12,curSpr,
					DISPLAY_DRAWME|DISPLAY_OFFCOLOR);
			break;
		case BLT_ROCK:
			if((me->timer<8) && (me->timer&1))
				return;	// flicker when almost gone
			curSpr=bulletSpr->GetSprite(SPR_ROCK+(me->anim));
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_SPINE:
			curSpr=bulletSpr->GetSprite(SPR_SPINE+(me->facing));
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			break;
		case BLT_BIGSHELL:
			curSpr=bulletSpr->GetSprite(SPR_BOOM);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_BIGAXE:
			curSpr=bulletSpr->GetSprite(SPR_BIGAXE+me->anim);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_SPEAR:
		case BLT_BADSPEAR:
		case BLT_HARPOON:
			curSpr=bulletSpr->GetSprite(SPR_SPEAR+me->facing);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_SLASH:
			if(me->anim>0)
			{
				curSpr=bulletSpr->GetSprite(SPR_SLASH+me->facing*3+me->anim-1);
				SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GHOST);
			}
			break;
		case BLT_MINE:
			curSpr=bulletSpr->GetSprite(SPR_MINE+me->anim/4);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			if(curMap->flags&MAP_UNDERWATER)	// underwater, it has a shadow, since it floats
				SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_ORBITER:
			curSpr=bulletSpr->GetSprite(SPR_ORBITER+me->facing);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_ORBITER2:
			curSpr=bulletSpr->GetSprite(SPR_ORBITER+me->facing);
			SprDrawOff(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,1,4,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_GREEN:
			curSpr=bulletSpr->GetSprite(SPR_GREEN);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_SCANSHOT:
		case BLT_SCANNER:
			curSpr=bulletSpr->GetSprite(SPR_SCANSHOT+me->anim);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_SCANLOCK:
			if(me->timer>10 || (me->timer&1))
			{
				curSpr=bulletSpr->GetSprite(SPR_SCANLOCK);
				SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
						DISPLAY_DRAWME|DISPLAY_GLOW);
			}
			break;
		case BLT_BALLLIGHTNING:
			break;
		case BLT_MINDWIPE:
			curSpr=bulletSpr->GetSprite(SPR_BOOM+me->anim);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			break;
		case BLT_REFLECT:
			curSpr=bulletSpr->GetSprite(me->anim/2+SPR_YELBOOM);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_GLOW);
			break;
		case BLT_SWAP:
			int x,y;

			curSpr=bulletSpr->GetSprite(SPR_GREEN);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
			SprDraw(me->x>>FIXSHIFT,me->y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			for(v=0;v<4;v++)
			{
				x=me->x+Cosine((me->anim+v*64)&255)*6;
				y=me->y+Sine((me->anim+v*64)&255)*6;
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,me->z>>FIXSHIFT,255,me->bright,curSpr,
					DISPLAY_DRAWME);
				SprDraw(x>>FIXSHIFT,y>>FIXSHIFT,0,255,me->bright,curSpr,
					DISPLAY_DRAWME|DISPLAY_SHADOW);
			}
			break;
	}
}

void UpdateBullets(Map *map,world_t *world)
{
	int i;

	for(i=0;i<config.numBullets;i++)
		if(bullet[i].type)
			UpdateBullet(&bullet[i],map,world);
}

void RenderBullets(void)
{
	int i;

	for(i=0;i<config.numBullets;i++)
		if(bullet[i].type)
			RenderBullet(&bullet[i]);
}

void FireMe(bullet_t *me,int x,int y,byte facing,byte type,byte friendly)
{
	int f;

	me->friendly=friendly;
	me->type=type;
	me->x=x;
	me->y=y;
	me->facing=facing;
	me->bright=0;

	switch(me->type)
	{
		case BLT_SKULL:
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*20;
			me->dz=0;
			me->target=65535;
			me->dx=Cosine(me->facing)*6;
			me->dy=Sine(me->facing)*6;
			me->facing=facing/16;
			break;
		case BLT_FIREBALL:
		case BLT_FIREBALL2:
			me->anim=0;
			me->timer=30;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing*32)*12;
			me->dy=Sine(me->facing*32)*12;
			me->dz=0;//FIXAMT*10;
			break;
		case BLT_COMET:
			me->anim=(byte)MGL_random(8);
			me->timer=255;
			me->z=400*FIXAMT+MGL_randoml(300*FIXAMT);
			me->dx=0;
			me->dy=0;
			me->dz=-FIXAMT*50;
			break;
		case BLT_LIFEBLIP:
			me->facing=Random(256);
			me->dx=Cosine(me->facing);
			me->dy=Sine(me->facing);
			me->dz=0;
			me->z=FIXAMT*20;
			me->anim=0;
			me->timer=255;
			break;
		case BLT_EVILFACE:
			me->anim=0;
			me->dx=-FIXAMT+Random(FIXAMT*2);
			me->dy=-FIXAMT-Random(FIXAMT*2);
			me->dz=0;
			me->z=40*FIXAMT;
			me->timer=30*10;
			break;
		case BLT_IGNITE:
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing)*12;
			me->dy=Sine(me->facing)*12;
			me->dz=0;
			break;
		case BLT_LASER2:
			me->anim=0;
			me->timer=30*10;
			me->facing=me->facing*32;
			me->dx=Cosine(me->facing)*32;
			me->dy=Sine(me->facing)*24;
			me->x+=Cosine(me->facing)*(Random(10));
			me->y+=Sine(me->facing)*(Random(8));
			me->dz=0;
			me->target=255+255*256;
			break;
		case BLT_ICESHARD:
			me->anim=0;
			me->timer=30;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing)*12;
			me->dy=Sine(me->facing)*12;
			me->dz=0;
			break;
		case BLT_SWAMPGAS:
			me->anim=0;
			me->timer=16;
			me->z=FIXAMT*2;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			break;
		case BLT_CLAW:
			me->anim=0;
			me->timer=30*5;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing)*12;
			me->dy=Sine(me->facing)*12;
			me->dz=0;
			break;
		case BLT_WIND:
			me->target=65535;
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*20;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			break;
		case BLT_MEGABOOM:
			MakeSound(SND_BIGBOOM,me->x,me->y,SND_CUTOFF,1200);
			me->anim=0;
			me->timer=10;
			me->z=FIXAMT*20;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			break;
		case BLT_EARTHSPIKE:
			me->anim=0;
			me->timer=30;
			me->z=0;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			break;
		case BLT_BIGSHOT:
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing)*6;
			me->dy=Sine(me->facing)*6;
			me->dz=0;
			break;
		case BLT_FLAMEWALL:
			me->anim=0;
			me->timer=30;
			me->z=0;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,600);
			break;
		case BLT_GASBLAST:
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing)*4;
			me->dy=Sine(me->facing)*4;
			me->dz=0;
			break;
		case BLT_HOLESHOT:
			me->z=FIXAMT*20;
			me->dx=Cosine(facing)*8;
			me->dy=Sine(facing)*8;
			me->dz=0;
			me->anim=0;
			me->timer=60;
			break;
		case BLT_BADSITFLAME:
			me->anim=0;
			me->timer=30+Random(30*15);
			me->z=FIXAMT*20;
			me->facing=Random(256);
			me->x+=((Random(3)-1)<<FIXSHIFT)+Cosine(me->facing)*5;
			me->y+=((Random(3)-1)<<FIXSHIFT)+Sine(me->facing)*5;
			f=Random(FIXAMT*6)+FIXAMT;
			me->dx=Cosine(me->facing)*f/FIXAMT;
			me->dy=Sine(me->facing)*f/FIXAMT;
			me->dz=Random(FIXAMT*4)+FIXAMT;
			MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,1100);
			break;
		case BLT_YELWAVE:
			me->anim=0;
			me->timer=15;
			me->z=FIXAMT*2;
			me->dz=0;
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
			break;
		case BLT_BIGYELLOW:
			me->anim=0;
			me->timer=30*10;
			me->z=FIXAMT*30;
			me->dz=0;
			me->dx=Cosine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*4;
			break;
		case BLT_SLIME:
			me->anim=0;
			me->timer=192;
			me->z=0;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			break;
		case BLT_ICEBEAM:
			me->anim=0;
			me->timer=20;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing*32)*26;
			me->dy=Sine(me->facing*32)*26;
			me->dz=0;
			break;
		case BLT_DEATHBEAM:
			me->anim=0;
			me->timer=20;
			me->z=FIXAMT*45;
			if(me->facing==32*0 || me->facing==32*4)
				me->z=FIXAMT*50;
			me->dx=Cosine(me->facing*32)*26;
			me->dy=Sine(me->facing*32)*26;
			me->dz=0;
			break;
		case BLT_DEATHBEAM2:
			me->anim=0;
			me->timer=20;
			me->z=FIXAMT*40;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			break;
			case BLT_ICEBEAM2:
			me->anim=0;
			me->timer=20;
			me->z=FIXAMT*40;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			break;
		case BLT_SCANSHOT:
			me->facing=Random(256);
			me->dx=Cosine(me->facing)*4;
			me->dy=Sine(me->facing)*4;
			me->dz=0;
			me->anim=0;
			me->timer=60;
			break;
		case BLT_SCANNER:
			me->dx=Cosine(facing*32)*16;
			me->dy=Sine(facing*32)*16;
			me->dz=0;
			me->anim=0;
			me->timer=60;
			me->facing=facing*32;
			break;
		case BLT_SWAP:
			me->dx=Cosine(facing*32)*10;
			me->dy=Sine(facing*32)*10;
			me->dz=0;
			me->anim=0;
			me->timer=120;
			me->facing=facing*32;
			break;
		case BLT_REFLECT:
			me->dx=0;
			me->dy=0;
			me->z=0;
			me->anim=0;
			me->timer=9;
			break;
		case BLT_BOOM:
			me->dx=0;
			me->dy=0;
			me->z=0;
			me->anim=0;
			me->timer=7;
			MakeSound(SND_BOMBBOOM,me->x,me->y,SND_CUTOFF,1800);
			break;
		case BLT_BALLLIGHTNING:
			me->timer=30;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing*32)*11;
			me->dy=Sine(me->facing*32)*11;
			me->dz=0;
			break;
		case BLT_HAMMER:
		case BLT_HAMMER2:
			me->anim=0;
			me->timer=30;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing*32)*12;
			me->dy=Sine(me->facing*32)*12;
			me->dz=FIXAMT*10;
			break;
		case BLT_LUNA:
			me->anim=0;
			me->timer=30;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing*32)*12;
			me->dy=Sine(me->facing*32)*12;
			me->dz=0;
			break;
		case BLT_CHEESEHAMMER:
			me->anim=0;
			me->timer=50;
			me->z=FIXAMT*15;
			me->dx=Cosine(me->facing*32)*14;
			me->dy=Sine(me->facing*32)*14;
			me->dz=0;
			break;
		case BLT_EVILHAMMER:
			me->anim=0;
			me->timer=30;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing*32)*12;
			me->dy=Sine(me->facing*32)*12;
			me->dz=FIXAMT*10;
			break;
		case BLT_BOMB:
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing)*8;
			me->dy=Sine(me->facing)*8;
			me->dz=FIXAMT*5;
			break;
		case BLT_MISSILE:
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*20;
			me->dz=0;
			me->facing=facing*2;
			me->target=65535;
			f=me->facing;
			f+=Random(5)-2;
			if(f<0)
				f+=16;
			me->facing=(byte)(f&15);
			me->x+=((Random(17)-8)<<FIXSHIFT);
			me->y+=((Random(17)-8)<<FIXSHIFT);
			me->dx=Cosine(me->facing*16)*4;
			me->dy=Sine(me->facing*16)*4;
			MakeSound(SND_MISSILELAUNCH,me->x,me->y,SND_CUTOFF,1100);
			break;
		case BLT_TORPEDO:
			me->anim=Random(8);
			me->timer=60;
			me->z=FIXAMT*20;
			me->dz=-6+Random(13);
			f=facing*32*16;
			f+=Random(33)-16;
			if(f<0)
				f+=256*16;
			me->target=f%(256*16);
			me->facing=me->target/256;
			me->x+=((Random(17)-8)<<FIXSHIFT);
			me->y+=((Random(17)-8)<<FIXSHIFT);
			me->dx=0;
			me->dy=0;
			break;
		case BLT_ACID:
		case BLT_SHARK:
		case BLT_FREEZE2:
			me->anim=0;
			me->timer=30;
			me->z=FIXAMT*10;
			me->dz=FIXAMT*4;
			me->dx=Cosine(me->facing)*10;
			me->dy=Sine(me->facing)*10;
			me->facing=((me->facing+16)&255)/32;
			break;
		case BLT_FREEZE:
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*10;
			me->dz=0;
			me->dx=Cosine(me->facing)*8;
			me->dy=Sine(me->facing)*8;
			me->facing=((me->facing+16)&255)/32;
			break;
		case BLT_FLAME:
		case BLT_FLAME2:
			me->anim=0;
			me->timer=24-Random(4);
			me->z=FIXAMT*20;
			me->x+=((Random(3)-1)<<FIXSHIFT)+Cosine(me->facing*32)*5;
			me->y+=((Random(3)-1)<<FIXSHIFT)+Sine(me->facing*32)*5;
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
			me->dz=-FIXAMT/2;
			if(Random(5)==0)
				MakeSound(SND_FLAMEGO,me->x,me->y,SND_CUTOFF,1100);
			break;
		case BLT_FLAME3:
			me->anim=4;
			me->timer=24-Random(4);
			me->z=FIXAMT*20;
			me->facing=Random(256);
			me->x+=((Random(3)-1)<<FIXSHIFT)+Cosine(me->facing)*5;
			me->y+=((Random(3)-1)<<FIXSHIFT)+Sine(me->facing)*5;
			me->dx=Cosine(me->facing);
			me->dy=Sine(me->facing);
			me->dz=FIXAMT/2;
			break;
		case BLT_LASER:
			me->anim=0;
			me->timer=30;
			me->z=FIXAMT*20-Random(FIXAMT);
			me->x+=((Random(3)-1)<<FIXSHIFT);
			me->y+=((Random(3)-1)<<FIXSHIFT);
			me->x+=FIXAMT/2-Random(FIXAMT);
			me->y+=FIXAMT/2-Random(FIXAMT);
			me->facing=me->facing*32+4-Random(9);
			me->dx=Cosine(me->facing)*24;
			me->dy=Sine(me->facing)*24;
			me->facing/=16;
			me->dz=0;
			MakeSound(SND_BULLETFIRE,me->x,me->y,SND_CUTOFF,1050);
			break;
		case BLT_ENERGY:
			me->anim=0;
			me->timer=30;
			me->z=FIXAMT*20;
			// this thing receives full 0-255 facings
			me->dx=Cosine(me->facing)*8;
			me->dy=Sine(me->facing)*8;
			me->dz=0;
			//MakeSound(SND_ENERGYFIRE,me->x,me->y,SND_CUTOFF,950);
			break;
		case BLT_MEGABEAM:
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*32;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			MakeSound(SND_MEGABEAM,me->x,me->y,SND_CUTOFF,5000);
			break;
		case BLT_MEGABEAM1:
			me->anim=0;
			me->timer=40;
			me->z=FIXAMT*32;
			me->dx=0;
			me->dy=32*FIXAMT;	// always 32 pixels to make a continuous not-too-overlapping beam
			me->dz=0;
			break;
		case BLT_SPORE:
			me->anim=0;
			me->timer=16;
			me->z=FIXAMT*16;
			me->dx=Cosine(me->facing)*4;
			me->dy=Sine(me->facing)*4;
			me->dz=0;
			break;
		case BLT_SHROOM:
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*32;
			me->dx=Cosine(me->facing*32)*8;
			me->dy=Sine(me->facing*32)*8;
			me->dz=0;
			break;
		case BLT_GRENADE:
			me->anim=0;
			me->timer=255;
			me->z=FIXAMT*80;
			f=Random(12)+1;
			me->dx=Cosine(me->facing)*f;
			me->dy=Sine(me->facing)*f;
			me->dz=FIXAMT*20;
			break;
		case BLT_SHOCKWAVE:
			me->anim=0;
			me->timer=7;
			me->z=0;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			break;
		case BLT_SNOWBALL:
			me->anim=0;
			me->timer=30;
			me->z=FIXAMT*20;
			// this thing receives full 0-255 facings
			me->dx=Cosine(me->facing)*8;
			me->dy=Sine(me->facing)*8;
			me->dz=FIXAMT*4;
			//MakeSound(SND_ENERGYFIRE,me->x,me->y,SND_CUTOFF,950);
			break;
		case BLT_BIGSNOW:
			me->anim=0;
			me->timer=30;
			me->z=FIXAMT*20;
			// this thing receives full 0-255 facings
			me->dx=Cosine(me->facing)*8;
			me->dy=Sine(me->facing)*8;
			me->dz=FIXAMT*4;
			//MakeSound(SND_ENERGYFIRE,me->x,me->y,SND_CUTOFF,950);
			break;
		case BLT_ICESPIKE:
		case BLT_DIRTSPIKE:
			me->anim=0;
			me->timer=9;
			me->z=0;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			break;
		case BLT_ROCK:
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing*32)*6;
			me->dy=Sine(me->facing*32)*6;
			me->dz=FIXAMT*8;
			//MakeSound(SND_ENERGYFIRE,me->x,me->y,SND_CUTOFF,950);
			break;
		case BLT_SPINE:
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*20;
			// this takes 0-255 directional facings
			me->dx=Cosine(me->facing)*8;
			me->dy=Sine(me->facing)*8;
			me->facing/=16;
			me->dz=0;
			break;
		case BLT_BIGSHELL:
			me->timer=30;
			me->z=FIXAMT*50;
			me->dx=Cosine(me->facing)*12;
			me->dy=Sine(me->facing)*12;
			me->dz=-FIXAMT*3;
			break;
		case BLT_BIGAXE:
			me->anim=0;
			me->timer=90;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing*32)*11;
			me->dy=Sine(me->facing*32)*11;
			me->dz=0;
			break;
		case BLT_LIGHTNING:
		case BLT_LIGHTNING2:
			me->timer=2;
			break;
		case BLT_SPEAR:
		case BLT_BADSPEAR:
			me->timer=60;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing*32)*9;
			me->dy=Sine(me->facing*32)*9;
			me->dz=FIXAMT*6;
			break;
		case BLT_HARPOON:
			me->timer=90;
			me->z=FIXAMT*10;
			me->dx=Cosine(me->facing*32)*12;
			me->dy=Sine(me->facing*32)*12;
			me->dz=FIXAMT;
			break;
		case BLT_SLASH:
			me->dx=0;
			me->dy=0;
			me->dz=0;
			me->timer=10;
			me->anim=0;
			me->z=FIXAMT*20;
			break;
		case BLT_MINE:
			me->anim=0;
			me->facing=2;
			me->timer=50;
			me->z=0;
			me->dx=0;
			me->dy=0;
			me->dz=0;
			if(curMap->flags&MAP_UNDERWATER)
				me->z=FIXAMT*10;
			break;
		case BLT_GREEN:
			me->anim=0;
			me->timer=30;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing)*8;
			me->dy=Sine(me->facing)*8;
			me->dz=0;
			break;
		case BLT_ORBITER:
		case BLT_ORBITER2:
			me->anim=15;
			me->timer=40;
			if(me->type==BLT_ORBITER2)
				me->timer=10;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing*32)*8+Sine(me->facing*32)*4;
			me->dy=Sine(me->facing*32)*8+Cosine(me->facing*32)*4;
			me->dz=0;
			me->target=65535;
			break;
		case BLT_MINDWIPE:
			me->anim=0;
			me->timer=40;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing*32)*10;
			me->dy=Sine(me->facing*32)*10;
			me->dz=0;
			break;
		case BLT_PAPER:
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing)*9;
			me->dy=Sine(me->facing)*9;
			me->dz=0;
			break;
		case BLT_BUBBLE:
			me->anim=0;
			me->timer=40+Random(30);
			me->z=FIXAMT*20;
			me->dx=Cosine(me->facing)*5;
			me->dy=Sine(me->facing)*5;
			me->dz=FIXAMT*4;
			break;

		case BLT_LILBOOM:
		case BLT_LILBOOM2:
			// Fix the little booms to show proper sprites when fired manually
			me->timer=9;
			break;

		case BLT_BUBBLEPOP:
			// Likewise with bubble pops
			me->timer=10;
			break;
	}
}

void QuadMissile(int x,int y,byte facing,byte friendly)
{
	int i;
	byte f;
	bullet_t *me;

	f=(facing*2-2)&15;

	for(i=0;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			me=&bullet[i];
			me->type=BLT_MISSILE;
			me->friendly=friendly;
			me->x=x;
			me->y=y;
			me->facing=f;
			me->bright=0;
			me->anim=0;
			me->timer=60;
			me->z=FIXAMT*20;
			me->dz=0;
			me->target=65535;
			me->x+=Cosine(me->facing*16)*10;
			me->y+=Sine(me->facing*16)*10;
			me->dx=Cosine(me->facing*16)*4;
			me->dy=Sine(me->facing*16)*4;
			f=(f+1)&15;
			if(f==facing*2)
				f=(f+1)&15;
			if(f==((facing*2+3)&15))
				break;
		}
	MakeSound(SND_MISSILELAUNCH,x,y,SND_CUTOFF,1100);
}


void LaunchMegabeam(int x,int y,word owner)
{
	int i;

	for(i=0;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			FireMe(&bullet[i],x,y,0,BLT_MEGABEAM,GetGuy(owner)->friendly);
			bullet[i].target=owner;
			break;
		}
}

void SpitAcid(int x,int y,byte facing,byte type,byte friendly)
{
	int i;

	for(i=0;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			bullet[i].friendly=friendly;
			bullet[i].type=type;
			bullet[i].x=x;
			bullet[i].y=y;
			bullet[i].facing=facing;
			bullet[i].bright=0;
			bullet[i].anim=0;
			bullet[i].timer=30;
			bullet[i].z=FIXAMT*30;
			bullet[i].dz=FIXAMT*3;
			bullet[i].dx=Cosine(bullet[i].facing)*10;
			bullet[i].dy=Sine(bullet[i].facing)*10;
			bullet[i].facing=((bullet[i].facing+16)&255)/32;
			break;
		}
}

void FireBullet(int x,int y,byte facing,byte type,byte friendly)
{
	int i;

	for(i=0;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			FireMe(&bullet[i],x,y,facing,type,friendly);
			break;
		}
}

void FireScanShots(Guy *victim)
{
	int i;
	byte count=0;

	for(i=0;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			FireMe(&bullet[i],goodguy->x,goodguy->y,(byte)Random(256),BLT_SCANSHOT,goodguy->friendly);
			bullet[i].target=victim->ID;
			count++;
			if(count==8)
				break;
		}
}

void FireBulletZ(int x,int y,int z,byte facing,byte type,byte friendly)
{
	int i;

	for(i=0;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			FireMe(&bullet[i],x,y,facing,type,friendly);
			bullet[i].z=z;
			break;
		}
}

// this only fires if there is room in the bullet list PAST a specific point
// this is used for the Megabeam to ensure that all the laser bits stay lined up nicely
void FireBulletAfter(int x,int y,byte facing,byte type,bullet_t *thisone,byte friendly)
{
	int i,j,start;

	start=-1;

	for(j=0;j<config.numBullets;j++)
		if(&bullet[j]==thisone)
		{
			start=j+1;
			break;
		}

	if(start==-1)
		return;

	for(i=start;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			FireMe(&bullet[i],x,y,facing,type,friendly);
			break;
		}
}

void FireExactBullet(int x,int y,int z,int dx,int dy,int dz,byte anim,byte timer,byte facing,byte type,byte friendly)
{
	int i;

	for(i=0;i<config.numBullets;i++)
		if(!bullet[i].type)
		{
			bullet[i].friendly=friendly;
			bullet[i].x=x;
			bullet[i].y=y;
			bullet[i].z=z;
			bullet[i].bright=0;
			bullet[i].dx=dx;
			bullet[i].dy=dy;
			bullet[i].dz=dz;
			bullet[i].anim=anim;
			bullet[i].timer=timer;
			bullet[i].facing=facing;
			bullet[i].type=type;
			bullet[i].target=65535;
			break;
		}
}

void SkullLaunch(int x,int y,byte facing,byte count,byte flags)
{
	byte angle,newfacing;

	MakeSound(SND_SKULLSHOOT,x,y,SND_CUTOFF,1200);
	if(count==1 || count==3 || count==5)	// 1,3,5 have direct forward fire
	{
		angle=facing*32;
		FireBullet(x,y,angle,BLT_SKULL,1);
	}
	if(count==2 || count==4)	// these have slight off-angle double forward fire
	{
		angle=facing*32-8;
		FireBullet(x,y,angle,BLT_SKULL,1);
		angle=facing*32+8;
		FireBullet(x,y,angle,BLT_SKULL,1);
	}
	if(count==3 || count==5)	// these have 45 degree angle fire
	{
		angle=facing*32-32;
		FireBullet(x,y,angle,BLT_SKULL,1);
		angle=facing*32+32;
		FireBullet(x,y,angle,BLT_SKULL,1);
	}
	if(count==4 || count==5)	// these add almost 90 degree off fire
	{
		angle=facing*32-56;
		FireBullet(x,y,angle,BLT_SKULL,1);
		angle=facing*32+56;
		FireBullet(x,y,angle,BLT_SKULL,1);
	}
	if(flags&HMR_REVERSE)
	{
		newfacing=((byte)(facing-4))%8;
		SkullLaunch(x,y,newfacing,count,flags&(~HMR_REVERSE));
	}
}

void Armageddon(Map *map,int x,int y,byte friendly)
{
	x=x-400*FIXAMT+MGL_randoml(800*FIXAMT);
	y=y-300*FIXAMT+MGL_randoml(600*FIXAMT);

	if(x<0 || y<0 || x>=map->width*TILE_WIDTH*FIXAMT || y>=map->height*TILE_HEIGHT*FIXAMT)
		return;

	//if(map->map[(x>>FIXSHIFT)/TILE_WIDTH+((y>>FIXSHIFT)/TILE_HEIGHT)*map->width].wall)
	//	return;

	FireBullet(x,y,0,BLT_COMET,friendly);
	//FireExactBullet(x,y,0,0,0,0,0,14,0,BLT_LIQUIFY);
	MakeSound(SND_FLAMEGO,x,y,SND_CUTOFF,100);
}

void HammerLaunch(int x,int y,byte facing,byte count,byte flags)
{
	byte angle,newfacing;
	byte type,spd,face,timer;
	int height,dz;

	if(player.cheesePower)
	{
		type=BLT_CHEESEHAMMER;
		face=facing*32;
		spd=14;
		height=FIXAMT*15;
		dz=0;
		timer=50;
	}
	else if(flags&HMR_REFLECT)
	{
		type=BLT_HAMMER2;
		face=facing;
		spd=12;
		height=FIXAMT*20;
		dz=FIXAMT*6;
		timer=30;
	}
	else
	{
		type=BLT_HAMMER;
		face=facing;
		spd=12;
		height=FIXAMT*20;
		dz=FIXAMT*6;
		timer=30;
	}
	if(player.playAs==PLAY_LUNACHIK)
	{
		if(type==BLT_HAMMER)
			type=BLT_LUNA;
		else if(type==BLT_HAMMER2)
			type=BLT_LUNA2;
		MakeSound(SND_LUNASHOOT,x,y,SND_CUTOFF,1200);
	}
	else if(player.playAs==PLAY_MYSTIC)
	{
		if(type==BLT_HAMMER)
			type=BLT_FIREBALL;
		else if(type==BLT_HAMMER2)
			type=BLT_FIREBALL2;
		MakeSound(SND_FBALLLAUNCH,x,y,SND_CUTOFF,1200);
	}
	else
		MakeSound(SND_HAMMERTOSS,x,y,SND_CUTOFF,1200);
	if(count==1 || count==3 || count==5)	// 1,3,5 have direct forward fire
	{
		angle=facing*32;
		FireExactBullet(x,y,height,Cosine(angle)*spd,Sine(angle)*spd,dz,0,timer,face,type,1);
	}
	if(count==2 || count==4)	// these have slight off-angle double forward fire
	{
		angle=facing*32-8;
		FireExactBullet(x,y,height,Cosine(angle)*spd,Sine(angle)*spd,dz,0,timer,face,type,1);
		angle=facing*32+8;
		FireExactBullet(x,y,height,Cosine(angle)*spd,Sine(angle)*spd,dz,0,timer,face,type,1);
	}
	if(count==3 || count==5)	// these have 45 degree angle fire
	{
		angle=facing*32-32;
		newfacing=((byte)(facing-1))%8;
		if(player.cheesePower)
			face=newfacing*32;
		else
			face=newfacing;
		FireExactBullet(x,y,height,Cosine(angle)*spd,Sine(angle)*spd,dz,0,timer,face,type,1);
		angle=facing*32+32;
		newfacing=(facing+1)%8;
		if(player.cheesePower)
			face=newfacing*32;
		else
			face=newfacing;
		FireExactBullet(x,y,height,Cosine(angle)*spd,Sine(angle)*spd,dz,0,timer,face,type,1);
	}
	if(count==4 || count==5)	// these add almost 90 degree off fire
	{
		angle=facing*32-56;
		newfacing=((byte)(facing-2))%8;
		if(player.cheesePower)
			face=newfacing*32;
		else
			face=newfacing;
		FireExactBullet(x,y,height,Cosine(angle)*spd,Sine(angle)*spd,dz,0,timer,face,type,1);
		angle=facing*32+56;
		newfacing=(facing+2)%8;
		if(player.cheesePower)
			face=newfacing*32;
		else
			face=newfacing;
		FireExactBullet(x,y,height,Cosine(angle)*spd,Sine(angle)*spd,dz,0,timer,face,type,1);
	}
	if(flags&HMR_REVERSE)
	{
		newfacing=((byte)(facing-4))%8;
		HammerLaunch(x,y,newfacing,count,flags&(~HMR_REVERSE));
	}
}

void HappyFire(int x,int y,byte facing)
{
	byte happyList[]={BLT_HAMMER,BLT_HAMMER2,BLT_MISSILE,BLT_FLAME,BLT_LASER,BLT_BOMB,
					  BLT_BALLLIGHTNING,BLT_BIGSHELL,BLT_BIGAXE,BLT_LIGHTNING,BLT_SPEAR,BLT_SLASH,
					  BLT_GREEN,BLT_TORPEDO,BLT_BUBBLE,BLT_LUNA};
	byte b;

	b=happyList[Random(16)];
	if(b==BLT_BOMB || b==BLT_GREEN || b==BLT_BIGSHELL || b==BLT_BUBBLE)
		facing*=32;
	FireBullet(x,y,facing,b,1);
}

void HappyLaunch(int x,int y,byte facing,byte count,byte flags)
{
	MakeSound(SND_HAMMERTOSS,x,y,SND_CUTOFF,1200);
	if(count==1 || count==3 || count==5)	// 1,3,5 have direct forward fire
	{
		HappyFire(x,y,facing);
	}
	if(count==2 || count==4)	// these have slight off-angle double forward fire
	{
		HappyFire(x,y,facing);
		if(Random(2)==0)
			HappyFire(x,y,(facing-1)&7);
		else
			HappyFire(x,y,(facing+1)&7);
	}
	if(count==3 || count==5)	// these have 45 degree angle fire
	{
		HappyFire(x,y,(facing-1)&7);
		HappyFire(x,y,(facing+1)&7);
	}
	if(count==4 || count==5)	// these add almost 90 degree off fire
	{
		HappyFire(x,y,(facing-2)&7);
		HappyFire(x,y,(facing+2)&7);
	}
	if(flags&HMR_REVERSE)
	{
		HappyLaunch(x,y,(facing+4)&7,count,(flags&(~HMR_REVERSE)));
	}
}

void ShroomFire(int x,int y,byte facing)
{
	byte a;
	int i;

	for(i=0;i<3;i++)
	{
		a=(facing*32+16-Random(33))&255;
		FireExactBullet(x,y,FIXAMT*16,Cosine(a)*6,Sine(a)*6,0,0,20,a,BLT_SPORE,1);
	}
}

void ShroomSpew(int x,int y,byte facing,byte count,byte flags)
{
	MakeSound(SND_MUSHSPORES,x,y,SND_CUTOFF,1200);

	if(count==1 || count==3 || count==5)	// 1,3,5 have direct forward fire
	{
		ShroomFire(x,y,facing);
	}
	if(count==2 || count==4)	// these have slight off-angle double forward fire
	{
		ShroomFire(x,y,facing);
		if(Random(2)==0)
			ShroomFire(x,y,(facing-1)&7);
		else
			ShroomFire(x,y,(facing+1)&7);
	}
	if(count==3 || count==5)	// these have 45 degree angle fire
	{
		ShroomFire(x,y,(facing-1)&7);
		ShroomFire(x,y,(facing+1)&7);
	}
	if(count==4 || count==5)	// these add almost 90 degree off fire
	{
		ShroomFire(x,y,(facing-2)&7);
		ShroomFire(x,y,(facing+2)&7);
	}
	if(flags&HMR_REVERSE)
	{
		ShroomSpew(x,y,(facing+4)&7,count,(flags&(~HMR_REVERSE)));
	}
}

void ReflectShot(void)
{
	reflect=1;
}

void BulletSwap(int sx,int sy,int width,int height,int dx,int dy)
{
	int i;

	sx*=(TILE_WIDTH*FIXAMT);
	sy*=(TILE_HEIGHT*FIXAMT);
	dx*=(TILE_WIDTH*FIXAMT);
	dy*=(TILE_HEIGHT*FIXAMT);
	width*=(TILE_WIDTH*FIXAMT);
	height*=(TILE_HEIGHT*FIXAMT);

	for(i=0;i<config.numBullets;i++)
	{
		if(bullet[i].type)
		{
			if(bullet[i].x>=sx && bullet[i].y>=sy && bullet[i].x<=(sx+width) && bullet[i].y<=(sy+height))
			{
				// in target area, swap
				bullet[i].x+=(-sx+dx);
				bullet[i].y+=(-sy+dy);
			}
			else if(bullet[i].x>=dx && bullet[i].y>=dy && bullet[i].x<=(dx+width) && bullet[i].y<=(dy+height))
			{
				// in other target area, swap
				bullet[i].x+=(-dx+sx);
				bullet[i].y+=(-dy+sy);
			}
		}
	}
}

void ReflectBullets(int x,int y,byte size,byte friendly)
{
	int i;
	int rx,ry,rx2,ry2;

	rx=x-size*FIXAMT;
	rx2=x+size*FIXAMT;
	ry=y-size*FIXAMT;
	ry2=y+size*FIXAMT;

	for(i=0;i<config.numBullets;i++)
	{
		if(bullet[i].type && bullet[i].friendly!=friendly && bullet[i].x>rx &&
			bullet[i].y>ry && bullet[i].x<rx2 && bullet[i].y<ry2)
		{
			if(bullet[i].type==BLT_BADSITFLAME)
				continue;	// no reflecting those
			bullet[i].friendly=friendly;
			bullet[i].dx=-bullet[i].dx;
			bullet[i].dy=-bullet[i].dy;
			if(bullet[i].type==BLT_ICESPIKE || bullet[i].type==BLT_DIRTSPIKE)
				bullet[i].facing=(bullet[i].facing+4)&7;
		}
	}
}

void RemoveOrbiters(int n,byte f,byte t)
{
	int i;

	if(n==0)
		return;

	for(i=0;i<config.numBullets;i++)
	{
		if(bullet[i].type==BLT_ORBITER && bullet[i].friendly==f && bullet[i].type==t)
		{
			MakeSound(SND_BOMBBOOM,bullet[i].x,bullet[i].y,SND_CUTOFF,2000);
			bullet[i].dx=0;
			bullet[i].dy=0;
			bullet[i].dz=0;
			bullet[i].type=BLT_BOOM;
			bullet[i].timer=7;
			bullet[i].anim=0;
			n--;
			if(n==0)
				return;
		}
	}

}

void MakeRadar(int rx,int ry,byte w)
{
	if(rx==255)
		return;

	rx=(rx*TILE_WIDTH+TILE_WIDTH/2)*FIXAMT;
	ry=(ry*TILE_HEIGHT+TILE_HEIGHT/2)*FIXAMT;

	TrackParticle(w,goodguy->x,goodguy->y,rx,ry);
/*	int camX,camY;

	if(rx==255)
		return;

	GetCamera(&camX,&camY);
	rx=rx*TILE_WIDTH-(camX-320);
	ry=ry*TILE_HEIGHT-(camY-240);

	if(rx<TILE_WIDTH/2)
		rx=TILE_WIDTH/2;

	if(ry<TILE_HEIGHT/2)
		ry=TILE_HEIGHT/2;

	if(rx>640-TILE_WIDTH/2)
		rx=640-TILE_WIDTH/2;

	if(ry>480-TILE_HEIGHT/2)
		ry=480-TILE_HEIGHT/2;

	rx+=(camX-320)+TILE_WIDTH/2;
	ry+=(camY-240)+TILE_HEIGHT/2;
	FireExactBullet(rx*FIXAMT,ry*FIXAMT,FIXAMT,0,0,0,0,20,0,BLT_SCANLOCK,0);
	*/
}

int CountBullets(byte type)
{
	int count = 0;

	for(int i=0;i<config.numBullets;i++)
		if(bullet[i].type == type)
			++count;

	return count;
}

int CountBulletsInRect(byte type,int x,int y,int x2,int y2)
{
	int count = 0;

	for(int i=0;i<config.numBullets;i++) {
		int tx = bullet[i].x / TILE_WIDTH / FIXAMT;
		int ty = bullet[i].y / TILE_HEIGHT / FIXAMT;
		if(bullet[i].type==type && tx>=x && ty>=y && tx<=x2 && ty<=y2)
			++count;
	}

	return count;
}

void ChangeBullet(byte fx,int x,int y,int type,int newtype)
{
	for(int i=0;i<config.numBullets;i++)
		if(bullet[i].type != newtype && ((type != 0 && bullet[i].type == type) || (type == 0 && bullet[i].type != 0)))
			if (x == 255 || ((bullet[i].x >> FIXSHIFT)/TILE_WIDTH == x && (bullet[i].y >> FIXSHIFT)/TILE_HEIGHT == y))
			{
				int dx = bullet[i].dx, dy = bullet[i].dy, f = bullet[i].facing;

				if (type == BLT_LASER)
					f /= 2;

				if (BulletFacingType(newtype) == 0)
					f = 0;
				else if (BulletFacingType(newtype) == 7 && BulletFacingType(type) == 255)
					f /= 32;
				else if (BulletFacingType(newtype) == 255 && BulletFacingType(type) == 7)
					f *= 32;

				FireMe(&bullet[i],bullet[i].x,bullet[i].y,f,newtype,bullet[i].friendly);
				bullet[i].dx = dx;
				bullet[i].dy = dy;
				if (fx)
					BlowSmoke(bullet[i].x, bullet[i].y, 0, FIXAMT/8);
			}

}

byte GetBulletAttackType(void)
{
	return attackType;
}

// TORPEDO, LASER
static const byte bulletFacingType[NUM_BULLETS] = {
	0,  	// BLT_NONE    0
	7,  	// BLT_HAMMER  1
	7,  	// BLT_HAMMER2 2	// this is a hammer with reflection
	7,  	// BLT_MISSILE 3
	7,  	// BLT_FLAME   4
	7,  	// BLT_LASER	5
	255,  	// BLT_ACID	6
	255,  	// BLT_BOMB	7
	0,  	// BLT_BOOM	8
	255,  	// BLT_ENERGY  9
	0,  	// BLT_MEGABEAM 10		// this is a ball of energy that launches off a megabeam1
	0,  	// BLT_MEGABEAM1 11	// this is a huge laser beam (downward)
	0,  	// BLT_MEGABEAM2 12	// this is the laser hitting an object (just a visual effect)
	7,  	// BLT_FLAME2	13	// just like flame, except it is anti-Bouapha
	255,  	// BLT_SPORE	14
	7,  	// BLT_SHROOM  15
	255,  	// BLT_GRENADE 16	// energy grenade, an enemy weapon
	0,  	// BLT_YELBOOM 17	// yellow explosion made by energy grenade
	0,  	// BLT_SHOCKWAVE 18	// purple shockwave, for Super Zombie stomp
	0,  	// BLT_LILBOOM 19	// explosion made by missile
	255,  	// BLT_SNOWBALL 20
	255,  	// BLT_BIGSNOW  21
	7,  	// BLT_ICESPIKE 22	// spike juts out of the ground
	7,  	// BLT_ROCK	23
	255,  	// BLT_SPINE	24 // cactus spine
	7,  	// BLT_EVILHAMMER 25 // a grey hammer that is anti-bouapha
	255,  	// BLT_BIGSHELL 26	// Bouapha's power armor shoots these
	7,  	// BLT_BIGAXE	27	// Bouapha weapon
	0,  	// BLT_LIGHTNING 28 // Bouapha weapon
	7,  	// BLT_SPEAR	29	// Bouapha's version of the pygmy spear
	0,  	// BLT_SLASH	30	// Bouapha's machete slash
	0,  	// BLT_MINE	31	// Bouapha's mines
	7,  	// BLT_BADSPEAR 32	// pygmy-thrown spear
	7,  	// BLT_ORBITER  33	// guy that flies around Bouapha shooting
	255,  	// BLT_GREEN	34	// friendly green bullets
	7,  	// BLT_BALLLIGHTNING 35
	0,  	// BLT_LIGHTNING2 36
	7,  	// BLT_MINDWIPE	37
	0,  	// BLT_REFLECT	38
	7,  	// BLT_SWAP	39
	255,  	// BLT_SHARK	40
	7,  	// BLT_ORBITER2 41	// orbit bomber
	7,  	// BLT_HARPOON	42	// a spear, underwater - only difference is it sinks much more slowly
	7,  	// BLT_SCANNER	43	// what the scanner scans with
	255,  	// BLT_SCANSHOT 44	// the shots the scanner fires after scanning
	7,  	// BLT_TORPEDO	45	// Bouapha's minisub shoots them
	7,  	// BLT_DIRTSPIKE 46	// ice spike in brown
	255,  	// BLT_PAPER	47
	0,  	// BLT_SCANLOCK 48	// the scanner locked onto a guy
	255,  	// BLT_BUBBLE	49
	255,  	// BLT_FREEZE	50
	0,  	// BLT_BUBBLEPOP 51	// a bubble that is popping, just visual effect
	0,  	// BLT_LILBOOM2 52		// a harmless lilboom
	7,  	// BLT_CHEESEHAMMER 53	// enhanced hammers
	255,  	// BLT_FREEZE2	54		// a freeze bullet that drops like acid bullets and splats
	7,  	// BLT_LUNA	55		// lunachick's bullets
	7,  	// BLT_LUNA2	56		// lunachick's bullets with wall-bounce power
	7,		// BLT_ICEBEAM  57
	0,		// BLT_ICEBEAM2 58 // dud ice beam, for when it hits walls
	7,		// BLT_DEATHBEAM 59	// clone of icebeam, but deadly to player
	0,		// BLT_DEATHBEAM2 60	// the dud version for when it hits walls
	0,		// BLT_SLIME 61
	7,		// BLT_BIGYELLOW 62	// big yellow bullet (same as grenade, but not lobbed or explosive)
	7,		// BLT_YELWAVE  63	// wavey thing
	255,    // BLT_FLAME3	64	// floaty flame, drifts up
	255,	// BLT_BADSITFLAME 65
	255,	// BLT_HOLESHOT 66	// black hole shot flying
	0,		// BLT_BLACKHOLE 67	// black hole existing
	7,		// turret laser
	255,	// BLT_IGNITE	// flamebringer shot that ignites the target hit
	7,		// BLT_GASBLAST 68
	0,		// BLT_FLAMEWALL
	255,
	255,
	0,
	255,	// BLT_WIND
	255,
	255,	// BLT_ICESHARD
	0,		// BLT_EVILFACE
	255,	// BLT_LIFEBLIP
	7,		// BLT_FIREBALL
	7,		// BLT_FIREBALL2
	255,	// BLT_SKULL
	0,		// BLT_COMET
	0,		// BLT_COMETBOOM
};

byte BulletFacingType(byte type)
{
	SDL_assert(type < NUM_BULLETS);
	return bulletFacingType[type];
}

static const char bulletName[][20] = {
	"Anything",
	"Hammer",
	"Bouncy Hammer",
	"Missile",
	"Flame",
	"AK-8087 Shot",
	"Acid",
	"Cherry Bomb",
	"Explosion",
	"Red Bullet",
	"Megabeam Source",
	"Megabeam Part",
	"Megabeam End",
	"Evil Flame",
	"Spore",
	"Mushroom",
	"Grenade",
	"Grenade Boom",
	"SDZ Shockwave",
	"Missile Boom",
	"Snowball",
	"Big Snowball",
	"Ice Spike",
	"Rock",
	"Cactus Spine",
	"Evil Hammer",
	"Power Shell",
	"Big Axe",
	"Lightning",
	"Spear",
	"Machete",
	"Landmine",
	"Evil Spear",
	"Orbiter",
	"Green Bullet",
	"Ball Lightning",
	"Zap Wand Shock",
	"Mind Control",
	"Reflect Shield",
	"Swap Gun",
	"Water Shot",
	"Orbit Bomber",
	"Harpoon",
	"Scanner",
	"Scanner Shot",
	"Torpedo",
	"Dirt Spike",
	"Paper",
	"Scanner Lock",
	"Bubble",
	"Freeze Ray",
	"Bubble Pop",
	"Harmless Boom",
	"Cheese Hammer",
	"Evil Freeze",
	"Lunachick Ray",
	"Bouncy Lunachick",
	"Ice Beam",
	"Ice Beam End",
	"Death Beam",
	"Death Beam End",
	"Slime",
	"Big Yellow Bullet",
	"Yell Wave",
	"Floaty Flame",
	"Evil Sitting Flame",
	"Black Hole Shot",
	"Black Hole",
	"Turret Laser",
	"Ignite",
	"Poison Gas",
	"Flame Wall",
	"Big Shot",
	"Earth Spike",
	"Mega Boom",
	"Wind",
	"Claw",
	"Swamp Gas",
	"Ice Shard",
	"Evil Face",
	"Life Blip",
	"Fireball",
	"Bouncy Fireball",
	"Skull",
	"Comet",
	"Comet Boom",
};
static_assert(SDL_arraysize(bulletName) == NUM_BULLETS, "Must give new bullets a name");

const char* BulletName(int type)
{
	if (type >= 0 && type < NUM_BULLETS)
		return bulletName[type];
	return "???";
}
