password="secret"
echo "Initial pass: $password" #This can read the password.
readonly password #Make password readonly - It makes it a const and it cannot be changed
password="newsecret"
echo "New password= $password"
