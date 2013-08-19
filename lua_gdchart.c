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
int GDC_annotation_ptsize = 2;

char *GDC_title_font = NULL;
int GDC_title_ptsize = 2;


char *GDC_ytitle_font = NULL;
double GDC_ytitle_ptsize = 2;

char *GDC_xtitle_font = NULL;
double GDC_xtitle_ptsize = 2;

char *GDC_xaxis_font = NULL;
double GDC_xaxis_ptsize = 2;

char *GDCPIE_title_font = NULL;
double GDCPIE_title_ptsize = 2;

char *GDCPIE_label_font = NULL;
double GDCPIE_label_ptsize = 2;

static int chart_type = GDC_BAR;
static int width = 400;
static int height = 350;
static unsigned long set_colors[1024] = {0};

static int lua_table_check(lua_State *L, const char *key)
{
  lua_pushstring(L, key);
  lua_gettable(L, -2);
  int check = lua_isnil(L, -1);
  lua_pop(L, 1);
  return !check;
}

static char *lua_table_get_string(lua_State *L, const char *key)
{
  char *val = NULL;

  lua_pushstring(L, key);
  lua_gettable(L, -2);
  if (lua_isstring(L, -1))
	val = (char *)lua_tostring(L, -1);
  lua_pop(L, 1);
  return val;
}

static double lua_table_get_number(lua_State *L, const char *key)
{
  double val = 0;

  lua_pushstring(L, key);
  lua_gettable(L, -2);
  if (lua_isnumber(L, -1))
	val = lua_tonumber(L, -1);
  lua_pop(L, 1);
  return val;
}

static int lua_table_get_integer(lua_State *L, const char *key)
{
  int val = 0;

  lua_pushstring(L, key);
  lua_gettable(L, -2);
  if (lua_isnumber(L, -1))
	val = lua_tointeger(L, -1);
  lua_pop(L, 1);
  return val;
}

static int lua_table_get_unsigned(lua_State *L, const char *key)
{
  int val = 0;

  lua_pushstring(L, key);
  lua_gettable(L, -2);
  if (lua_isnumber(L, -1))
	val = lua_tounsigned(L, -1);
  lua_pop(L, 1);
  return val;
}
 
static int lua_table_get_boolean(lua_State *L, const char *key)
{
  int val = 0;

  lua_pushstring(L, key);
  lua_gettable(L, -2);
  if (lua_isboolean(L, -1))
	val = lua_toboolean(L, -1);
  lua_pop(L, 1);
  return val;
}

static void *lua_table_get_userdata(lua_State *L, const char *key)
{
  void *val = 0;

  lua_pushstring(L, key);
  lua_gettable(L, -2);
  if (lua_islightuserdata(L, -1))
	val = lua_touserdata(L, -1);
  lua_pop(L, 1);
  return val;
}

