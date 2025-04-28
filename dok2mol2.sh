path="/home/ylujiang/zzjun/UpCode/TwoDot/tools/dok2mol2"

for i in $(grep Score $1|wc -l|xargs -I {} seq {}); do 
    $path $1 $2 $i >> $3;
    echo "" >> $3;
done