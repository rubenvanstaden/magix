inductex lman3.gds -l ixman3.ldf -i lman3.inp -fh
del sol_ffh.txt
ren sol.txt sol_ffh.txt
inductex lman3.gds -l ixman3.ldf -i lman3.geo -th
del sol_tth.txt
ren sol.txt sol_tth.txt
del a.txt
del b.txt
del fastout.out
del ix.cur
del matrix*.*
del *.vtk
del *.info
del Z.txt
del vsb.ver
