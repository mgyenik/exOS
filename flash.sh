commands="speed 12000
exec device = at91sam3s4b
erase
loadbin ./out.bin,0x400000
r
g
q"

echo "$commands" | JLinkExe
