#!/usr/bin/lua

chart = require("lua_gdchart")
data = {{x = "1", y = 43}, 
		{x = "2", y = 66}, 
		{x = "3", y = 22}}
chart.style{
  			--width = 400, 
			--hight = 350, 
			title = "test", 
			xtitle = "X axis",
			ytitle = "Y axis",
			ytitle2 = "Y axis2",
			--title_size = ,
			--xtitle_size = ,
			--ytitle_size = ,
			--xaxisfont_size = ,
			--yaxisfont_size = ,
			ylabel_fmt = "%.2f%%",
			ylabel2_fmt = "%.2f%%",
			--xlabel_spacing = 5,
			--ylabel_density = 80,
			--requested_ymin = ,
			--requested_ymax = ,
			--requested_yinterval = ,
			--zshelf = ,
			--grid = true,
			--xaxis = true,
			--yaxis = true,
			--yaxis2 = true,
			--yval_style = true,
			--stack_type = nil
			_3d_depth = 2,
			--_3d_angle = 45,
			--bar_width = ,
}
a = chart.draw{dataset = data, file = "chart.png"}
if not a then print("error") end
