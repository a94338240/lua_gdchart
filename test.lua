#!/usr/bin/lua

chart = require("lua_gdchart")
data = {["1"] = 43, ["2"] = 66, ["3"] = 22}
a = chart.draw{dataset = data, file = "chart.png", width = 500, height = 400}
if not a then print("error") end
