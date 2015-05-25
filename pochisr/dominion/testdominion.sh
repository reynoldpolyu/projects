temp=$(mktemp)
time_us=100000

trap "rm $temp" EXIT

for (( i = 1; i <= 1000000; i++ )); do
	echo '##################'
	echo SEED = $i
	echo '##################'

	echo $i 1>&2

	./fdiff <(stdbuf -o0 ./testdominion $i $time_us 2>&1) \
		<(stdbuf -o0 ./testdominion_alt $i $time_us 2>&1) 10 >$temp
	if [[ $? == 1 ]]; then
		cat $temp
	fi
done
