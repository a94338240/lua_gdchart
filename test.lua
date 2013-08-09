#!/usr/bin/lua

chart = require("lua_gdchart")
data = {{x = "1", y = 43}, 
		{x = "2", y = 66}, 
		{x = "3", y = 22}}
a = chart.draw{dataset = data, file = "chart.png", width = 500, height = 400, ylabel_fmt = "%2.0f%%"}
if not a then print("error") end
