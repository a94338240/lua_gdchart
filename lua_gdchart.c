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

#include "array_alloc.h"
#include "gdc.h"
#include "gdchart.h"
#include "gdcpie.h"

char *GDC_annotation_font = NULL;
char *GDCPIE_title_font = NULL;
int GDC_xaxis_ptsize = 1;
char *GDC_ytitle_font = NULL;
int GDC_xtitle_ptsize = 1;
char *GDC_xtitle_font = NULL;
int GDC_annotation_ptsize = 1;
char *GDCPIE_label_font = NULL;
int GDC_ytitle_ptsize = 1;
int GDC_title_ptsize = 1;
char *GDC_xaxis_font = NULL;
int GDCPIE_title_ptsize = 1;
char *GDC_title_font = NULL;
int GDCPIE_label_ptsize = 2;

static int lua_gdgraph_draw(lua_State *L)
{
  const char *x[1024] = {0};
  float y[1024] = {0.0};
  int w = 250, h = 200;

  if (!lua_istable(L, -1))
	goto check_args;

  lua_pushstring(L, "dataset");
  lua_gettable(L, -2);
  if (!lua_istable(L, -1)) {
	fprintf(stderr, "dataset is not a table, it's %s\n", lua_typename(L, -1));
	goto check_args;
  }

  lua_pushnil(L);
  int i = 0;
  while (lua_next(L, -2)) {
	lua_pushstring(L, "x");
	lua_gettable(L, -2);
	x[i] = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_pushstring(L, "y");
	lua_gettable(L, -2);
	y[i] = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_pop(L, 1);
	i++;
  }
  lua_pop(L, 1);

  lua_pushstring(L, "file");
  lua_gettable(L, -2);
  if (!lua_isstring(L, -1)) {
	fprintf(stderr, "file is not a string, it's %s\n", lua_typename(L, -1));
	goto check_args;
  }
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
 
  GDC_out_graph(w, h, fp, GDC_BAR, i, (char **)x, 1, y, NULL);

  fclose(fp);
  lua_pushboolean(L, 1);
  return 1;
 check_args:
  return 0;
}

static luaL_Reg funcs[] = {
  {"draw", lua_gdgraph_draw},
  {NULL, NULL}
};

int luaopen_lua_gdchart(lua_State *L)
{
  lua_newtable(L);
  luaL_setfuncs(L, funcs, 0);
  return 1;
}
