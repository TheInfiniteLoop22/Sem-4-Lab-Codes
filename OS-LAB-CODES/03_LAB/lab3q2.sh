echo "Enter the value of n:"
read n
count=0
num=1
echo "First $n odd numbers:"
while [ $count -lt $n ]; do
	echo $num
	num=$((num+2))
	count=$((count+1))
done
