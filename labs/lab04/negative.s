# negative.s
# int main(void) {
#     int n;
#     printf("Enter a number: ");
#     scanf("%d",&n);
#     if ( n > 0 ) {
#         printf("You have entered a positive number.\n");
#     } else if ( n < 0) {
#         printf("Don't be so negative!\n");
#     } else {
#         printf("You have entered zero.\n");
#     } 
#     return 0;
# }

### Global data

   .data

input_msg:
   .asciiz "Enter a number: "
positive_msg:
   .asciiz "You have entered a positive number.\n"
zero_msg:
   .asciiz "You have entered zero.\n"
negative_msg:
   .asciiz "Don't be so negative!\n"


### main() function
   .text
   .globl main

main:
   la    $a0, input_msg
   li    $v0, 4
   syscall                  # printf("Enter a number: ");

   li    $v0, 5
   syscall                  # scanf("%d", into $v0)
   move  $s0, $v0           # store n in $s0

   blez  $s0, negative      # if (n <= 0) goto negative
   la    $a0, positive_msg
   li    $v0, 4
   syscall                  # printf("You have entered a positive number.\n")
   j     end                # goto end

negative:
   bgez  $s0, zero          # if (n >= 0) goto zero
   la    $a0, negative_msg
   li    $v0, 4
   syscall                  # printf("Don't be so negative!\n")
   j     end                # goto end

zero:
   la    $a0, zero_msg 
   li    $v0, 4
   syscall                  # printf("You have entered zero.\n")

end:
   li    $v0, 0
   jr    $ra                # return 0

