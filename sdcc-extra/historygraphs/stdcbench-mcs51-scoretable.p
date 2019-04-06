set output "stdcbench-mcs51-score.svg"
set terminal svg size 640,480
set style data lines
set key bottom right
set xlabel "revision"
set arrow from 9618, 151 to 9618, 150
set label "3.6.0" at 9618, 151
set arrow from 10233, 144 to 10233, 143
set label "3.7.0" at 10233, 144
set arrow from 10582, 145 to 10582, 144
set label "3.8.0" at 10582, 145
plot "stdcbench-mcs51-scoretable" using 1:4 title "default", "stdcbench-mcs51-scoretable" using 1:2 title "size", "stdcbench-mcs51-scoretable" using 1:3 title "speed"

