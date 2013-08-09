/*
    lua_gdchart - Lua GDChart bind.
    Copyright (C) 2013  David Wu <david@pocograph.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <values.h>
#include <lua.h>
#include <lauxlib.h> 

#include "gdc.h"
#include "gdchart.h"

static int lua_gdgraph_draw(lua_State *L)
{
  const char *x[1024] = {0};
  float y[1024] = {0.0};
  int w = 250, h = 200;

  if (!lua_istable(L, -1))
	goto check_args;

  lua_pushstring(L, "dataset");
  lua_gettable(L, -2);
  if (!lua_istable(L, -1))
	goto check_args;
  lua_pushnil(L);
  int i = 0;
  while (lua_next(L, -2)) {
	const char *key = lua_tostring(L, -2);
	float value = lua_tonumber(L, -1);
	x[i] = key;
	y[i] = value;
	lua_pop(L, 1);
	i++;
  }
  lua_pop(L, 1);

  lua_pushstring(L, "file");
  lua_gettable(L, -2);
  if (!lua_isstring(L, -1))
	goto check_args;
  FILE *fp = fopen(lua_tostring(L, -1), "w+");
  lua_pop(L, 1);

  lua_pushstring(L, "width");
  lua_gettable(L, -2);
  if (lua_isnumber(L, -1))
	w = lua_tonumber(L, -1);
  lua_pop(L, 1);

  lua_pushstring(L, "height");
  lua_gettable(L, -2);
  if (lua_isnumber(L, -1))
	h = lua_tonumber(L, -1);
  lua_pop(L, 1);

  unsigned long sc[1]    = { 0xFF8080 };
 
  GDC_BGColor   = 0xFFFFFFL;
  GDC_LineColor = 0x000000L;
  GDC_SetColor  = &(sc[0]);
  GDC_stack_type = GDC_STACK_BESIDE;
	
  GDC_out_graph(w, h, fp, GDC_BAR, i, (char **)x, 1, y, NULL);

  lua_pushboolean(L, 1);
  return 1;
 check_args:
  return 0;
}

static luaL_Reg funcs[] = {
  {"draw", lua_gdgraph_draw},
  {NULL, NULL}
};

int luaopen_gdgrapd(lua_State *L)
{
  lua_newtable(L);
  luaL_setfuncs(L, funcs, 0);
  return 1;
}
