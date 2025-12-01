echo "Enter coefficients a, b, c of quadratic equation (ax^2+bx+c=0):"
read a b c

if [ $a -eq 0 ]; then
  echo "Not a quadratic equation."
  exit 1
fi

d=$((b*b - 4*a*c))

case 1 in
  $((d>0)))
    # Two real and distinct roots
    root1=$(echo "scale=2; (-$b + sqrt($d))/(2*$a)" | bc -l)
    root2=$(echo "scale=2; (-$b - sqrt($d))/(2*$a)" | bc -l)
    echo "Roots are real and distinct: $root1 and $root2"
    ;;
  $((d==0)))
    # Real and equal roots
    root=$(echo "scale=2; -$b/(2*$a)" | bc -l)
    echo "Roots are real and equal: $root"
    ;;
  *)
    # Complex roots
    real=$(echo "scale=2; -$b/(2*$a)" | bc -l)
    imag=$(echo "scale=2; sqrt(-$d)/(2*$a)" | bc -l)
    echo "Roots are complex: $real + $imag i and $real - $imag i"
    ;;
esac

