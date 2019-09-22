/*
 *  definitions.h
 */
#pragma once

/*
 *  function defs
 */

typedef bool    (*rank_revel_fn)(float*);
typedef void    (*set_clan_tag_fn)(const char*, const char*);
typedef void    (*random_seed_fn)(int);
typedef int     (*random_int_fn)(int, int);
typedef float   (*random_float_fn)(float, float);

/*
 *  surface
 */

typedef unsigned long long VPANEL;
typedef unsigned long long HFONT;

typedef unsigned char byte;

/*
 *  definision
 */

#define IN_ATTACK       (1 << 0)
#define IN_JUMP         (1 << 1)
#define IN_DUCK         (1 << 2)
#define IN_FORWARD      (1 << 3)
#define IN_BACK         (1 << 4)
#define IN_USE          (1 << 5)
#define IN_CANCEL       (1 << 6)
#define IN_LEFT         (1 << 7)
#define IN_RIGHT        (1 << 8)
#define IN_MOVELEFT     (1 << 9)
#define IN_MOVERIGHT    (1 << 10)
#define IN_ATTACK2      (1 << 11)
#define IN_RUN          (1 << 12)
#define IN_RELOAD       (1 << 13)
#define IN_ALT1         (1 << 14)
#define IN_ALT2         (1 << 15)
#define IN_SCORE        (1 << 16)
#define IN_SPEED        (1 << 17)
#define IN_WALK         (1 << 18)
#define IN_ZOOM         (1 << 19)
#define IN_WEAPON1      (1 << 20)
#define IN_WEAPON2      (1 << 21)
#define IN_BULLRUSH     (1 << 22)
#define IN_GRENADE1     (1 << 23)
#define IN_GRENADE2     (1 << 24)
#define IN_ATTACK3      (1 << 25)

#define FL_ONGROUND     (1<<0)    // At rest / on the ground
#define FL_DUCKING      (1<<1)    // Player flag -- Player is fully crouched
#define FL_WATERJUMP    (1<<2)    // player jumping out of water
#define FL_ONTRAIN      (1<<3)    // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN       (1<<4)    // Indicates the entity is standing in rain
#define FL_FROZEN       (1<<5)    // Player is frozen for 3rd person camera
#define FL_ATCONTROLS   (1<<6)    // Player can't move, but keeps key inputs for controlling another entity
#define FL_CLIENT       (1<<7)    // Is a player
#define FL_FAKECLIENT   (1<<8)    // Fake client, simulated server side; don't send network messages to them
#define FL_INWATER      (1<<9)    // In water


#define CONTENTS_EMPTY      0x0     // No contents
#define CONTENTS_SOLID      0x1     // an eye is never valid in a solid
#define CONTENTS_WINDOW     0x2     // translucent, but not watery (glass)
#define CONTENTS_AUX        0x4
#define CONTENTS_GRATE      0x8     // alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't
#define CONTENTS_SLIME      0x10
#define CONTENTS_WATER      0x20
#define CONTENTS_BLOCKLOS   0x40    // block AI line of sight
#define CONTENTS_OPAQUE     0x80    // things that cannot be seen through (may be non-solid though)
#define LAST_VISIBLE_CONTENTS   CONTENTS_OPAQUE
#define ALL_VISIBLE_CONTENTS    (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))
#define CONTENTS_MOVEABLE       0x4000
#define CONTENTS_PLAYERCLIP     0x10000
#define CONTENTS_MONSTERCLIP    0x20000
#define CONTENTS_MONSTER        0x2000000       // should never be on a brush, only in game
#define CONTENTS_DEBRIS         0x4000000
#define CONTENTS_DETAIL         0x8000000       // brushes to be added after vis leafs
#define CONTENTS_TRANSLUCENT    0x10000000      // auto set if any surface has trans
#define CONTENTS_LADDER         0x20000000
#define CONTENTS_HITBOX         0x40000000      // use accurate hitboxes on trace


