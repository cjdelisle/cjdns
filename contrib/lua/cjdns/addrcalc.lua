-- Cjdns address conversion functions
-- Translated from Cjdns C code to lua code by Alex <alex@portlandmeshnet.org>

--- @module cjdns.addrcalc
local addrcalc = {}

local bit32 = require("bit32")
local sha2 = require("sha2")

function addrcalc.Base32_decode(input)
	local numForAscii = {
			99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
			99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
			99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9,99,99,99,99,99,99,
			99,99,10,11,12,99,13,14,15,99,16,17,18,19,20,99,
			21,22,23,24,25,26,27,28,29,30,31,99,99,99,99,99,
			99,99,10,11,12,99,13,14,15,99,16,17,18,19,20,99,
			21,22,23,24,25,26,27,28,29,30,31,99,99,99,99,99,
		}
	
	local output = {}
	local outputIndex = 0
	local inputIndex = 0
	local nextByte = 0
	local bits = 0
	
	while inputIndex < string.len(input) do
		i = string.byte(input,inputIndex+1)
		if bit32.band(i,0x80) ~= 0 then
			error("Bad base32 decode input character " .. i)
		end
		
		b = numForAscii[i+1]
		inputIndex = inputIndex + 1
		if b > 31 then
			error("Bad base32 decode input character " .. i)
		end
		
		nextByte = bit32.bor(nextByte, bit32.lshift(b, bits))
		bits = bits + 5
		
		if bits >= 8 then
			output[outputIndex+1] = bit32.band(nextByte, 0xff)
			outputIndex = outputIndex + 1
			bits = bits - 8
			nextByte = bit32.rshift(nextByte, 8)
		end
	end
	
	if bits >= 5 or nextByte ~= 0 then
		error("Bad base32 decode input, bits is " .. bits .. " and nextByte is " .. nextByte);
	end
	
	return string.char(unpack(output));
end


function addrcalc.pkey2ipv6(k)
	if string.sub(k,-2) ~= ".k" then
		error("Invalid key")
	end
	
	kdata = addrcalc.Base32_decode(string.sub(k,1,-3))
	hash = sha2.sha512(kdata)
	hash = sha2.sha512hex(hash)
	addr = string.sub(hash,1,4)
	for i = 2,8 do
		addr = addr .. ":" .. string.sub(hash,i*4-3,i*4)
	end
	
	return addr
end

return addrcalc
