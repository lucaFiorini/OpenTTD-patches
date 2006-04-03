/* $Id$ */

#ifndef WATER_MAP_H
#define WATER_MAP_H

typedef enum WaterTileType {
	WATER_CLEAR,
	WATER_COAST,
	WATER_LOCK,
	WATER_DEPOT,
} WaterTileType;

typedef enum DepotPart {
	DEPOT_NORTH = 0x80,
	DEPOT_SOUTH = 0x81,
	DEPOT_END   = 0x84,
} DepotPart;

typedef enum LockPart {
	LOCK_MIDDLE = 0x10,
	LOCK_LOWER  = 0x14,
	LOCK_UPPER  = 0x18,
	LOCK_END    = 0x1C
} LockPart;

static inline WaterTileType GetWaterTileType(TileIndex t)
{
	if (_m[t].m5 == 0) return WATER_CLEAR;
	if (_m[t].m5 == 1) return WATER_COAST;
	if (IS_INT_INSIDE(_m[t].m5, LOCK_MIDDLE, LOCK_END)) return WATER_LOCK;
	if (IS_INT_INSIDE(_m[t].m5, DEPOT_NORTH, DEPOT_END)) return WATER_DEPOT;

	assert(0);
}

static inline bool IsWater(TileIndex t)
{
	return GetWaterTileType(t) == WATER_CLEAR;
}

static inline bool IsCoast(TileIndex t)
{
	return GetWaterTileType(t) == WATER_COAST;
}

static inline bool IsClearWaterTile(TileIndex t)
{
	return IsTileType(t, MP_WATER) && IsWater(t) && GetTileSlope(t, NULL) == 0;
}

static inline TileIndex GetOtherShipDepotTile(TileIndex t)
{
	return t + (HASBIT(_m[t].m5, 0) ? -1 : 1) * (HASBIT(_m[t].m5, 1) ? TileDiffXY(0, 1) : TileDiffXY(1, 0));
}

static inline TileIndex IsShipDepot(TileIndex t)
{
	return IS_INT_INSIDE(_m[t].m5, DEPOT_NORTH, DEPOT_END);
}

static inline Axis GetShipDepotAxis(TileIndex t)
{
	return (Axis)GB(_m[t].m5, 1, 1);
}

static inline DiagDirection GetLockDirection(TileIndex t)
{
	return (DiagDirection)GB(_m[t].m5, 0, 2);
}


static inline void MakeWater(TileIndex t)
{
	SetTileType(t, MP_WATER);
	SetTileOwner(t, OWNER_WATER);
	_m[t].m2 = 0;
	_m[t].m3 = 0;
	_m[t].m4 = 0;
	_m[t].m5 = 0;
}

static inline void MakeShore(TileIndex t)
{
	SetTileType(t, MP_WATER);
	SetTileOwner(t, OWNER_WATER);
	_m[t].m2 = 0;
	_m[t].m3 = 0;
	_m[t].m4 = 0;
	_m[t].m5 = 1;
}

static inline void MakeShipDepot(TileIndex t, Owner o, DepotPart base, Axis a)
{
	SetTileType(t, MP_WATER);
	SetTileOwner(t, o);
	_m[t].m2 = 0;
	_m[t].m3 = 0;
	_m[t].m4 = 0;
	_m[t].m5 = base + a * 2;
}

static inline void MakeLockTile(TileIndex t, byte section)
{
	SetTileType(t, MP_WATER);
	SetTileOwner(t, OWNER_WATER);
	_m[t].m2 = 0;
	_m[t].m3 = 0;
	_m[t].m4 = 0;
	_m[t].m5 = section;
}

static inline void MakeLock(TileIndex t, DiagDirection d)
{
	TileIndexDiff delta = TileOffsByDir(d);

	MakeLockTile(t, LOCK_MIDDLE + d);
	MakeLockTile(t - delta, LOCK_LOWER + d);
	MakeLockTile(t + delta, LOCK_UPPER + d);
}

#endif
