time_start=$(($(date +%s%N)/1000000))
./run22wreads
time_end=$(($(date +%s%N)/1000000))
let time_total=time_end-time_start
echo "Total time in milliseconds:"
echo $time_total