#define TEXTURE_GROUP_LIGHTMAP                      "Lightmaps"
#define TEXTURE_GROUP_WORLD                         "World textures"
#define TEXTURE_GROUP_MODEL                         "Model textures"
#define TEXTURE_GROUP_VGUI                          "VGUI textures"
#define TEXTURE_GROUP_PARTICLE                      "Particle textures"
#define TEXTURE_GROUP_DECAL                         "Decal textures"
#define TEXTURE_GROUP_SKYBOX                        "SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS                "ClientEffect textures"
#define TEXTURE_GROUP_OTHER                         "Other textures"
#define TEXTURE_GROUP_PRECACHED                     "Precached"                // TODO: assign texture groups to the precached materials
#define TEXTURE_GROUP_CUBE_MAP                      "CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET                 "RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED                   "Unaccounted textures"    // Textures that weren't assigned a texture group.


#define MAXSTUDIOBONES 128
#define BONE_USED_BY_HITBOX 0x100


#define SURF_LIGHT      0x0001      // value will hold the light strength
#define SURF_SKY2D      0x0002      // don't draw, indicates we should skylight + draw 2d sky but not draw the 3D skybox
#define SURF_SKY        0x0004      // don't draw, but add to skybox
#define SURF_WARP       0x0008      // turbulent water warp
#define SURF_TRANS      0x0010
#define SURF_NOPORTAL   0x0020      // the surface can not have a portal placed on it
#define SURF_TRIGGER    0x0040      // FIXME: This is an xbox hack to work around elimination of trigger surfaces, which breaks occluders
#define SURF_NODRAW     0x0080      // don't bother referencing the texture
#define SURF_HINT       0x0100      // make a primary bsp splitter
#define SURF_SKIP       0x0200      // completely ignore, allowing non-closed brushes
#define SURF_NOLIGHT    0x0400      // Don't calculate light
#define SURF_BUMPLIGHT  0x0800      // calculate three lightmaps for the surface for bumpmapping
#define SURF_NOSHADOWS  0x1000      // Don't receive shadows
#define SURF_NODECALS   0x2000      // Don't receive decals
#define SURF_NOPAINT    SURF_NODECALS   // the surface can not have paint placed on it
#define SURF_NOCHOP     0x4000      // Don't subdivide patches on this surface
#define SURF_HITBOX     0x8000      // surface is part of a hitbox


#define MASK_ALL            (0xFFFFFFFF)
#define MASK_SOLID          (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define MASK_PLAYERSOLID    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define MASK_NPCSOLID       (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define MASK_NPCFLUID       (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define MASK_WATER          (CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
#define MASK_OPAQUE         (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
#define MASK_VISIBLE        (MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define MASK_VISIBLE_AND_NPCS   (MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define MASK_SHOT           (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define MASK_SHOT_BRUSHONLY (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
#define MASK_SHOT_HULL      (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)


#define SEQUENCE_DEFAULT_DRAW               0
#define SEQUENCE_DEFAULT_IDLE1              1
#define SEQUENCE_DEFAULT_IDLE2              2
#define SEQUENCE_DEFAULT_LIGHT_MISS1        3
#define SEQUENCE_DEFAULT_LIGHT_MISS2        4
#define SEQUENCE_DEFAULT_HEAVY_MISS1        9
#define SEQUENCE_DEFAULT_HEAVY_HIT1         10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB     11
#define SEQUENCE_DEFAULT_LOOKAT01           12

#define SEQUENCE_BUTTERFLY_DRAW             0
#define SEQUENCE_BUTTERFLY_DRAW2            1
#define SEQUENCE_BUTTERFLY_LOOKAT01         13
#define SEQUENCE_BUTTERFLY_LOOKAT03         15

#define SEQUENCE_FALCHION_IDLE1             1
#define SEQUENCE_FALCHION_HEAVY_MISS1       8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP    9
#define SEQUENCE_FALCHION_LOOKAT01          12
#define SEQUENCE_FALCHION_LOOKAT02          13

#define SEQUENCE_DAGGERS_IDLE1              1
#define SEQUENCE_DAGGERS_LIGHT_MISS1        2
#define SEQUENCE_DAGGERS_LIGHT_MISS5        6
#define SEQUENCE_DAGGERS_HEAVY_MISS2        11
#define SEQUENCE_DAGGERS_HEAVY_MISS1        12

#define SEQUENCE_BOWIE_IDLE1                1