static int lua_gdchart_style(lua_State *L)
{
  if (!lua_istable(L, -1))
	goto check_args;

  if (lua_table_check(L, "width")) width = lua_table_get_number(L, "width");
  if (lua_table_check(L, "height")) height = lua_table_get_number(L, "height");
  if (lua_table_check(L, "title")) GDC_title = lua_table_get_string(L, "title");
  if (lua_table_check(L, "xtitle")) GDC_xtitle = lua_table_get_string(L, "xtitle");
  if (lua_table_check(L, "ytitle")) GDC_ytitle = lua_table_get_string(L, "ytitle");
  if (lua_table_check(L, "ytitle2")) GDC_ytitle2 = lua_table_get_string(L, "ytitle2");
  if (lua_table_check(L, "title_size")) GDC_title_size = lua_table_get_number(L, "title_size");
  if (lua_table_check(L, "xtitle_size")) GDC_xtitle_size = lua_table_get_number(L, "xtitle_size");
  if (lua_table_check(L, "ytitle_size")) GDC_ytitle_size = lua_table_get_number(L, "ytitle_size");
  if (lua_table_check(L, "xaxisfont_size")) GDC_xaxisfont_size = lua_table_get_number(L, "xaxisfont_size");
  if (lua_table_check(L, "yaxisfont_size")) GDC_yaxisfont_size = lua_table_get_number(L, "yaxisfont_size");
  if (lua_table_check(L, "ylabel_fmt")) GDC_ylabel_fmt = lua_table_get_string(L, "ylabel_fmt");
  if (lua_table_check(L, "ylabel2_fmt")) GDC_ylabel2_fmt = lua_table_get_string(L, "ylabel2_fmt");
  if (lua_table_check(L, "xlabel_spacing")) GDC_xlabel_spacing = lua_table_get_integer(L, "xlabel_spacing");
  if (lua_table_check(L, "ylabel_density")) GDC_ylabel_density = lua_table_get_number(L, "ylabel_density");
  if (lua_table_check(L, "requested_ymin")) GDC_requested_ymin = lua_table_get_number(L, "requested_ymin");
  if (lua_table_check(L, "requested_ymax")) GDC_requested_ymax = lua_table_get_number(L, "requested_ymax");
  if (lua_table_check(L, "requested_yinterval")) GDC_requested_yinterval = lua_table_get_number(L, "requested_yinterval");
  if (lua_table_check(L, "zshelf")) GDC_0Shelf = lua_table_get_boolean(L, "zshelf");
  if (lua_table_check(L, "grid")) GDC_grid = lua_table_get_boolean(L, "grid");
  if (lua_table_check(L, "xaxis")) GDC_xaxis = lua_table_get_boolean(L, "xaxis");
  if (lua_table_check(L, "yaxis")) GDC_yaxis = lua_table_get_boolean(L, "yaxis");
  if (lua_table_check(L, "yaxis2")) GDC_yaxis2 = lua_table_get_boolean(L, "yaxis2");
  if (lua_table_check(L, "yval_style")) GDC_yval_style = lua_table_get_boolean(L, "yval_style");
  if (lua_table_check(L, "stack_type")) GDC_yval_style = lua_table_get_boolean(L, "stack_type");
  if (lua_table_check(L, "_3d_depth")) GDC_3d_depth = lua_table_get_number(L, "_3d_depth");
  if (lua_table_check(L, "_3d_angle")) GDC_3d_angle = lua_table_get_number(L, "_3d_angle");
  if (lua_table_check(L, "bar_width")) GDC_bar_width = lua_table_get_number(L, "bar_width");
  if (lua_table_check(L, "HLC_style")) GDC_HLC_style = lua_table_get_number(L, "hlc_style");
  if (lua_table_check(L, "HLC_cap_width")) GDC_HLC_cap_width = lua_table_get_number(L, "hlc_cap_width");
  if (lua_table_check(L, "annotation")) GDC_annotation = lua_table_get_userdata(L, "annotation");
  if (lua_table_check(L, "annotation_font_size")) GDC_annotation_font_size = lua_table_get_number(L, "annotation_font_size");

  if (lua_table_check(L, "bg_color")) GDC_BGColor = lua_table_get_unsigned(L, "bg_color");
  if (lua_table_check(L, "grid_color")) GDC_GridColor = lua_table_get_unsigned(L, "grid_color"); 
  if (lua_table_check(L, "line_color")) GDC_LineColor = lua_table_get_unsigned(L, "line_color"); 
  if (lua_table_check(L, "plot_color")) GDC_PlotColor = lua_table_get_unsigned(L, "plot_color"); 
  if (lua_table_check(L, "vol_color")) GDC_VolColor = lua_table_get_unsigned(L, "vol_color"); 
  if (lua_table_check(L, "title_color")) GDC_TitleColor = lua_table_get_unsigned(L, "title_color"); 
  if (lua_table_check(L, "xtitle_color")) GDC_XTitleColor = lua_table_get_unsigned(L, "xtitle_color"); 
  if (lua_table_check(L, "ytitle_color")) GDC_YTitleColor = lua_table_get_unsigned(L, "ytitle_color"); 
  if (lua_table_check(L, "ytitle2_color")) GDC_YTitle2Color = lua_table_get_unsigned(L, "ytitle2_color"); 
  if (lua_table_check(L, "xlabel_color")) GDC_XLabelColor = lua_table_get_unsigned(L, "xlabel_color"); 
  if (lua_table_check(L, "ylabel_color")) GDC_YLabelColor = lua_table_get_unsigned(L, "ylabel_color"); 
  if (lua_table_check(L, "ylabel2_color")) GDC_YLabel2Color = lua_table_get_unsigned(L, "ylabel2_color"); 
  if (lua_table_check(L, "set_color")) { 
	if (!lua_istable(L, -1))
	  goto out;
	lua_pushnil(L);
	int i = 0;
	while (lua_next(L, -2)) {
	  set_colors[i++] = lua_tounsigned(L, -1);
	  lua_pop(L, 1);
	}
	GDC_SetColor = set_colors;
  out:
	lua_pop(L, 1);
  }

  lua_pushboolean(L, 1);
  return 1;

 check_args:
  return 0;
}
  
static int lua_gdchart_draw(lua_State *L)
{
  const char *x[1024] = {0};
  float y[1024] = {0.0};

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

  GDC_out_graph(width, height, fp, chart_type, i, (char **)x, 1, y, NULL);

  fclose(fp);
  lua_pushboolean(L, 1);
  return 1;
 check_args:
  return 0;
}

static luaL_Reg funcs[] = {
  {"draw", lua_gdchart_draw},
  {"style", lua_gdchart_style},
  {NULL, NULL}
};

int luaopen_lua_gdchart(lua_State *L)
{
  lua_newtable(L);
  luaL_setfuncs(L, funcs, 0);
  return 1;
}
