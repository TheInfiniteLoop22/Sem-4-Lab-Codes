echo "Name of script: $0"
echo "First argument: $1"
echo "A lsit of all arguments: $*"
echo "No of arguments: $#"
echo "This script places the date in a temp file called $1.$$"
date >"$1.$$"
ls "$1.$$"
rm "$1.$$"

