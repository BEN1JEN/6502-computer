-- compile lfont to asm

local infilen, outfilen = tostring(arg[1]), tostring(arg[2])
local hfilen = outfilen:gsub("%.asm$", ".h")

local infile, outfile = io.open(infilen, "r"), io.open(outfilen, "w")
local hfile = io.open(hfilen, "w")

if not infile then
	error("Couldn't open file `" .. infilen .. "'")
end
if not outfile then
	error("Couldn't open file `" .. outfilen .. "'")
end
if not hfile then
	error("Couldn't open file `" .. hfilen .. "'")
end

local intable = load(infile:read("*all"))()
infile:close()

if type(intable) ~= "table" then
	outfile:close()
	error("Input file `" .. infilen .. "' did not return a table.")
end

outfilen = outfilen:gsub("^.*/", "")

local defname = outfilen:upper():gsub("[^A-Z]", "_")
local varname = outfilen:gsub("%.[a-zA-Z0-9_]-$", ""):gsub("[^a-zA-Z]([a-zA-Z])", string.upper)

outfile:write(".scope font_", varname, "_scope\n\n")

local sizeX, sizeY = intable.size.x or intable.size[1], intable.size.y or intable.size[2]

outfile:write(".segment \"RODATA3\"\n")

outfile:write(varname, "_size_x = " .. tostring(sizeX), "\n")
outfile:write(varname, "_size_y = " .. tostring(sizeY), "\n")

outfile:write(varname, "_letters:\n")
for i = 32, 254 do
	local letter = string.char(i)
	if intable[letter] then
		outfile:write("\t.byte ")
		local first = true
		local number = 0
		local offset = 0
		for y = 1, sizeY do
			for x = 1, sizeX do
				if intable[letter][y]:sub(x, x) == "X" then
					number = number + 2^offset
				end
				offset = offset + 1
				if offset > 7 then
					if not first then
						outfile:write(", ")
					end
					first = false
					outfile:write("$", string.format("0%x", number):sub(-2, -1))
					number = 0
					offset = 0
				end
			end
		end
		if offset > 0 then
			outfile:write(", $")
			outfile:write(string.format("0%x", number):sub(-2, -1))
		end
		outfile:write(" ; ", letter, "\n")
	end
end

outfile:write("\n.export ", varname, "_size_x\n")
outfile:write(".export ", varname, "_size_y\n")
outfile:write(".export ", varname, "_letters\n\n")

outfile:write(".endscope\n")
outfile:close()

hfile:write(".ifndef ", defname, "\n")
hfile:write(defname, " = 1\n")

hfile:write(".import ", varname, "_size_x\n")
hfile:write(".import ", varname, "_size_y\n")
hfile:write(".import ", varname, "_letters\n\n")

hfile:write(".endif\n")
hfile:close()
