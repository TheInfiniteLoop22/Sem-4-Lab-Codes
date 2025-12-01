echo "Enter numbers separated by spaces:"
read -a arr
sum=0
for num in "${arr[@]}"; do
	sum=$((sum+num))
done
echo "Sum is $sum"
