#!/usr/bin/lua

chart = require("lua_gdchart")
data = {["1"] = 43, ["2"] = 66, ["3"] = 22}
chart.draw{dataset = data, file = chart.gif, width = 500, height = 400}
